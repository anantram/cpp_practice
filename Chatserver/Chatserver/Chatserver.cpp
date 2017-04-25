// Chatserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <cstring>
#include<string>
#include <vector>
#include <Ws2tcpip.h>
#include <stdio.h>

#define NAME_SIZE 20
#define PASS_SIZE 20
#define DATA_SIZE 1000
#define PORT_NUMBER 8888 
#define RECEIVE_FLAG 2000 

#pragma comment(lib,"ws2_32.lib")

using namespace std;


class User {

public:
	char user_id[NAME_SIZE];
	char user_pass[PASS_SIZE];

	User() {
	}

	friend ostream& operator<<(ostream& os, const User& s) {
		os << s.user_id << '\n';
		os << s.user_pass << '\n';
		return os;
	}

	friend std::istream& operator >> (std::istream& is, User& s) {
		is >> s.user_id >> s.user_pass;
		return is;
	}
};

class Message {

public:
	char sender_id[NAME_SIZE];
	char receiver_id[NAME_SIZE];
	char msg[DATA_SIZE];

	Message() {

	}

	friend ostream& operator<<(ostream& os, const Message& s) {
		os << s.sender_id << '\n';
		os << s.receiver_id << '\n';
		os << s.msg << '\n';
		return os;
	}

	friend std::istream& operator >> (std::istream& is, Message& s) {
		string temp;
		is >> s.sender_id;
		is >> s.receiver_id;
		is.get();
		getline(is, temp);
		strcpy_s(s.msg, temp.c_str());
		return is;
	}
};


class Interaction {

public:
	Interaction() {

	}

	void interact(string currentUser, SOCKET new_socket) {
		Message new_message;
		fstream messages;
		char send_message[DATA_SIZE];
		int recv_size;
		char server_reply[DATA_SIZE];

		// Starting the Interaction with the client

		for (;;) {
			recv_size = 0;
			for (; recv_size == 0;) {														//waiting for the user first input
				recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
			}
			if (recv_size > 0) {
				server_reply[recv_size] = '\0';
				cout << server_reply[0] << "\n";

				switch (server_reply[0]) {
					// Send Message
				case '1':
					messages.open("messages.txt", ios::in | ios::out | ios::app);
					recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0); 		// Getting the reciever id
					server_reply[recv_size] = '\0';
					strcpy_s(new_message.receiver_id, server_reply);
					send_message[0] = '\0';
					recv_size = 0;
					for (; recv_size == 0;) {

						recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
						server_reply[recv_size] = '\0';
						if (recv_size > 0) {
							strcpy_s(new_message.msg, server_reply);
							strcpy_s(new_message.sender_id, currentUser.c_str());
							messages << new_message;
							messages.close();
						}
					}
					break;
					// Check Messages			
				case '2':
					ifstream ifs("messages.txt");
					while (ifs.good()) {
						ifs >> new_message;
						if (strcmp(new_message.receiver_id, currentUser.c_str()) == 0) {
							send_message[0] = '\0';
							strcpy_s(send_message, new_message.sender_id);
							cout << strlen(send_message) << " " << send_message << "\n";
							send(new_socket, send_message, strlen(send_message), 0);
							send_message[0] = '\0';
							strcpy_s(send_message, new_message.msg);
							cout << strlen(send_message) << " " << send_message << "\n";
							send(new_socket, send_message, strlen(send_message), 0);
						}
					}
					send_message[0] = '\0';
					strcpy_s(send_message, "end");
					send(new_socket, send_message, strlen(send_message), 0);

					ifs.close();
					break;
				}
			}
		}
	}

	User verify(SOCKET new_socket) {
		fstream ofs("user.txt", ios::in | ios::out | ios::app);
		User newuser, nulluser;
		char send_message[100];
		int recv_size = 0;
		char server_reply[DATA_SIZE];
		string logged_user, logged_pass;

		recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
		if (recv_size > 0) {
			server_reply[recv_size] = '\0';
			if (server_reply[0] == '1') {

				//					strcpy_s(send_message,"user_name: \n");
				//		        	send(new_socket , send_message , strlen(send_message) , 0);
				recv_size = 0;
				for (; recv_size == 0;) {

					recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
					server_reply[recv_size] = '\0';
					if (recv_size > 0) {

						strcpy_s(newuser.user_id, server_reply);
						logged_user = string(server_reply);
						break;
					}
				}

				//					strcpy_s(send_message,"user_pass: ");
				//					send(new_socket , send_message , strlen(send_message) , 0);
				recv_size = 0;
				for (; recv_size == 0;) {

					recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
					server_reply[recv_size] = '\0';
					if (recv_size > 0) {

						strcpy_s(newuser.user_pass, server_reply);
						ofs << newuser;
						ofs.close();
						break;
					}
				}
				//strcpy_s(send_message,"New user added");
				//send(new_socket , send_message , strlen(send_message) , 0);

				ifstream ifs("user.txt");
				while (ifs.good()) {
					ifs >> newuser;
				}
				ifs.close();
				return newuser;
			}
			else {
				server_reply[recv_size] = '\0';
				logged_user = string(server_reply);

				recv_size = 0;
				for (; recv_size == 0;) {

					recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
					server_reply[recv_size] = '\0';
					if (recv_size > 0) {

						logged_pass = string(server_reply);
						break;
					}
				}

				ifstream ifs("user.txt");
				while (ifs.good()) {
					ifs >> newuser;
					if (logged_user.compare(newuser.user_id) == 0) {
						if (logged_pass.compare(newuser.user_pass) == 0) {
							return newuser;
						}
					}
				}
				strcpy_s(nulluser.user_id, "NULL");
				return nulluser;
			}
		}
	}
};



