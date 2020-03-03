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
#include "registryagent.h"
#include "Entry.h"

#include "MicoleStringStream.h"


using namespace AgentModuleConstants;

RegistryAgent::RegistryAgent(const string &name, const string &domain)
: MicoleAgent(name,"Registry","127.255.255.255:2010")
{
	Entry e("Registry", ACTIVE);
	agents[e.getId()] = e;
	bindMessage("^Register:(.*)", BUS_CALLBACK_OF(RegistryAgent, handleRegisterMessage ));
	bindMessage("^Unregister:(.*)", BUS_CALLBACK_OF(RegistryAgent, handleUnRegisterMessage ));
	bindMessage("^PrintRegister$", BUS_CALLBACK_OF(RegistryAgent, handlePrintRequest ));
	
	bindMessage("^Set (.*) = (.*);$", BUS_CALLBACK_OF(RegistryAgent, handleSetVar ));
	bindMessage("^SET (.*) = (.*);$", BUS_CALLBACK_OF(RegistryAgent, handleSetVar ));

	bindMessage("^GET (.*);$", BUS_CALLBACK_OF(RegistryAgent, handleGetVar ));
}

RegistryAgent::~RegistryAgent()
{
	cout << "RegistryAgent destructor" << endl;
}


void RegistryAgent::handleSetVar(MicoleBus *app, int argc, const char **argv) 
{
	setVar(argv[0],argv[1]);
}


void RegistryAgent::handleGetVar(MicoleBus *app, int argc, const char **argv) 
{
	MicoleStringStream mss;
	mss << "VAR " << string(argv[0]) << "=" << getVar(argv[0]); 
	sendMessage(mss.str());
}


void RegistryAgent::handleQuitMessage( MicoleBus *app, int argc, const char **argv)
{
	cout << "registryAgent stopping..." << endl;
	agents.clear();
}

void RegistryAgent::handleRegisterMessage(MicoleBus *app, int argc, const char **argv)
{
	cout << "HandleRegister..." << endl;
	
	const char* arg = (argc < 1) ? "" : argv[0];
	//ostringstream s;

	//string reply = "";
	Entry e(arg, ACTIVE);// = new Entry;
	e.setApp(app);
	agents[e.getId()] = e;

	//s<<e.id;
	//reply = string("RegisterReply:")+s.str();
	//reply = string("RegisterReply:")+string(e.id+0x30);
	//e.app->SendMsg(reply.data());
}

void RegistryAgent::handlePrintRequest(MicoleBus *app, int argc, const char **argv)
{
	/*typedef map<int,Entry>::iterator i;
	Entry e;
	for (i p = agents.begin(); p!= agents.end(); ++p)
	{
		e = p->second;
		cout <<e.id<<'\t'<<e.name<<'\t'<<e.state<<'\n';
	}*/
	
	for (map<int,Entry>::iterator i = agents.begin() ; i != agents.end(); i++)
		cout << (i->second).getId() << '\t' << (i->second).getName() << '\t' << (i->second).getState() << endl;
}
void RegistryAgent::handleUnRegisterMessage(MicoleBus *app, int argc, const char **argv)
{
}
