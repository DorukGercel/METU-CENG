#include<unistd.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "logging.h"

#ifdef SYSVR4
#define PIPE(a) pipe(a)
#else
#include<sys/socket.h>
#define PIPE(a) socketpair(AF_UNIX, SOCK_STREAM,PF_UNIX,(a))
#endif

int main() {

    int starting_bid, min_increment, no_bidders, times_2_no_bidders, no_args,  no_bidding_bidders, current_bid = 0, current_bid_owner = -1, timeout = -1;
    char* buffer_input = (char*)malloc(255*sizeof(char));   //NOT USED
    char* buffer_output = (char*)malloc(255*sizeof(char));   //NOT USED

    //Take the first three integer argument
    scanf("%d %d %d", &starting_bid, &min_increment, &no_bidders);
    no_bidding_bidders = no_bidders;

    //PID Holder, File name holder for the executable files and Arg List
    int pid[no_bidders];
    int bidder_status[no_bidders];
    char file_names[no_bidders][255];
    char **arg_list[no_bidders];
    
    //The file name are saved in outer loop and the arg list are saved in inner loop
    for(int i = 0; i < no_bidders; i++){
        scanf("%s", file_names[i]);
        scanf("%d", &no_args);
        arg_list[i] = (char**)malloc((no_args + 2)*sizeof(char*));
        for (int j = 0; j < no_args + 2; j++)
        {
            arg_list[i][j] = (char*)malloc(255*sizeof(char));
            if(j == 0){
                arg_list[i][j] = file_names[i];
            }
            else if(j == no_args + 1){
                arg_list[i][j] = NULL;
            }
            else{
                scanf("%s", arg_list[i][j]);
            }
        }
    }
    /*
    for(int i = 0; i < no_bidders; i++){
        printf("File name: %s\n", file_names[i]);
        for(int j = 0; j < 2; j++){
            printf("Args: %s\n", arg_list[i][j]);
        }
    }
    */

    //Save the terminal
    //terminal = dup(1);

    //Create the necessary file descriptors for the pipes
    times_2_no_bidders = no_bidders * 2;
	int fd[times_2_no_bidders];

    //Open necessary pipes.
    for(int i = 0; i < no_bidders; i++){
        PIPE(fd + 2*i);
    }
    
    //Declare the struct for the poll()
    struct pollfd pfds[no_bidders];
    
    //Creating children attached to one parrent
    for(int i = 0; i < no_bidders; i++){
	    if (pid[i] = fork()) {
            //Change the poll struct for the active fds
            pfds[i].fd = fd[i*2];
            pfds[i].events = POLLIN;
            //Close the odd numbered file desciptors
	    	close(fd[1 + i*2]);
	    } 
        else {
            for(int j = 0; j < times_2_no_bidders; j++){
                if(j == (i*2 + 1)){
                    //Attach the input and output to the pipe
                    dup2(fd[j], 0); 
                    dup2(fd[j], 1);
                    close(fd[j]);
                }
                else
                {
                    //Close the unnecessary file descriptors
                    close(fd[j]);
                }
            }

            execv(file_names[i], arg_list[i]);
            break;
	    }
    }

    while(1){
        poll(pfds, no_bidders, timeout);
        for(int i = 0; i < no_bidders; i++){
            if(pfds[i].revents == POLLIN){
                //Create the client message and server message pointers
                cm* message_input = (cm*) malloc(sizeof(cm));
                sm* message_output = (sm*)malloc(sizeof(sm));
                //Create the input info and output info pointers
                ii* info_input = (ii*)malloc(sizeof(ii));
                oi* info_output = (oi*)malloc(sizeof(oi));
                //Read the message to the input buffer
                read(fd[i*2], message_input, sizeof(cm));
                //Print Input
                info_input->type = message_input->message_id;
                info_input->pid = pid[i];
                info_input->info = message_input->params;
                print_input(info_input, i);
                //Client Connect case
                if(message_input->message_id == CLIENT_CONNECT){
                    //Set the timeout
                    if(timeout < 0){
                        timeout = message_input->params.delay;
                    }
                    else{
                        if(message_input->params.delay < timeout){
                            timeout = message_input->params.delay;
                        }
                    }
                    //Form the message to send the client
                    message_output->message_id = SERVER_CONNECTION_ESTABLISHED;
                    message_output->params.start_info.client_id = i;
                    message_output->params.start_info.starting_bid = starting_bid;
                    message_output->params.start_info.current_bid = current_bid;
                    message_output->params.start_info.minimum_increment = min_increment;
                    
                    //Write to buffer
                    write(fd[i*2], message_output, sizeof(sm));
        
                    //Print output
                    info_output->type = message_output->message_id;
                    info_output->pid = pid[i];
                    info_output->info = message_output->params;
                    print_output(info_output, i);

                }
                //Client Bid case
                else if(message_input->message_id == CLIENT_BID){
                    //Form the message to send the client
                    message_output->message_id = SERVER_BID_RESULT;
                    if(message_input->params.bid < starting_bid){
                        message_output->params.result_info.result = BID_LOWER_THAN_STARTING_BID;
                    }
                    else if(message_input->params.bid < current_bid){
                        message_output->params.result_info.result = BID_LOWER_THAN_CURRENT;
                    }
                    else if((message_input->params.bid - current_bid) < min_increment){
                        message_output->params.result_info.result = BID_INCREMENT_LOWER_THAN_MINIMUM;
                    }
                    else{
                        message_output->params.result_info.result = BID_ACCEPTED;
                        if(message_input->params.bid > current_bid){
                            current_bid = message_input->params.bid;
                            current_bid_owner = i;
                        }
                    }
                    message_output->params.result_info.current_bid = current_bid;

                    //Write to buffer
                    write(fd[i*2], message_output, sizeof(sm));

                    //Print output
                    info_output->type = message_output->message_id;
                    info_output->pid = pid[i];
                    info_output->info = message_output->params;
                    print_output(info_output, i);
                }
                //Client Finished case
                else if(message_input->message_id == CLIENT_FINISHED){
                    bidder_status[i] = message_input->params.status;
                    no_bidding_bidders-=1;
                    if(!no_bidding_bidders){
                        //Form the message to send the client
                        message_output->message_id = SERVER_AUCTION_FINISHED;
                        message_output->params.winner_info.winner_id = current_bid_owner;
                        message_output->params.winner_info.winning_bid = current_bid;

                        print_server_finished(current_bid_owner, current_bid);

                        for(int j = 0; j < no_bidders; j++){
                            //Write to buffer
                            write(fd[j*2], message_output, sizeof(sm));

                            //Print output
                            info_output->type = message_output->message_id;
                            info_output->pid = pid[j];
                            info_output->info = message_output->params;
                            print_output(info_output, j);
                        }
                        for(int j = 0; j < no_bidders; j++){
                            //Reap operation
                            int child_status;
                            waitpid(pid[j], &child_status, 0);
                            print_client_finished(j, child_status, (child_status == bidder_status[j]));
                            close(fd[j*2]);
                        }
                        break;
                    }
                }
                free(message_input);
                free(message_output);
                free(info_input);
                free(info_output);
            }

        }
        if(!no_bidding_bidders){
            break;
        }
    }
    
	return 0;
}
