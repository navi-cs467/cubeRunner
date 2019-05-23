/*****************************************************
** Program name: client.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/14/2019
** Description: Header file for client function.
*****************************************************/

#ifndef client_hpp
#define client_hpp

#include "include.hpp"
#include "constants.hpp"

int initSocket(char* hostname, char* portNum);
void sendMessage_C(int socketFD, char* buffer);
void receiveMessage_C(int socketFD, char* buffer);
int startConnection(int socketFD, struct addrinfo *servinfo);
int createSocket_C(struct addrinfo *servinfo);
struct addrinfo* getServerInfo_C(char* hostname, char* portNum);

#endif
