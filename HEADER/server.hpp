/*****************************************************
** Program name: server.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/14/2019
** Description: Header file for server function.
*****************************************************/

#ifndef server_hpp
#define server_hpp

#include "include.hpp"

void initServer(char* portNum, int* firstClient, int* secondClient);
void acceptConnections(int socketFD, int* firstClient, int* secondClient);
void bindSocket(int socketFD, struct addrinfo *servinfo);
int startConnection(int socketFD, struct addrinfo *servinfo);
void receiveMessage(int socketFD, char* buffer);
void sendMessage(int socketFD, char* buffer);
int createSocket(struct addrinfo *servinfo);
struct addrinfo* getServerInfo(char* portNum);

#endif
