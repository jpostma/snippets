// BroadcastTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#if WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"WS2_32.lib")
typedef int socklen_t;
#elif linux
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>

typedef int SOCKET;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define _stricmp strcasecmp
#endif
#include <iostream>



int main()
{
#if WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2),&wsaData);
	SOCKET listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#elif linux
	SOCKET listenSocket = socket(AF_INET, SOCK_DGRAM, 0);
#endif

	BOOL enable = TRUE;
	int ret = setsockopt(listenSocket, SOL_SOCKET, SO_BROADCAST, (char*)&enable, sizeof(enable));
	if (ret != 0)
		perror(strerror(errno));

	ret = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
	if (ret != 0)
		perror(strerror(errno));
	sockaddr_in bindAddress;
	bindAddress.sin_addr.s_addr = INADDR_ANY;
	bindAddress.sin_family = AF_INET;
	bindAddress.sin_port = htons(4000);
	ret = bind(listenSocket, (sockaddr*)&bindAddress, sizeof(bindAddress));
	if (ret != 0)
		perror(strerror(errno));

	SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ret = setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST, (char*)&enable, sizeof(enable));
	if (ret != 0)
		perror(strerror(errno));

#if 0
	sockaddr_in sendAddress;
	sendAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_port = htons(4000);
	char message[] = "hahaha";
	ret = sendto(sendSocket, message, strlen(message) + 1, 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
	if (ret <= 0)
		perror(strerror(errno));
#endif

	while (1)
	{
		char buf[1024] = { 0 };
		sockaddr_in from;
		socklen_t fromSize = sizeof(from);
		int ret = recvfrom(listenSocket, buf, sizeof(buf) - 1, 0, (sockaddr*)&from, &fromSize);
		if (ret <= 0)
		{
			printf("Closed...\n");
			break;
		}
		if (!_stricmp(buf, "anyonethere"))
		{
			char hostName[1024],buf[1024];
			gethostname(hostName, 1024);
			sprintf(buf,"host=%s", hostName);
			sendto(listenSocket, buf, strlen(buf) + 1, 0, (sockaddr*)&from, sizeof(from));
		}
		char fromBuffer[1024] = { 0 };
		inet_ntop(AF_INET, &from.sin_addr.s_addr, fromBuffer, sizeof(fromBuffer));
		printf("received: %s from %s\n", buf,fromBuffer);
	}
	return 0;
}