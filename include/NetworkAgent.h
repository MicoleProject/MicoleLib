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

#ifndef NetworkAgent_H
#define NetworkAgent_H

#include "micolelib.h"

#include <config.h>
#include "MicoleAgentThread.h"
#include <winsock2.h>

#include <iostream>
//#include <string>

using namespace std;


/** @defgroup network Network
 *  Network control classes
 *  @{
 */

/*!
 * \brief
 * Tunnel beetween local bus and a distant bus
 * 
 * This class provide network connection with a distant network. 
 * Creation of a distant connection is provided by the function startNework in AbstractMicoleApplication classes.
 *
 * Prefix you message by "Net: " to ensure what your message is send on distant Network.
 *
 * Example:
 * 
 * \code
 * Net: OUT VTP : right=(0000000000001111,0000000011110000,0000111100000000,1111000000000000); dur=100;
 * \endcode
 * 
 * \see
 * ServerAgent | DServerAgent | AbstractMicoleApplication::startNetwork
 */
class MICOLELIB_API NetworkAgent :  MicoleAgentThread
{
	private:
		void initSocket(char * domain, int port);
		void handleNetworkMessage ( MicoleBus *app, int argc, const char **argv );
		SOCKET _sock;


	public:

		NetworkAgent(char * domain, int port); 
		
		~NetworkAgent();

		/**
		 * thread main loop
		 */
		virtual void run();
		void sendOnNetwork(string s);

	protected:
		/**
		 * callback when thread is prepared to be stopped
		 */
		virtual void prepareToStop(int argc, const char **argv);
		/**
		 * callback when thread is prepared to be suspended
		 */ 
		virtual void prepareToSuspend(int argc, const char **argv);

};

#endif

/** @} */ //end network