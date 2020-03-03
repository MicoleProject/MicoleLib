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

#ifndef ServerAgent_H
#define ServerAgent_H
#include "micolelib.h"

#define BUTTON_LAG 0.1 /**< define button lag time*/

#include <config.h>
#include "MicoleAgentThread.h"
#include "DServerAgent.h"

#include <Display.h>
#include <Scene.h>
#include <Node.h>
#include <Field.h>

#include <iostream>
#include <list>

using namespace Reachin;
using namespace std;

class DServerAgent;

/** 
 * ServerAgent send 3d coordinates of reachin device
 * reachin device is configured into REACHIN API Config tools.<br/>
 * Reachin device is usually a Phantom but can be a delta, 
 * omega or other 3D feedback device 
 */

class MICOLELIB_API ServerAgent : public MicoleAgentThread
{
	private:
		int _port;
		list<DServerAgent *> _DServList;


	public:
		ServerAgent(int port); 
		
		~ServerAgent();

		/**
		 * thread main loop
		 */
		virtual void run();

		//void sendOnAllNetwork(string s);
		void sendOnAllClients(string s);

		void removeThread(DServerAgent * srv) {
			_DServList.remove(srv);
		}

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