void newconnection(SOCKET new_socket, char message[]) {
	Interaction new_interaction;
	char server_reply[DATA_SIZE];
	string user;
	User currentUser;
	send(new_socket, message, strlen(message), 0);
	cout << "new connection message send\n";
	currentUser = new_interaction.verify(new_socket);
	user = string(currentUser.user_id);

	if (strcmp(currentUser.user_id, "NULL") == 0) {
		server_reply[0] = '\0';
		strcpy_s(server_reply, "Invalid login");
		send(new_socket, server_reply, strlen(server_reply), 0);
	}
	else {
		server_reply[0] = '\0';
		strcpy_s(server_reply, user.c_str());
		send(new_socket, server_reply, strlen(server_reply), 0);
		new_interaction.interact(user, new_socket);
	}
}

void newthread(SOCKET s, struct sockaddr_in* client, int c, char * message) {
	SOCKET new_socket;
	char server_reply[DATA_SIZE];
	cout << "reached here\n";

	cout << "asd\n";

	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {

		int recv_size = 0;
		recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
		cout << server_reply << " new request \n";
		if (recv_size > 0) {
			newconnection(new_socket, message);
		}
		cout << "waiting\n";
	}

	scanf_s("%s", server_reply,1024);
}

class newserver {
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	//template < class thread, class Alloc = allocator<thread> > class vector;
	vector<thread> mythreads;

public:

	newserver() {

	}

	void Initialize(void) {
		char message[] = "Hello, I have received your connection\nPlease enter Your ID else enter 1 for registering";
		char server_reply[DATA_SIZE];

		printf("\nInitialising Winsock...");
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

			printf("Failed. Error Code : %d", WSAGetLastError());
		}

		cout << thread::hardware_concurrency() << ": Threads are avalible\n";

		cout << "Initialized.\n";
		//Create a socket
		if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
		}

		cout << "Socket created.\n";
		//Prepare the sockaddr_in structure
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(PORT_NUMBER);

		//Bind
		int temp = 0;
		temp = ::bind(s, (struct sockaddr *)&server, sizeof(server));
		if (temp == SOCKET_ERROR) {

			cout << "Bind failed with error code : " << WSAGetLastError() << "\n";
			exit(EXIT_FAILURE);
		}

		cout << "Bind done \n";

		//Listen to incoming connections
		listen(s, 3);

		//Accept and incoming connection
		cout << "Waiting for incoming connections...\n";

		c = sizeof(struct sockaddr_in);

		while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {

			int recv_size = 0;
			recv_size = recv(new_socket, server_reply, RECEIVE_FLAG, 0);
			cout << server_reply << " new request \n";
			if (recv_size > 0) {
				std::thread newThread(newconnection, new_socket, message);
				newThread.detach();
			}
			cout << "waiting\n";
		}



		////         Removing the code block
		//         
		//		//for(int thread_count = 0; thread_count < (thread::hardware_concurrency()-1);thread_count++) {
		//			std::thread funcTest1(newthread,s,&client,c,message);
		//           	//mythreads.push_back(move(funcTest1));
		//           	funcTest1.detach();
		//	//	}
		//
		if (new_socket == INVALID_SOCKET) {

			cout << "accept failed with error code : " << WSAGetLastError() << "\n";
		}

		closesocket(s);
		WSACleanup();
	}
};


int main(int argc, char *argv[])
{
	newserver server;
	server.Initialize();
	return 0;
}

