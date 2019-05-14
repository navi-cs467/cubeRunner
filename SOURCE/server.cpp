/*
 References: CS 372 & CS 344 Programming Assignments, https://beej.us/guide/bgnet/
*/

#include "../HEADER/server.hpp"

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <sys/ioctl.h>
// #include <arpa/inet.h>
// #include <sys/stat.h>
// #include <sys/time.h>
// #include <omp.h>

/*
	Function uses getaddrinfo to return a addrinfo* struct, which
	contains an Internet address that can be specified in future calls to connect,
	it also set the criteria for selecting socket address structures returned in
	the list pointed to by servinfo such as the desired address family (AF_INET)
	and the desired address socktype (SOCK_STREAM)
*/
struct addrinfo* getServerInfo(char* portNum)
{
	// adpated from Beej's guide, get address info to fill addrinfo struct
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	// initialize struct as empty
	memset(&hints, 0, sizeof hints);

	// IPv4
	hints.ai_family = AF_INET;

	// tcp connection
	hints.ai_socktype = SOCK_STREAM;

	// handles ip for us
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, portNum, &hints, &servinfo);

	return servinfo;
}

/*
  Creates socket by using the results from the getaddrinfo function
	that was called in the getServerInfo function, returns the file descriptor (int)
	for the socket
*/
int createSocket(struct addrinfo *servinfo)
{
	int socketFD;

	// feed results of getaddrinfo to socket as explained in Beej's guide and returns socket descriptor
	socketFD = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	//check for errors
	if (socketFD < 0)
	{
		fprintf(stderr,"Error opening socket");
		exit(1);
	}

	return socketFD;
}

/*
 Loops until all data is sent to the client, making sure the complete message is
 sent over the socket
*/
void sendMessage(int socketFD, char* buffer)
{
	// adapted from cs 344 lectures, make sure all the data is sent over the socket
	// Send message to client
	int charsWritten = send(socketFD, buffer, strlen(buffer), 0);
	if (charsWritten < strlen(buffer)) printf("WARNING: Not all data written to socket!\n");

	int checkSend = -5;  // Holds amount of bytes remaining in send buffer
	do
	{
		ioctl(socketFD, TIOCOUTQ, &checkSend);  // Check the send buffer for this socket
	}

	while (checkSend > 0);  // Loop forever until send buffer for this socket is empty
}

void receiveMessage(int socketFD, char* buffer)
{
	int len_received, bytes_received;
	len_received = sizeof(buffer);
	bytes_received = recv(socketFD, buffer, len_received, 0);

	//check for error
	if (bytes_received == -1)
	{
		fprintf(stderr,"Error receving from socket\n");
		exit(0);
	}

	// server closed the connection so client is terminated
	else if (bytes_received == 0)
	{
		fprintf(stderr,"Server has closed the connection\n");
		close(socketFD);
		endwin();
		exit(0);
	}
}

/*
	Binds the already created socket so that it can start listening
	for incoming connections
*/
void bindSocket(int socketFD, struct addrinfo *servinfo)
{
	bind(socketFD, servinfo->ai_addr, servinfo->ai_addrlen);
}

// start the game with two clients, determining which connections have input on sockets to read from
// for now we start chat, wait for client input and then send a confirmation message
// loops until SIGINT *** will adjust this later
// start game with connection to server, loops until SIGINT for now
	// void startGame(int socketFD)
	// {
	// 		//create two threads, one for sending data to server and one for receiving
	// 		//we do this so that we will still receive server data despite the blocking of getch
	// 		omp_set_num_threads(2);
	//
	// 		#pragma omp parallel sections
	// 		{
	// 			// get user input to send to server
	// 		 #pragma omp section
	//      {
	// 			 while(1)
	// 			 {
	// 					//buffers for messages for client to server
	// 	 			  char userInput[2048];
	//
	// 	 			  memset(userInput, '\0', sizeof(userInput));
	//
	// 	 		 		// Get input from the user using getch to avoid the user needing to press enter
	// 					int ch = getch();
	//
	// 					sprintf(userInput, "%c", ch);;
	//
	// 	 		 		sendMessage(socketFD, userInput);
	// 			 }
	//      }
	//
	// 		 //receving data from server periodically on different thread
	// 		 #pragma omp section
	//      {
	// 			while(1)
	// 			{
	// 				// buffer to hold messages received from the server
	// 				char messageReceived[2048];
	// 				memset(messageReceived, '\0', sizeof(messageReceived));
	//
	// 				//receive data from server, if there is any
	// 			 	receiveMessage(socketFD, messageReceived);
	//
	// 				//print out for testing purposes
	// 				printw("%s", messageReceived);
	// 				refresh();
	//
	// 				//create another function to parse server data (game metrics) ***
	// 			}
	//      }
	// 	}
	// }


/*
 Accepts new connections, accepting 1 new connection for single player and 2 for multiplayer
*/
void acceptConnections(int socketFD, int* firstClient, int* secondClient)
{
	// Beej's guide accepting connections from clients
	struct sockaddr_storage client_addr;
	socklen_t addr_size;

	// accept incomming connection from first client
	addr_size = sizeof client_addr;
	*firstClient = accept(socketFD, (struct sockaddr *)&client_addr, &addr_size);

	//send confirmation that client 1 is connected
	char confirm1[1] = "0";
	char confirm2[1] = "1";

	sendMessage(*firstClient, confirm1);

	// accept incomming connection from second client
	addr_size = sizeof client_addr;
	*secondClient = accept(socketFD, (struct sockaddr *)&client_addr, &addr_size);

	//send confirmation to both client 1 & 2 that both players are connected
	sendMessage(*secondClient, confirm2);

	//will add game mode checks later ***

}


/*
    starts server, binds the initial socket for listening on and
    then calls acceptConnections to accept new connections to the server
*/
void initServer(char* portNum, int* firstClient, int* secondClient)
{
	struct addrinfo *servinfo = getServerInfo(portNum);

	// create socket to communicate with client
	int socketFD = createSocket(servinfo);

	// bind socket
	bindSocket(socketFD, servinfo);

  // print for debugging purposes, will remove later
	printf("Server open on port %s\n", portNum);

	// listen on socket for multiple connections
	listen(socketFD, 10);

	// accepting new connections
	acceptConnections(socketFD, firstClient, secondClient);

	// free servinfo linked list
	freeaddrinfo(servinfo);
}

// starting out with command line entered values for now, will change in coming weeks***
/* int main(int argc, char *argv[])
{

	// save command-line entered port number
	char* portNum = argv[1];

	// starts server processes
	initServer(portNum);
} */
