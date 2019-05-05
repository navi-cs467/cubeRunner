/*
 References: CS 372 & CS 344 Programming Assignments, https://beej.us/guide/bgnet/
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <omp.h>

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

/*
	Receive messages from client
*/
void receiveMessage(int socketFD)
{

	// buffer to hold messages received from the client
	char messageReceived[2048];
	memset(messageReceived, '\0', sizeof(messageReceived));

	int len_received, bytes_received;
	len_received = sizeof(messageReceived);

	bytes_received = recv(socketFD, messageReceived, len_received, 0);

	// check for error
	if (bytes_received == -1)
	{
		fprintf(stderr,"Error receiving from socket\n");
		exit(1);
	}

	// otherwise, print the message
	printf("Received from client: %s\n", messageReceived);
}

/*
	Uses the already created socket to start a connection to the client over that
	socket, returns 0 on success and 1 otherwise
*/
int startConnection(int socketFD, struct addrinfo *servinfo)
{
	// connect to socket, from Beej's Guide
	int connectStatus = connect(socketFD, servinfo->ai_addr, servinfo->ai_addrlen);

	// check for connection error, exit if error occurs
	if (connectStatus < 0)
	{
		fprintf(stderr,"Error connecting to client data connection\n");
		return 1;
	}

	return 0;
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
void starGameMultiPlayer(int firstClient, int secondClient)
{
	while(1)
	{
		struct timeval tv;
		fd_set readfds;

		// adding 2.5s timer for testing, will shorten this later as needed for game to run smoothly ***
		tv.tv_sec = 2;
    tv.tv_usec = 500000;

		// clear the set ahead of time
		FD_ZERO(&readfds);

		// add descriptors to the set
		FD_SET(firstClient, &readfds);
		FD_SET(secondClient, &readfds);

		//buffers for messages sent from client
		char clientMsg1[1024];
		memset(clientMsg1, '\0', sizeof(clientMsg1));

		//buffers for messages sent from client
		char clientMsg2[1024];
		memset(clientMsg2, '\0', sizeof(clientMsg2));

		int n = secondClient + 1;

		int status = select(n, &readfds, NULL, NULL, &tv);

		if (status == -1)
		{
    	printf("error: select"); // error occurred in select()
		}
		else if (status == 0)
		{
    	//no data
		}
		else
		{
			char confirm[100] = "Server: I have received your message\n";

	    // one or both of the descriptors have data, so we check both
	    if (FD_ISSET(firstClient, &readfds))
			{
	        recv(firstClient, clientMsg1, sizeof clientMsg1, 0);
					// otherwise, print the message
					printf("Received from client 1: %s\n", clientMsg1);
	    }

	    if (FD_ISSET(secondClient, &readfds))
			{
	        recv(secondClient, clientMsg2, sizeof clientMsg2, 0);
					printf("Received from client 2: %s\n", clientMsg2);
	    }
		}

		//periodically send data to clients, this will be replaced with game data
		//send data in two different threads
		char data[100] = "Server: Sending data periodically to client\n";

		//Set number of omp threads
		omp_set_num_threads(2);

		#pragma omp parallel sections
		{
		 #pragma omp section
     {
			 sendMessage(firstClient, data);
     }
		 #pragma omp section
     {
			 sendMessage(secondClient, data);
     }
	 	}
	}
}

/*
 Accepts new connections, accepting 1 new connection for single player and 2 for multiplayer
*/
void acceptConnections(int socketFD, char* p)
{
	// Beej's guide accepting connections from clients
	int firstClient, secondClient;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;

	printf("\nListening for new connections...\n");

	// accept incomming connection from first client
	addr_size = sizeof client_addr;
	firstClient = accept(socketFD, (struct sockaddr *)&client_addr, &addr_size);

	printf("Connected to first client...\n");

	// accept incomming connection from second client
	addr_size = sizeof client_addr;
	secondClient = accept(socketFD, (struct sockaddr *)&client_addr, &addr_size);
	printf("Connected to second client...\n");

	starGameMultiPlayer(firstClient, secondClient);
}


/*
    starts server, binds the initial socket for listening on and
    then calls acceptConnections to accept new connections to the server
*/
void initServer(char* portNum)
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
	acceptConnections(socketFD, portNum);

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
