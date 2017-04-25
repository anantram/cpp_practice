// Chatclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include <cstring>
#include <string>

#define receive_flag 2000


using namespace std;

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char server_reply[2000];
	string temp_input;
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");
	char input[10];

	// Defining the values e.g server address and port
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);


	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	// sending a connection message

	send(s, "helloserver", 12, 0);
	printf("new connection hello server message send \n");

	// Reciveing the acknoloegment

	recv_size = recv(s, server_reply, 2000, 0);
	puts("Reply received\n");
	server_reply[recv_size] = '\0';
	puts(server_reply);

	// starting the conversation

	printf("Enter 1 for new user else enter you userid\n");
	scanf_s("%s", &input,1024);
	send(s, input, strlen(input), 0);

	if (input[0] == '1') {
		cout << "Enter new User ID: ";
		scanf_s("%s", &input,1024);
		send(s, input, strlen(input), 0);
		cout << "Enter new User Password: ";
		scanf_s("%s", &input,1024);
		send(s, input, strlen(input), 0);
		cout << "New user created : ";
	}
	else {
		cout << "Enter Password\n";
		scanf_s("%s", &input,1024);
		send(s, input, strlen(input), 0);
	}

	server_reply[0] = '\0';
	for (; recv_size == 0;) {
		recv_size = recv(s, server_reply, receive_flag, 0);
	}
	server_reply[recv_size] = '\0';
	cout << server_reply << '\n';

	if (strcmp(server_reply, "Invalid login") == 0) {
		return 0;
	}

	do {
		cout << "1: New message\n2: check Messages\n3: Exit\n";
		scanf_s("%s", &input,1024);
		send(s, input, strlen(input), 0);

		if (input[0] == '1') {
			cout << "Enter Receiver ID: ";
			scanf_s("%s", &input,1024);
			send(s, input, strlen(input), 0);
			cout << '\n';

			cout << "Enter Message: ";
			cin.get();
			getline(cin, temp_input);
			//scanf_s("%s",&input,1024);
			send(s, temp_input.c_str(), strlen(temp_input.c_str()), 0);
			cout << '\n';
		}
		else if (input[0] == '2') {
			server_reply[0] = '\0';
			for (;;) {
				server_reply[0] = '\0';
				recv_size = 0;
				for (; recv_size == 0;) {
					recv_size = recv(s, server_reply, receive_flag, 0);
				}
				server_reply[recv_size] = '\0';
				if (strcmp(server_reply, "end") == 0) {
					break;
				}
				cout << server_reply << '\n';
			}
		}
		else if (input[0] == '3')
			return 0;
	} while (input[0] != 'e');

	return 0;
}
