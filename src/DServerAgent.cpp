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
#include "DServerAgent.h"
#include "MicoleAgentThread.h"
#include "MicoleStringStream.h"


DServerAgent::DServerAgent(SOCKET sock, ServerAgent * srv)
: MicoleAgentThread("DServerAgent","DServerAgent")
{
	_sock = sock;
	_parent = srv;
	send(_sock, "All is ok!!!\0", 13, 0);
	start(); //Start thread*/
}

DServerAgent::~DServerAgent()
{}

void DServerAgent::run ()
{
	char *buffer = new char[1024];
	int result;

	while(getState()==ACTIVE)
	{
		result = recv(_sock, buffer, 1023, 0);

		if (result == -1) {
			_parent->removeThread(this);
		} else {
			OutputDebugStringA("In network: ");
			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
			_parent->sendOnAllClients(buffer);
		}
		//::Sleep(1);
	}
	closesocket(_sock);
	WSACleanup();

	
}

void DServerAgent::sendOnNetwork(string s) {
	send(_sock, (s+"\0").c_str(), (int(s.size())+1), 0);
}


/**
 * @TODO dummy function just for test
 */
void DServerAgent::prepareToStop(int argc, const char **argv)
{
}

void DServerAgent::prepareToSuspend(int argc, const char **argv)
{
}
