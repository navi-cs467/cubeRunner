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
void receiveMessage_S(int socketFD, char* buffer);
void sendMessage_S(int socketFD, char* buffer);
int createSocket_S(struct addrinfo *servinfo);
struct addrinfo* getServerInfo_S(char* portNum);

#endif
