#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include<mutex>

#define NAME_SIZE 20
#define PASS_SIZE 20
#define DATA_SIZE 1000
#define PORT_NUMBER 8888 
#define RECEIVE_FLAG 2000 

#pragma comment(lib,"ws2_32.lib")

using namespace std;

using namespace std;
mutex mtx;
chrono::seconds sec(1);

void output(int input) {
	string file = "";
	file.append("1");
	file.append(".txt");
	fstream ofs(file, ios::in | ios::out | ios::app);
	cout << input << ": thread a number\n";
	char input_value[20];
	for(int i = 0;i < 100;i++) {
		ofs << (i*input) << " bythread: " << input<< '\n';
		cout << input << ": thread a number\n";
	}
	ofs.close();
}


void connection(SOCKET s,int c, int thread_id){
	struct sockaddr_in client;
	SOCKET client_socket;
	char server_reply[20];
	cout << "server has: " << server_reply << '\n';
	int rec_size = 0;
	while((client_socket = accept(s ,(struct sockaddr *)&client, &c)) != INVALID_SOCKET ) {
		rec_size = send(client_socket , "hello client nhow are you??" , 100, 0);
    	cout << "waiting : " << rec_size << '\n';
    	rec_size = 0;
    	for(;rec_size == 0;) {
    		rec_size = recv(client_socket , server_reply , 2000 , 0);
			if(rec_size >0) {
    			server_reply[rec_size] = '\0';
    			cout << "Reply by client on thread: " << thread_id << " is: " << server_reply << " : " << rec_size << endl;
    			break;
			}
    		rec_size = 0;
		}
    	rec_size = 0;
    	for(;rec_size == 0;) {
    		rec_size = recv(client_socket , server_reply , 2000 , 0);
			if(rec_size >0) {
    			server_reply[rec_size] = '\0';
    			cout << "Good Bye message: " << thread_id << " is: " << server_reply << " : " << rec_size << endl;
    			send(client_socket ,server_reply ,strlen(server_reply), 0);
    			close(client_socket);
    			break;
			}
    		rec_size = 0;
		}	   	
	}
}
	

int main() {
	
	WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char message[] = "Hello, I have received your connection\nPlease enter Your ID else enter 1 for registering";
    char server_reply[DATA_SIZE];
    int recv_size;
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
        
		printf("Failed. Error Code : %d",WSAGetLastError());
    }
    
    cout << thread::hardware_concurrency() << ": Threads are avalible\n";
     
    cout <<"Initialized.\n";
   //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET){
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    cout << "Socket created.\n";
         
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT_NUMBER );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){
		cout << "Bind failed with error code : " << WSAGetLastError() << "\n";
        exit(EXIT_FAILURE);
    }
     
    cout << "Bind done \n" ;
 
    //Listen to incoming connections
    listen(s , 5);
     
    //Accept and incoming connection
    cout << "Waiting for incoming connections...\n";
     
    c = sizeof(struct sockaddr_in);
    cout << c << " size is\n";
	int max = thread::hardware_concurrency();
	vector<thread> threads;
	for(int temp = 0; temp < (max -1); temp++){
 		thread newthread(connection,s,c,temp);
 		cout << "new thread created\n";
 		threads.push_back(move(newthread));
	}
	
	for(int temp = 0; temp < (max -1); temp++){
 		threads[temp].join();
	}
	return 0;
 }
