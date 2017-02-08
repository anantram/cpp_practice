#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include <cstring>

#include <io.h>
#include <winsock2.h>
#include <thread>
#include <fstream>
#include <vector>
#include<mutex>

#define receive_flag 2000 

using namespace std;
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    string temp_input;
    int recv_size;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
    char input[10];

    // Defining the values e.g server address and port
     server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888);
    
 
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }
     
    puts("Connected");
    
    recv_size = recv(s , server_reply , 2000 , 0);
    puts("Reply received\n");
    puts(server_reply);
    server_reply[0] = '\0';
    cin >> server_reply;
    send(s , server_reply , strlen(server_reply), 0);
    cin >> server_reply;
    send(s , server_reply , strlen(server_reply), 0);
    recv_size = 0;
	for(;recv_size == 0;) {
		recv_size = recv(s , server_reply , 2000 , 0);
		if(recv_size >0) {
			server_reply[recv_size] = '\0';
			cout << "Good Bye message: "<< server_reply << " : " << recv_size << endl;
			break;
		}
		recv_size = 0;
	}
	close(s);
    
}
