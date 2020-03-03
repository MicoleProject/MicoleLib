/*
 * This file is part of Micole Architecture
 *
 * Copyright (C) 2007 Micole partners
 *
 * Micole Architecture is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU Lesser General 
 * Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any 
 * later version.
 *
 * Micole Architecture is distributed in the hope that it will be 
 * useful, * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU Lesser General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Micole Architecture.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"
#include "NetworkAgent.h"
#include "MicoleAgentThread.h"
#include "MicoleStringStream.h"
#include "ServerAgent.h"

NetworkAgent::NetworkAgent(char * domain, int port)
: MicoleAgentThread("NetworkAgent","NetworkAgent")
{
	initSocket(domain, port);
	bindMessage( "^Net: (.*)$", BUS_CALLBACK_OF(NetworkAgent, handleNetworkMessage ));
	start(); //Start thread*/
}

void NetworkAgent::handleNetworkMessage ( MicoleBus *app, int argc, const char **argv )
{
	string message = string(argv[0]);
	sendOnNetwork(message);
}

void NetworkAgent::initSocket(char * domain, int port) 
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
	
	SOCKADDR_IN sin;
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	//host=(struct hostent*)gethostbyname(serverhost);
	
	sin.sin_addr.s_addr			= inet_addr(domain);
	sin.sin_family				= AF_INET;
	sin.sin_port				= htons(port);
	int retCode = connect(_sock, (SOCKADDR *)&sin, sizeof(sin));
	
	if ((retCode==-1)&&(strcmp(domain,"127.0.0.1")==0)) //if error and server is localhost
	{
		new ServerAgent(port);
		::Sleep(10);
		initSocket(domain, port);
	}
}

NetworkAgent::~NetworkAgent()
{}

void NetworkAgent::run ()
{
	char *buffer = new char[1024];

	while(getState()==ACTIVE)
	{
		recv(_sock, buffer, 1023, 0);
		OutputDebugStringA("In network: ");
		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");
		sendMessage(buffer);
		//::Sleep(1);
	}
	closesocket(_sock);
	WSACleanup();
}

void NetworkAgent::sendOnNetwork(string s) 
{
	send(_sock, (s+"\0").c_str(), (int(s.size())+1), 0);
}


/**
 * @TODO dummy function just for test
 */
void NetworkAgent::prepareToStop(int argc, const char **argv)
{
}

void NetworkAgent::prepareToSuspend(int argc, const char **argv)
{
}
