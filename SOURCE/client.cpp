/*
 References: CS 372 & CS 344 Programming Assignments
 https://beej.us/guide/bgnet/
 https://www.comrevo.com/2016/01/how-to-create-threads-using-openmp-api.html?m=1
 https://linux.die.net/man/3/getch
 https://stackoverflow.com/questions/15306463/getchar-returns-the-same-value-27-for-up-and-down-arrow-keys
*/

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
	Receives messages sent from sever over the socket, if the value of recv is 0
	the server has closed the connection and the program exits
*/
void receiveMessage_C(int socketFD, char* buffer)
{
	int bytes_received = recv(socketFD, buffer, MSG_SIZE, 0);

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

		// int len = sizeof(buffer);
		// char *p = buffer;
		// int n;
		// while ( len > 0 && (n=recv(socketFD,p,len,0)) > 0 ) {
		//   p += n;
		//   len =- n;
		// }
		// if ( len > 0 || n < 0 ) {
		//   // oops, something went wrong
		// }
}

/*
Loops until all data is sent to the server, making sure the complete message is
sent over the socket
*/
void sendMessage_C(int socketFD, char* buffer)
{
	// adapted from cs 344 lectures, make sure all the data is sent over the socket
	// Send message to client
	int charsWritten = send(socketFD, buffer, MSG_SIZE, 0);
	// if (charsWritten < sizeof(buffer)) printf("WARNING: Not all data written to socket!\n");
	//
	// int checkSend = -5;  // Holds amount of bytes remaining in send buffer
	// do
	// {
	// 	ioctl(socketFD, TIOCOUTQ, &checkSend);  // Check the send buffer for this socket
	// }
	//
	// while (checkSend > 0);  // Loop forever until send buffer for this socket is empty
			//
			// int len = sizeof(buffer);
			// char *p = buffer;
			// int n;
			// while ( len > 0 && (n=send(socketFD,p,len,0)) > 0 ) {
			//   p += n;
			//   len =- n;
			// }
			// if ( len > 0 || n < 0 ) {
			//   // oops, something went wrong
			// }
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

// // start game with connection to server, loops until SIGINT for now
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



// starting out with command line entered values for now
// int main(int argc, char *argv[])
// {
// 	/* Curses Initialisations */
// 	initscr();
// 	keypad(stdscr, TRUE);
// 	noecho();
// 	cbreak();
//
// 	// save command-line entered hostname
// 	char* hostname = argv[1];
//
// 	// save command-line entered port number
// 	char* portNum = argv[2];
//
// 	int socketFD = initSocket(hostname, portNum);
//
// 	// start game, with client sending first message
// 	startGame(socketFD);
// 	endwin();
// }
