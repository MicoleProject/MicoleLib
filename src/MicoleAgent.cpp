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
#include "MicoleAgent.h"
#include "MicoleStringStream.h"

using namespace AgentModuleConstants;

MicoleBus * MicoleAgent::bus = 0;

MicoleAgent::MicoleAgent(const string &name, const string &type, const char *domain)
:agentName(name), agentType(type), state(ACTIVE)
{
	cout<< "MicoleAgent constructor 1: " << name <<endl;
	if (!bus){
		bus = new MicoleBus(true);
		//bus->start(domain);
	}

	bindMessage("^Quit$", BUS_CALLBACK_OF(MicoleAgent, handleQuitMessage ));
	/*bindMessage("^Ping$", BUS_CALLBACK_OF(MicoleAgent, handlePingMessage ));
	
	bindMessage("^Start$", BUS_CALLBACK_OF(MicoleAgent, handleStartMessage));
	bindMessage("^Print$", BUS_CALLBACK_OF(MicoleAgent, handlePrintIDMessage));
	bindMessage("^RegisterReply:(.*)", BUS_CALLBACK_OF(MicoleAgent, handleRegisterReply));
	bindMessage("^Suspend$", BUS_CALLBACK_OF(MicoleAgent, handleSuspendMessage ));*/
	
}

MicoleAgent::~MicoleAgent()
{
	if(bus)
	{
		cout << "Stopping bus..." << endl;
		bus->stop();
		bus = 0;
	}
	if(bindings.size() >0)
		bindings.erase(bindings.begin(), bindings.end());
}

string MicoleAgent::getAgentName()
{
	return agentName;
}

string MicoleAgent::getAgentType()
{
	return agentType;
}

int MicoleAgent::sendMessage(const char *message)
{
	//sendDirectMessage("bus",1,message);
	bus->sendMsg(message);
	return 0;
}

int MicoleAgent::sendMessage(const  string message)
{
	
	bus->sendMsg(message.c_str());
	return 0;
}

//Ivy::BindMsg(...) returns new regexp_id every time it's called. So, we
//need to update the bindings after calling BindMsg().
int MicoleAgent::bindMessage(const char *regexp, MicoleCallback * cb)
{
	MicoleBinding b(regexp, bus, cb);

	bindings.push_back(b);
	//return b.getId();
	return 0;
}



void MicoleAgent::handleSuspendMessage( MicoleBus *app, int argc, const char **argv)
{
	suspendAgent();
	state = SUSPENDED;
}

void MicoleAgent::handleResumeMessage(MicoleBus *app, int argc, const char **argv)
{
	resumeAgent();
	state = ACTIVE;
}

void MicoleAgent::handleQuitMessage( MicoleBus *app, int argc, const char **argv)
{
	cout<<"MicoleAgent "<<getAgentName()<<" stopping..."<<endl;
	/*
	//@TODO: handle the unbinds...
	for (vector<MicoleBinding>::iterator i = bindings.begin(); i != bindings.end(); i++)
		unBindMessage(*i);*/

	//bus->UnbindMsg(resumeId);
	//bus->UnbindMsg(printId);
	//bus->UnbindMsg(startId);
	//bus->UnbindMsg(regReplyId); //@TODO cause crash, unknown bind
	//bus->UnbindMsg(pingId);
	//bus->UnbindMsg(quitId);
	stopAgent();
	state = UNKNOWN;
}


void MicoleAgent::handlePingMessage(MicoleBus *app, int argc, const char **argv)
{
	//cout << getAgentName() << " received msg: PING! " << endl;
	MicoleStringStream s;
	s << agentName << ": " << state;

}

void MicoleAgent::handleRegisterReply( MicoleBus *app, int argc, const char **argv)
{
	const char* arg = (argc < 1) ? "" : argv[0];
	cout << "Register Reply: " << arg << endl;
	identifier = arg;
}

// 
void MicoleAgent::suspendAgent()
{
	/*cout<<"Suspending MicoleAgent"<<endl;
	for (unsigned i = 0 ; i < bindings.size() ; ++i)
		unBindMessage(bindings[i]);

	resumeId = bus->BindMsg("^Resume$",BUS_CALLBACK_OF(MicoleAgent, handleResumeMessage ));
	*/
	//bindings.erase(bindings.begin(),bindings.end());
}


void MicoleAgent::resumeAgent()
{
	/*
		-re-bind the agen's messages
		-unbind resume handler
	*//*
	for (unsigned i = 0 ; i < bindings.size() ; ++i)
		bindMessage( bindings[i]);

	bus->UnbindMsg(resumeId);*/
}

void MicoleAgent::stopAgent()
{
	//bus->stop(); crashes the program
}

void MicoleAgent::handleStartMessage( MicoleBus *app, int argc, const char **argv)
{
	obtainIdentifier();
}

void MicoleAgent::handlePrintIDMessage( MicoleBus *app, int argc, const char **argv)
{
	cout<<"Name: "<<agentName<<"\t"<< "ID: "<<identifier<<endl;
}

void MicoleAgent::obtainIdentifier()
{
	if(this->agentType != "Registry")
		sendMessage("Register:"+agentName);
	else
		identifier = "1";
}

/*
void SuspendMsgListener::OnDirectMessage(MicoleBus *app, int id, const char *arg){
	
	if(id == 1)
	{
		cout<<"Trying to suspend MicoleAgent..."<<id<<endl;
		master->suspendAgent();
	}
	
}

SuspendMsgListener::SuspendMsgListener(MicoleAgent *m){

	master = m;
}
*/

