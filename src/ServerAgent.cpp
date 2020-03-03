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
#include "ServerAgent.h"
#include "MicoleAgentThread.h"
#include "MicoleStringStream.h"

#include "DServerAgent.h"


ServerAgent::ServerAgent(int port)
: MicoleAgentThread("ServerAgent","ServerAgent")
{
	_port = port;
	start(); //Start thread*/
}

ServerAgent::~ServerAgent()
{}

void ServerAgent::run ()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
	SOCKET sock;
	SOCKET csock;
	SOCKADDR_IN sin;
	SOCKADDR_IN csin;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr			= INADDR_ANY;
	sin.sin_family				= AF_INET;
	sin.sin_port				= htons(_port);
	bind(sock, (SOCKADDR *)&sin, sizeof(sin));
	listen(sock, 0);
	while(getState()==ACTIVE)
	{
		int sinsize = sizeof(csin);
		if((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET)
			_DServList.push_back(new DServerAgent(csock,this));
	}
}

void ServerAgent::sendOnAllClients(string s) 
{
	for (list<DServerAgent*>::iterator k = _DServList.begin() ; k != _DServList.end() ; k++) 
		(*k)->sendOnNetwork(s);
}


/**
 * @TODO dummy function just for test
 */
void ServerAgent::prepareToStop(int argc, const char **argv)
{
}

void ServerAgent::prepareToSuspend(int argc, const char **argv)
{
}
