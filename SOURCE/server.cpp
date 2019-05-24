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
struct addrinfo* getServerInfo_S(char* portNum)
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
int createSocket_S(struct addrinfo *servinfo)
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

void sendMessage_S(int socketFD, char* buffer)
{
	// Send message to client
	int charsWritten = send(socketFD, buffer, MSG_SIZE, 0);
}

void receiveMessage_S(int socketFD, char* buffer)
{
	int bytes_received = recv(socketFD, buffer, MSG_SIZE, 0);

	//check for error
	if (bytes_received == -1)
	{
		fprintf(stderr,"Error receving from socket\n");
		close(socketFD);
		exit(0);
	}

	// server closed the connection
	else if (bytes_received == 0)
	{
		fprintf(stderr,"Server has closed the connection\n");
		close(socketFD);
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

/*
 Accepts new connections, accepting 1 new connection for single player and 2 for multiplayer
*/
void acceptConnections(int socketFD, int* firstClient, int* secondClient)
{
	// Beej's guide accepting connections from clients
	struct sockaddr_storage client_addr;
	socklen_t addr_size;

	// accept incoming connection from first client
	addr_size = sizeof client_addr;
	*firstClient = accept(socketFD, (struct sockaddr *)&client_addr, &addr_size);

	// accept incoming connection from second client
	addr_size = sizeof client_addr;
	*secondClient = accept(socketFD, (struct sockaddr *)&client_addr, &addr_size);
}


/*
    starts server, binds the initial socket for listening on
*/
int initServer(char* portNum)
{
	struct addrinfo *servinfo = getServerInfo_S(portNum);

	// create socket to communicate with client
	int socketFD = createSocket_S(servinfo);

	// bind socket
	bindSocket(socketFD, servinfo);

  // print for debugging purposes, will remove later
	printf("Server Open On Port %s...\n", portNum);

	// listen on socket for multiple connections
	listen(socketFD, 10);

	// free servinfo linked list
	freeaddrinfo(servinfo);

	return socketFD;
}

// starting out with command line entered values for now, will change in coming weeks***
/* int main(int argc, char *argv[])
{

	// save command-line entered port number
	char* portNum = argv[1];

	// starts server processes
	initServer(portNum);
} */
