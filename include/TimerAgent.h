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

#ifndef TIMERAGENT_H
#define TIMERAGENT_H
#include "micoleagentthread.h"

#include "micolelib.h"


/**
 * Timer Agent count seconds elapsed since it starts
 */
class MICOLELIB_API TimerAgent: public MicoleAgentThread
{
	public:
		TimerAgent(const string &name="TimerAgent", const string &type="ThreadedTimer", const int &_delayMs = 1000);
		virtual ~TimerAgent(void);
		
		virtual void run();
		virtual void onTimer()  {};

		void handleTime(MicoleBus *app,int argc, const char **argv);
		int getElapsedTime();

	private:
		int _elapsedTime; /**< elapsed since thread start*/
		int _threadActive;
		int _delay;

	protected:
		virtual void prepareToStop(int argc, const char **argv);
		virtual void prepareToSuspend(int argc, const char **argv);
};

#endif
