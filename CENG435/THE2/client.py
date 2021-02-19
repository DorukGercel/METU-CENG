#!/usr/bin/python

from socket import *
import sys, time, os, hashlib

#TCP Client Function
def tcpClient(server_IP_Address, server_TCP_Port, client_TCP_Port):
    #Predefined size of chunk
    sizeOfChunk = 900
    #Size of file and size of readed data so far
    sizeOfFile = os.stat('./transfer_file_TCP.txt').st_size
    sizeOfReadData = 0

    #Necessary connections for TCP Client are created
    clientSocket = socket(AF_INET, SOCK_STREAM)
    clientSocket.bind(('',client_TCP_Port))
    clientSocket.connect((server_IP_Address, server_TCP_Port))

    #Open file and start reading
    with open('./transfer_file_TCP.txt', 'r') as fileReader:
        while True:
            #Read file as a predefined size of chunk
            fileChunk = fileReader.read(sizeOfChunk)
            sizeOfReadData += len(fileChunk)

            #Place size of chunk, the sent time and file payload, then send them                
            lenPayload = str(len(fileChunk))

            #Check if last file data to send or not, then send the message (different formats)
            if sizeOfReadData == sizeOfFile:
                sendChunk = '!' + lenPayload + '@' + str(time.time()) + '*' + fileChunk
            else:
                sendChunk = lenPayload + '@' + str(time.time()) + '*' + fileChunk

            #Send the message and recieve  the check message from the server
            clientSocket.send(sendChunk.encode())
            serverMessage = clientSocket.recv(10).decode()

            #Read file has ended
            if sizeOfReadData == sizeOfFile:
                break
    #Close socket
    clientSocket.close()
   
#UDP Client Function
def udpClient(server_IP_Address, server_UDP_Port, client_UDP_Port):
     
    #Predefined size of chunk
    sizeOfChunk = 900
    #Size of file and size of readed data so far
    sizeOfFile = os.stat('./transfer_file_UDP.txt').st_size
    sizeOfReadData = 0
    #Current seq no for synchronization
    currentSequenceNo = 0
    #Number of retransfered packages
    noRetransfer = 0
    #Flags for last package and last character
    isLastPackage = False
    sendingEndCharacter = False
    #Counter to close connection after last character is sent
    endConnectionCounter = 5
    #First predefined timeout value
    timeoutValue = 1
    #Max and min achievable timeout values (range of timeout valuess)
    minTimeout = 0.1
    maxTimeout = 10

    #Necessary connections for TCP Client are created
    clientSocket = socket(AF_INET, SOCK_DGRAM)
    clientSocket.bind(('',client_UDP_Port))

    #Open file and start reading
    with open('./transfer_file_UDP.txt', 'r') as fileReader:
        while True:
            #Read file as a predefined size of chunk
            fileChunk = fileReader.read(sizeOfChunk)
            sizeOfReadData += len(fileChunk)

            #If Last Part is read send the file and break, else continue reading and sending
            if sizeOfReadData == sizeOfFile:
                isLastPackage = True

            while True:
                #Place size of chunk, the sent time and file payload, then send them
                #print("Send Package: ", currentSequenceNo)
                #Create necessary package according to flag conditions
                if isLastPackage == False:
                    sendPayload = str(currentSequenceNo) + '#' + str(len(fileChunk)) + '@' + str(time.time()) + '*' + fileChunk
                else:
                    sendPayload = '!' + str(currentSequenceNo) + '#' + str(len(fileChunk)) + '@' + str(time.time()) + '*' + fileChunk
                #Calculate checksum
                checksum = hashlib.sha1(sendPayload.encode()).hexdigest()
                sendChunk = checksum + sendPayload
                #Send the message
                clientSocket.sendto (sendChunk.encode(), (server_IP_Address, server_UDP_Port))
                
                #Check fo package loss or delay
                try:
                    clientSocket.settimeout(timeoutValue)
                    ack_nack_byte_chunk, serverAddress = clientSocket.recvfrom(2048)
                    clientSocket.settimeout(None)
                    try:
                        #Control Checksum
                        ack_nack_chunk = ack_nack_byte_chunk.decode()
                        #Extract Checksum which is first 40 bytes
                        ack_nack_Checksum = ack_nack_chunk[0:40]
                        ack_nack = ack_nack_chunk[40:]
                        calculatedChecksum = hashlib.sha1(ack_nack.encode()).hexdigest()
                        #No corruption
                        if ack_nack_Checksum == calculatedChecksum:
                            #The correct package is recieved
                            if int(ack_nack) == (currentSequenceNo):
                                #print("OK!!!: ", currentSequenceNo, " ACKed!!!")
                                #Decrease timeout value
                                if minTimeout < (timeoutValue - 0.1*timeoutValue):
                                    timeoutValue -= 0.1*timeoutValue
                                    #print("New Low Timeout", timeoutValue)
                                
                                #Increase seq no
                                currentSequenceNo += 1
                                
                                #Continue sending file
                                if isLastPackage == False:
                                    break
                                #ACKed End Character and Exit
                                if sendingEndCharacter == True:
                                    #print("ACKed Exit!!!")
                                    break
                                #Last file package sent
                                else:
                                    sendingEndCharacter = True
                                    continue                                    
                            #The incorrect package is recieved
                            else:
                                #print("Seq No Fault ",currentSequenceNo, " ", ack_nack)
                                noRetransfer += 1
                                continue
                        #Corrupted package
                        else:
                            #print("Corrupt")
                            noRetransfer += 1
                            continue
                    #Package is undecodable
                    except UnicodeDecodeError:
                        #print("Decode")
                        noRetransfer += 1
                        continue
                #Package delay or loss
                except timeout:
                    #print("Timeout")
                    #Waiting for an ack message for end character
                    if sendingEndCharacter == True:
                        endConnectionCounter -= 1
                        if endConnectionCounter == 0:
                            #print("Timeout Exit!!!")
                            break
                    #Increase timeout value
                    if maxTimeout > (timeoutValue + 0.2*timeoutValue):
                        timeoutValue += 0.2*timeoutValue
                        #print("New High Timeout", timeoutValue)
                    noRetransfer += 1
                    continue
            
            #Last Package is sent and exit
            if isLastPackage == True and sendingEndCharacter == True:
                print("UDP Transmission Re-transferred Packets: ", noRetransfer)
                break
        #Close connection
        clientSocket.close()

def main():
    #Server IP Address, Server UDP and TCP listen ports, Client UDP and TCP send ports are given as argv
    server_IP_Address = sys.argv[1]
    server_UDP_Port = int(sys.argv[2])
    server_TCP_Port = int(sys.argv[3])
    client_UDP_Port = int(sys.argv[4])
    client_TCP_Port = int(sys.argv[5])
    #TCP and UDP clients
    tcpClient(server_IP_Address, server_TCP_Port, client_TCP_Port)
    udpClient(server_IP_Address, server_UDP_Port, client_UDP_Port)


if __name__ == "__main__":
    main()