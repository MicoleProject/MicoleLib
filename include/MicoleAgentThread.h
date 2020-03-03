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

#ifndef MICOLEAGENTTHREAD_H
#define MICOLEAGENTTHREAD_H
#include "micolelib.h"

#include "MicoleAgent.h"
#include "Lo2kThread.h"

/** @defgroup core Core
 *  Main core classes
 *  All core classes in "needed" by micoleLib. Please do not modify anything into it.
 *  @{
 */

/**
 * Threaded version of MicoleAgent
 * MicoleAgentThread provide easy thread use with a classic
 * run and start method
 * @see MicoleAgent
 */
class MICOLELIB_API MicoleAgentThread: public MicoleAgent, public lo2k::Thread
{
	public:
		~MicoleAgentThread();

		AgentState getState();

		/**
		 * the run method inherited from Thread, agent is started using Thread's start() method.
		 * This is an abstract function, must be implemented in inherited classes.
		 */
		virtual void run()=0;

		virtual void start();

		//void suspendAgent();
		//void activateAgent();
		//void handleQuitMessage(MicoleBus *app, int argc, const char **argv );
		//void handleSuspendMessage(MicoleBus *app, int argc, const char **argv );
		//void handleResumeMessage(MicoleBus *app, int argc, const char **argv );

	protected:
		//virtual void prepareToStop(int argc, const char **argv)=0;
		//virtual void prepareToSuspend(int argc, const char **argv)=0;
		void suspendAgent();
		void resumeAgent();
		virtual void stopAgent();

		/**
		 * Default constructor
		 * @param name agent name
		 * @param type agent type (not used for the moment)
		 * @param domain ip and port where the agent listen
		 */
		MicoleAgentThread(const string &name, const string &type, const char * domain = "127.255.255.255:2010");
};

/** @} */ //end core
#endif
