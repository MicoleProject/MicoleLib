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
#include "MicoleAgentThread.h"
//#include "agentmodule.h"

using namespace AgentModuleConstants;

MicoleAgentThread::MicoleAgentThread(const string &name, const string &type, const char * domain)
: MicoleAgent(name,type,domain)
{
	/*
	int count = bindMessage(QUIT, BUS_CALLBACK_OF(MicoleAgentThread, &MicoleAgentThread::handleQuitMessage));
	count = bindMessage(SUSPEND, BUS_CALLBACK_OF(MicoleAgentThread, &MicoleAgentThread::handleSuspendMessage));
	count = bindMessage(RESUME, BUS_CALLBACK_OF(MicoleAgentThread, &MicoleAgentThread::handleResumeMessage));
	*/
}

AgentState MicoleAgentThread::getState()
{
	return state;
}

MicoleAgentThread::~MicoleAgentThread()
{
	cout<<"Terminating MicoleAgentThread..."<<endl;
}

/*
void MicoleAgentThread::handleQuitMessage(MicoleBus *app, int argc, const char **argv ){
	prepareToStop(argc,argv);
	state = UNKNOWN;
	//terminate();
}
*/
void MicoleAgentThread::start() 
{
	state = ACTIVE;
	Thread::start();
}

/*
void MicoleAgentThread::handleSuspendMessage(MicoleBus *app, int argc, const char **argv ){
	state = SUSPENDED;
	prepareToSuspend(argc,argv);
	this->suspend();


}

void MicoleAgentThread::handleResumeMessage(MicoleBus *app, int argc, const char **argv ){
	state = ACTIVE;
	this->resume();


}*/
void MicoleAgentThread::suspendAgent()
{
/*	cout<<"Suspending MicoleAgentThread"<<endl;
	for (unsigned i = 0 ; i < bindings.size() ; ++i)
		unBindMessage(bindings[i]);

	resumeId = bus->bindMsg("^Resume$",BUS_CALLBACK_OF(MicoleAgent, handleResumeMessage ));*/
//	this->suspend();
	//bindings.erase(bindings.begin(),bindings.end());
}

void MicoleAgentThread::resumeAgent()
{
	/*
		-re-bind the agen's messages
		-unbind resume handler
		-resume thread
	*/
/*	for (unsigned i = 0 ; i < bindings.size() ; ++i)
		bindMessage( bindings[i]);

	bus->UnbindMsg(resumeId);
	this->resume();*/
}

void MicoleAgentThread::stopAgent()
{
	//prepareToStop(argc,argv);
//	this->terminate();
}
