//
//  client.cpp
//  3500hw4
//
//  Created by Irwan Winarto on 3/9/18.
//  Copyright © 2018 Irwan Winarto. All rights reserved.
//

#include <string.h>
#include <string>
//#include "client.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>        // for the structures
#include <arpa/inet.h>
#include <sys/uio.h>
#include <unistd.h>
#include <vector>

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA 1024

using namespace std;

// 1. connect to socket
// 2. connect to server
// 3. send/recv
void number_game();

int main(int argc, const char * argv[])
{
    int client_fd = 0;
    struct sockaddr_in server_addr;
	const int PORT = atoi(argv[2]);
	const char* ip_addr = argv[1];

    // connect to socket
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Client socket error\n");
        exit(-1);
    }
	cout << "Connected to socket. Client fd: " << client_fd << endl;
	
    
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
	cout << "PORT:" << PORT << endl;
	cout << "ip_addr: " << ip_addr << endl;
	cout << "PORT:" << server_addr.sin_port << endl;
	//cout << "ip_addr: " << ip_addr << endl;
	
	
	
    // convert ipv4 and ipv6 addesses from text to binary form????<--------???
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) == -1)
    {
        perror("Invalid address. Address not supported\n");
        exit(-1);
    }
	cout << "IP converted\n";
    
    // connect to server
	//cout << "Server addr: " << server_addr.sin_addr << endl;
	cout << "Port: " << PORT << endl;
    if(connect(client_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("Failed to connect to server\n");
        exit(-1);
    }
    cout << "Connected to server\n";
	
	
	
	
	// get name
	string name;
	cout << "Please input name: ";
	getline(cin, name);
   
   // SEND SOMETHING
   char* hello = &name[0];
   const uint len = name.length();
   char buffer[len];// = {0};		/// CHANGE TO VARIABLE NAME LENGTH
   for(int i = 0; i < len; i++)
	   buffer[i] = 0;
   
   send(client_fd, hello, strlen(hello), 0);
   cout << "Hello message sent\n";
   cout << "length: " << strlen(hello);
   
   
   
   
   
   
   
   
   // Ask user for guess and verify
    const int min_guess = 1;
	const int max_guess = 200;
	const int mystery_numbers = 3;
	int guesses[mystery_numbers] = {0};
	string guess;
	
	int equal = 0;
	int too_high = 0;
	int too_low = 0;
	
	int turn = 1;
	bool playing = true;
	while(playing)
	{
		// ask user for guesses
		cout << "Turn: " << turn << endl;
		cout << "Enter three guesses.\n";
		getline(cin, guess);
		char* cstr = new char[guess.length() + 1];
		strcpy(cstr, guess.c_str());
		char* p = strtok(cstr, " ");
		
		char* send_this_guess[mystery_numbers];
		int i=0;
		while(p !=0)
		{
			cout << p << endl;
			if(atoi(p) < min_guess || atoi(p) > max_guess)
			{
				cout << "Error. Guess must be between " << min_guess << " and " << max_guess << endl;
				break;
			}
			else
			{
				send_this_guess[i] = p;
				i++;
				p = strtok(NULL, " ");
			}
		}
		
		// send the guess to server
		char*sendpls = &guess[0];
		send(client_fd, sendpls, strlen(sendpls), 0);
		
		// read server response
		int valread = read(client_fd, buffer, 1024);
		cout << "buffer: " << buffer << endl;
		
		//check response
		if(buffer == "VEECTOREE")
		{
			cout << "veectorree\n";
			playing = !playing;
			
			
		}
		else
		{
			turn++;
			
			// read again
			
		}
		delete[] cstr;
	}
		
		
		
		
	
	// print out victory message
	cout << "VEECTOREE\n\n";
	
	
	
	
	
	
	
   
   // read victory message
   int valread = read(client_fd, buffer, 1024);
   cout << "buffer: " << buffer << endl;
   
   
    return 0;
}

/*void number_game()
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
		cout << "Enter three guesses.\n";
		for(int i = 0; i < mystery_numbers; i++)
		{
			cin >> guesses[i];
			if(guesses[i] < min_guess || guesses[i] > max_guess)
			{
				cout << "Error. Guess must be between " << min_guess << " and " << max_guess << endl;
				return 0;
			}
		
			// check guess against answer
			if(guesses[i] == answers[i])
			{
				equal++;
			}
			else if(guesses[i] < answers[i])
			{
				too_low++;
			}
			else if(guesses[i] > answers[i])
			{
				too_high++;
			}
		}
	
		cout << "Too high: " << too_high << "     Too low: " << too_low << "     Equal: " << equal << endl;
		if(equal != mystery_numbers)
			turn++;
		else 
			playing = !playing;
	}
	
	// print out victory message
	cout << "VEECTOREE\n\n";
}*/
