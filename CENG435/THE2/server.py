#!/usr/bin/python

from socket import *
import sys, errno, time, hashlib

#Create ACK message (isAck == True) or NACK message (isAck == False)
def ackCreator(packageNo, isAck):
    if(isAck):
        ackMessage = str(packageNo)
        checksum = hashlib.sha1(ackMessage.encode()).hexdigest()
        messageToSend = checksum + ackMessage
        return messageToSend
    else:
        nackMessage = str(packageNo-1)
        checksum = hashlib.sha1(nackMessage.encode()).hexdigest()
        messageToSend = checksum + nackMessage
        return messageToSend

#Returns a dictionary (message extracted to it's components)
def extractUDP_Message(message):
    extractedMessage = {}
    if message[0] == '!':
        extractedMessage['isLastMessage'] = True
        startIndex = 1
    else:
        extractedMessage['isLastMessage'] = False
        startIndex = 0

    #Symbolize the end of 'sequence number'
    positionOfHash = message.find("#")
    #Symbolize the end of 'size of payload'
    positionOfAt = message.find("@")
    #Symbolize the end of 'send time'
    positionOfStar = message.find("*")
    #Start of the sent file payload
    positionOfPayloadBegin = positionOfStar + 1
    #Payload size
    sizeOfPayload = int(message[positionOfHash + 1:positionOfAt])
    #End of the sent file payload
    positionOfPayloadEnd = positionOfPayloadBegin + sizeOfPayload
    #End of the sent file payload
    positionOfPayloadEnd = positionOfPayloadBegin + sizeOfPayload

    #Extract sequence no
    extractedMessage['sequenceNo'] = int(message[startIndex:positionOfHash])
    #Extract send time
    extractedMessage['sendTime'] = float(message[(positionOfAt + 1):positionOfStar])
    #Extract payload
    extractedMessage['payload'] = message[positionOfPayloadBegin:positionOfPayloadEnd]

    return extractedMessage

#Returns a dictionary (message extracted to it's components)
def extractTCP_Message(message):
    extractedMessage = {}
    if message[0] == '!':
        extractedMessage['isLastMessage'] = True
        startIndex = 1
    else:
        extractedMessage['isLastMessage'] = False
        startIndex = 0

    #Symbolize the end of 'size of payload'
    positionOfAt = message.find("@")
    #Symbolize the end of 'send time'
    positionOfStar = message.find("*")
    #Start of the sent file payload
    positionOfPayloadBegin = positionOfStar + 1
    #Payload size
    sizeOfPayload = int(message[startIndex:positionOfAt])
    #End of the sent file payload
    positionOfPayloadEnd = positionOfPayloadBegin + sizeOfPayload
        
    #Extract send time
    extractedMessage['sendTime'] = float(message[(positionOfAt + 1):positionOfStar])
    #Extract payload
    extractedMessage['payload'] = message[positionOfPayloadBegin:positionOfPayloadEnd]
    #The data of the upcoming sent are replaced to the chunk
    extractedMessage['unusedMessage'] = message[positionOfPayloadEnd:]
    
    return extractedMessage

#TCP Server Function
def tcpServer(server_TCP_Listen_Port):
    #Number of bytes to be read from the socket
    predefinedSize = 1024
    #Both of them used for finding the average value
    beginTime = 0
    endTime = 0

    #Chunk, message, payload(file) start as an empty string
    chunk = ""
    message = ""
    payload = ""

    #Array of chunks, send times and recieve timea
    chunkArray = []
    sendTimesArray = []
    receiveTimesArray = []

    #Necessary connections for TCP Server are created
    serverSocket = socket(AF_INET,SOCK_STREAM)
    serverSocket.bind(('',server_TCP_Listen_Port))
    serverSocket.listen(1)

    #Accept connection request
    connectionSocket, addr = serverSocket.accept()

    try:
        #Loop to recieve from TCP Socket
        while True:
            
            #Read every byte available from socket
            chunk = connectionSocket.recv(predefinedSize).decode()

            if chunk:
                #Record recieve time
                readTime = time.time()
            else:
                #Exit if no data is available
                break
            
            #Append the read chunk to the array
            chunkArray.append(chunk)
            #Append the receive time
            receiveTimesArray.append(readTime)

            #Send message back to the client
            answerMessage = "&"
            connectionSocket.send(answerMessage.encode())


            if chunk[0] == '!':
                #Exit because last package is recieved
                break
            
        for i in range(0, len(chunkArray)):
            #Extract all chunk messages and needed operations
            recievedMessage = chunkArray[i]
            extractedMessage = extractTCP_Message(recievedMessage)

            sendTimesArray.append(extractedMessage['sendTime'])
            payload += extractedMessage['payload']
            message = extractedMessage['unusedMessage']

            if i == 0:
                beginTime = extractedMessage['sendTime']
            
            if i == (len(chunkArray) - 1):
                endTime = receiveTimesArray[i]       

        #All file is recieved and the after desired prints TCP Server connection is closed
        timeDifferences = sum(receiveTimesArray) - sum(sendTimesArray)
        recievedPacketsNo = len(receiveTimesArray)

        print("TCP Packets Average Transmission Time: ", (timeDifferences/recievedPacketsNo) * 1000, "ms")
        print("TCP Communication Total Transmission Time: ",(endTime-beginTime) * 1000, "ms")

        #Close both connection and server socket
        connectionSocket.close()
        serverSocket.close()

        with open('./new_transfer_file_TCP.txt', 'w') as fileWrite:
            fileWrite.write(payload)

    #Caution for broken pipe exception
    except IOError as e:
        if e.errno == errno.EPIPE:
            
            print("TCP Packets Average Transmission Time: ",(timeDifferences)/recievedPacketsNo, "ms")
            print("TCP Communication Total Transmission Time: ",(endTime-beginTime) * 1000, "ms")
            
            #Close both connection and server socket
            connectionSocket.close()
            serverSocket.close()

