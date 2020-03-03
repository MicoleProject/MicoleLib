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

//#include "agentmodule.h"
//#include <cc++/tokenizer.h>
//#include <cc++\thread.h>
#include "soundplayeragent.h"


#include <string>

using namespace std;
using namespace AgentModuleConstants;
//using ost::StringTokenizer;

SoundPlayerAgent::SoundPlayerAgent(string name, string type, const char* domain):MicoleAgent(name,type,domain)
{
	bindMessage( SOUND, BUS_CALLBACK_OF(SoundPlayerAgent, handleOutputMessage ));
	bindMessage( "^Bye$",      BUS_CALLBACK_OF(SoundPlayerAgent, handleQuitMessage ));
}

SoundPlayerAgent::SoundPlayerAgent():MicoleAgent("SoundPlayerAgent","SoundAgent","127.255.255.255:2010")
{
	bindMessage( SOUND, BUS_CALLBACK_OF(SoundPlayerAgent, handleOutputMessage ));
	//count = bindMessage( "^Quit$",      BUS_CALLBACK_OF(SoundPlayerAgent, &SoundPlayerAgent::handleQuitMessage ));
}

void SoundPlayerAgent::handleOutputMessage(IvyApplication *app, int argc, const char **argv)
{
/*
	StringTokenizer st(*argv,";:=",false,true);
	//char *c = "file";
	string str = "file";
	string name = "name";
	bool fileb = false;
	bool nameb = false;

	StringTokenizer::iterator i;
	for (i = st.begin() ; i != st.end() ; ++i)
	{
		if (*i == str)
		{
			cout<<"We have a match!"<<endl;
			fileb = true;
		}
		else if (*i == name)
			nameb = true;
		cout << "Token: '" << *i << "'\t";
		cout << " next Delim: '" << i.nextDelimiter() << "'" << endl;
	}*/
}

void SoundPlayerAgent::handleQuitMessage( IvyApplication *app, int argc, const char **argv )
{
	cout << "HelloAgent stops bus" << endl;
	cont = false;
	if (bus)
	{
		//agent->stop();
		bus->stop();
		delete bus; // This statement is never reached! Don't know why!  
	}
}
