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

#ifndef DServerAgent_H
#define DServerAgent_H
#include "micolelib.h"

#define BUTTON_LAG 0.1 /**< define button lag time*/

#include <config.h>
#include "MicoleAgentThread.h"
#include "ServerAgent.h"

#include <Display.h>
#include <Scene.h>
#include <Node.h>
#include <Field.h>

#include <iostream>
//#include <string>

using namespace Reachin;
using namespace std;


/** @defgroup core Core
 *  Main core classes
 *  All core classes in "needed" by micoleLib. Please do not modify anything into it.
 *  @{
 */


class ServerAgent;



/*!
 * \brief
 * Dedicated server agent class
 * 
 * this class provide a dedicated connection beetween a client and the server.
 * 
 * \see
 * ServerAgent
 */
class MICOLELIB_API DServerAgent : public MicoleAgentThread
{
	private:
		ServerAgent * _parent; /**< parent server */
		SOCKET _sock; /**< windows socket */

	public:
		/**
		 * default constructor
		 * do not call it by yourself
		 */
		DServerAgent(SOCKET sock, ServerAgent * srv); 
		
		/**
		 * default destructor
		 */
		~DServerAgent();

		/**
		 * thread main loop
		 */
		virtual void run();
		
		/**
		 * send information throught the network
		 * do not use it by yourself
		 * @param s message to send through the network
		 */
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

/** @} */ //end core

#endif
