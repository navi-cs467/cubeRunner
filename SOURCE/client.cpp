/*****************************************************************
** Program name: client.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for client networking functions.
******************************************************************/

#include "../HEADER/client.hpp"

/*
  Function uses getaddrinfo to return a addrinfo* struct, which
	contains an Internet address that can be specified in future calls to connect,
	it also set the criteria for selecting socket address structures returned in
	the list pointed to by servinfo such as the desired address family (AF_INET)
	and the desired address socktype (SOCK_STREAM)
*/
struct addrinfo* getServerInfo_C(char* hostname, char* portNum)
{
	// from Beej's guide, get addrees info to fill addrinfo struct
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	// initialize struct as empty
	memset(&hints, 0, sizeof hints);

	// IPv4
	hints.ai_family = AF_INET;

	// tcp connection
	hints.ai_socktype = SOCK_STREAM;

	// getting ready to connect to host
	status = getaddrinfo(hostname, portNum, &hints, &servinfo);

	return servinfo;
}

/*
  Creates socket by using the results from the getaddrinfo function
	that was called in the getServerInfo function, returns the file descriptor (int)
	for the socket
*/
int createSocket_C(struct addrinfo *servinfo)
{
	int socketFD;

	// feed results of getaddrinfo to socket as explained in Beej's guide and returns socket descriptor
	socketFD = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	return socketFD;
}

/*
  Uses the already created socket to start a connection to the server over that socket
*/
int startConnection(int socketFD, struct addrinfo *servinfo)
{
	// connect to socket, from Beej's Guide
	int connectStatus = connect(socketFD, servinfo->ai_addr, servinfo->ai_addrlen);

	return connectStatus;
}

/*
	Receives messages sent from server over the socket, returns the result
*/
int receiveMessage_C(int socketFD, char* buffer)
{
	//receive message from server
	int bytes_received = recv(socketFD, buffer, MSG_SIZE, 0);

	return bytes_received;

}

/*
	Send data to the server, returning the result
*/
int sendMessage_C(int socketFD, char* buffer)
{

	// Send message to server
	int charsWritten = send(socketFD, buffer, MSG_SIZE, 0);

	return charsWritten;

}

// initialize the socket for client to connect to server
int initSocket(char* hostname, char* portNum)
{
	struct addrinfo *servinfo = getServerInfo_C(hostname, portNum);

	// create socket to communicate with server
	int socketFD = createSocket_C(servinfo);

	// start connection to server
	int connectStatus = startConnection(socketFD, servinfo);

	//if socket couldn't be created, return the negative value
	if (socketFD < 0)
	{
		return socketFD;
	}

	//if socket couldn't connect, return the negative value
	if (connectStatus < 0)
	{
		return connectStatus;
	}

	//otherwise, return the valid connected socket
	return socketFD;
}

/*
 References: CS 372 & CS 344 Programming Assignments
 https://beej.us/guide/bgnet/
*/