#UDP Server Function
def udpServer(server_UDP_Listen_Port):
    #Arrays of needed data
    sendTimes = []
    recieveTimes = []

    #Chunk, payload(file) start as an empty string
    chunk = ""
    payload = ""

    #Start from first seq no for synchronization
    currentSequenceNo = 0
    #Required for end state of server
    waitingEndMessage = False

    #Necessary connections for TCP Server are created
    serverSocket = socket(AF_INET, SOCK_DGRAM)
    serverSocket.bind(('', server_UDP_Listen_Port))

    try:
        while True:
            #Recieve message and store time
            byteChunk, clientAddress = serverSocket.recvfrom(2048)
            recieveTime = time.time()
            #Try for package decode error
            try:
                #Control Checksum
                chunk = byteChunk.decode()
                #Extract Checksum which is first 40 bytes
                recievedChecksum = chunk[0:40]
                message = chunk[40:]
                calculatedChecksum = hashlib.sha1(message.encode()).hexdigest()

                #No corruption
                if recievedChecksum == calculatedChecksum:
                    extractedMessage = extractUDP_Message(message)
                    #The correct package is recieved
                    if extractedMessage['sequenceNo'] == (currentSequenceNo):
                        #Recieving file packages
                        if waitingEndMessage == False:
                            #print("OK!!!: ", currentSequenceNo, " Recieved!!!")
                            #Extract message, save necessary data and payload
                            payload += extractedMessage['payload']
                            sendTimes.append(extractedMessage['sendTime'])
                            recieveTimes.append(recieveTime)
                            #Send ACK
                            ackMessage = ackCreator(currentSequenceNo, True)
                            serverSocket.sendto(ackMessage.encode(), clientAddress)
                            #Increace seq no
                            currentSequenceNo += 1

                            if extractedMessage['isLastMessage'] == True:
                                #Obtained last file packakge
                                waitingEndMessage = True

                        #Recieving end character
                        else:
                            ackMessage = ackCreator(currentSequenceNo, True)
                            serverSocket.sendto(ackMessage.encode(), clientAddress)
                            break

                    #The incorrect package is recieved
                    else:
                        #print("Seq No Fault ", currentSequenceNo," " ,extractedMessage['sequenceNo'])
                        nackMessage = ackCreator(currentSequenceNo, False)
                        serverSocket.sendto(nackMessage.encode(), clientAddress)
                #Corrupted package
                else:
                    #print("Corrupt")
                    nackMessage = ackCreator(currentSequenceNo, False)
                    serverSocket.sendto(nackMessage.encode(), clientAddress)

            #Package is undecodable
            except UnicodeDecodeError:
                #print("Decode")
                nackMessage = ackCreator(currentSequenceNo, False)
                serverSocket.sendto(nackMessage.encode(), clientAddress)
            #print("Expect Sequence No: ",currentSequenceNo)
        
        #Print wanted informations then close server
        print("UDP Packets Average Transmission Time: ", (sum(recieveTimes) - sum(sendTimes))*1000/len(recieveTimes) , "ms")
        print("UDP Communication Total Transmission Time: ",(recieveTimes[-1] - sendTimes[0])* 1000, "ms")
        serverSocket.close()

        #Write to file
        with open('./new_transfer_file_UDP.txt', 'w') as fileWrite:
            fileWrite.write(payload)

    #Caution for broken pipe exception
    except IOError as e:
        if e.errno == errno.EPIPE:
            #Print wanted informations then close server
            print("UDP Packets Average Transmission Time: ", (sum(recieveTimes) - sum(sendTimes))*1000/len(recieveTimes) , "ms")
            print("UDP Communication Total Transmission Time: ",(recieveTimes[-1] - sendTimes[0])* 1000, "ms")
            serverSocket.close()

def main():
    #Server UDP and TCP listen ports are given as argv
    server_UDP_Listen_Port = int(sys.argv[1])
    server_TCP_Listen_Port = int(sys.argv[2])
    #TCP and UDP servers
    tcpServer(server_TCP_Listen_Port)
    udpServer(server_UDP_Listen_Port)
    

if __name__ == "__main__":
    main()