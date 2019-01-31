//
//  main.cpp
//  3500hw4
//
//  Created by Irwan Winarto on 3/9/18.
//  Copyright © 2018 Irwan Winarto. All rights reserved.
//

#include <string.h>
#include <strings.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>        // for the structures
#include <time.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "pthread.h"
#include <string>
#include <iomanip>

using namespace std;

// FUNCTIONS======================================================================================================
int* number_generator();
void* number_game(void* client_fd);

int main(int argc, const char * argv[]) {
    const int port_num = atoi(argv[1]);
	cout <<"PORTNUM: " << port_num << endl;
	
    int server_fd, client_fd;
    unsigned int struc_len;
    int sent;
    struct sockaddr_in server, client;
    
	
    // verify passed arguments. Only supposed to receive the port number
    /*if(argc != 1)
    {
        perror("Wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    cout << "Number of arguments: " << argc << endl;*/
	
	
	
    int count = 0;
    while (argv[count])
    {
        cout << "arg: " << argv[count] << endl;;
        count++;
    }
    
    // ensure connection to socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
       {
           perror("socket failed\n");
           exit(EXIT_FAILURE);
       }
	cout << "Connected to socket\n";
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
    server.sin_addr.s_addr = INADDR_ANY;       // bind to all available interface on local machine
    //bzero(&server, 8);                         // zero out remaining.. the remaining is 8bytes long??
    
    struc_len = sizeof(sockaddr_in);
    
	
	cout << "portnum: " << server.sin_port << endl;
	cout << "portnum: " << htons(port_num) << endl;

    // forcefully attaching socket to port
    if((::bind(server_fd, (struct sockaddr*) &server, struc_len)) == -1)
    {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
    
    // after bound, we listen. Allow num of pending connections
    int max_pending_connections = 3;
    if(listen(server_fd, max_pending_connections) == -1)
    {
        perror("listenn\n");
        exit(EXIT_FAILURE);
    }
    
    while(1)
    {
		cout << "Inside while loop\n";
        // It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
        if((client_fd = accept(server_fd, (struct sockaddr*) &server, (socklen_t*) &struc_len)) == -1)
        {
            perror("Accept error\n");
            exit(EXIT_FAILURE);
        }
        cout << "New client connected from port no." << ntohs(client.sin_port) << " and IP " << inet_ntoa(client.sin_addr) << endl;
        
   
		
		// Create thread to process client
		int max_players = 2;
		pthread_t pid[max_players];
		int* player_score;
		
		// create threads MAXIMUM NUMBER OF THREADS??
		int create_check = pthread_create(&pid[0], NULL, number_game, (void*) &client_fd);
		if(create_check != 0)
		{
			cout << "Error creating thread\n";
			exit(EXIT_FAILURE);
		}
		cout << "A thread has been created\n";
		
		
		
		
        // RECEIVE MESSAGEEEEEE
		char name[1024] = {0};
		char* hello = "Hello from server";
		//cout << "Sizeofname: " << sizeof(name)/sizeof(name[0]) << endl;
		int valread = read(client_fd, name, sizeof(name));
		cout << "Name: " << name << endl;
		
		send(client_fd, hello, strlen(hello), 0);
		cout << "Hello message sent\n";
		
		//delete hello;
		
		
		
		
		
		
		
    }
    
    return 0;
}

// FUNCTION DEFINITIONS===========================================================================================
int* number_generator()
{
    srand((int)time(NULL));
    const int num_of_nums = 3;
    int* num = new int[num_of_nums];
    int mask = 0;
    for(int i = 0; i < num_of_nums; i++)
    {
        mask = rand() % 200 + 1;
        num[i] = mask;
        cout << "Here: " << num[i] << endl;
    }
    //delete[] num;
    return num;
}



// this
void* number_game(void* client_fd)
{
	const int min_guess = 1;
	const int max_guess = 200;
	const int mystery_numbers = 3;
	int guesses[mystery_numbers] = {0};
	int answers[mystery_numbers];
    int *generated_num = number_generator();
	int equal = 0;
	int too_high = 0;
	int too_low = 0;
	
	// get the answers
    cout << "THISSS\n";
    for(int i = 0; i < mystery_numbers; i++)
    {
        answers[i] = generated_num[i];
        cout << generated_num[i] << endl;
    }
	
	
	int turn = 1;
	bool playing = true;
	while(playing)
	{
		// ask user for guesses
		cout << "Turn: " << turn << endl;
		
		// Read the guesses
		int client_answer[mystery_numbers];
		int count = 0;
		char guess[1024] = {0};
		int valread = read(*(int*)client_fd, guess, sizeof(guess));
		string guess_str = guess;
		
		char* cstr = new char[guess_str.length() + 1];
		strcpy(cstr, guess_str.c_str());
		char* p = strtok(cstr, " ");
		while(p !=0)
		{
			cout << p << endl;
			client_answer[count] = atoi(p);
			count++;
			p = strtok(NULL, " ");
		}
		
		// check guess against answer
		for(int i = 0; i < mystery_numbers; i++)
		{
			cout << client_answer[i] << ", \n";
			if(client_answer[i] == answers[i])
			{
				equal++;
			}
			else if(client_answer[i] < answers[i])
			{
				too_low++;
			}
			else if(client_answer[i] > answers[i])
			{
				too_high++;
			}
		}
	
		string stat = "Too high: " + to_string(too_high) + "     Too low: " + to_string(too_low) + "     Equal: " + to_string(equal);
		if(equal != mystery_numbers)
		{
			turn++;
			// reset counter
			equal = 0;
			too_high = 0;
			too_low = 0;
			
			char*send_stat = &stat[0];
			send(*(int*)client_fd, send_stat, strlen(send_stat), 0);
		}
		else 
		{
			playing = !playing;
			
			//send victory message
			string victory_mesg = "VEECTOREE";
			// send the guess to server
			char*sendpls = &victory_mesg[0];
			send(*(int*)client_fd, sendpls, strlen(sendpls), 0);
			
			// update leaderboard
			
			
			
			
			
			pthread_exit(NULL);
		}
		
		
	}
	
	// print out victory message
	cout << "VEECTOREE\n\n";
}
