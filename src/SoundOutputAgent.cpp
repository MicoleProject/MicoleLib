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

#include "SoundOutputAgent.h"
#include "ReachinMicoleApplication.h"
#include "MicoleAgentThread.h"


SoundOutputAgent::SoundOutputAgent(const string &lan): MicoleAgent("SoundOutputAgent","SenderAgent")  
{
	//set language of choice
	language = lan;

	bindMessage( "^PlaySound(.*)", BUS_CALLBACK_OF(SoundOutputAgent, handlePlaySoundMessage ));
	bindMessage( "^StopSound(.*)", BUS_CALLBACK_OF(SoundOutputAgent, handleStopSoundMessage ));
	bindMessage( "^PauseSound(.*)", BUS_CALLBACK_OF(SoundOutputAgent, handlePauseSoundMessage ));
	bindMessage( "^CancelSound(.*)", BUS_CALLBACK_OF(SoundOutputAgent, handleCancelSoundMessage ));

	LoadLibrary("ALSound_d.dll");
	dis = ReachinMicoleApplication::getInstance()->getDisplay();
}


void SoundOutputAgent::handlePlaySoundMessage(IvyApplication *app, int argc, const char **argv)
{
	//echo "Play"
	string s_result = "Play";
	sendMessage(s_result.data());
	
	string input(argv[0]);
	//copy all letters but one, starting at position 1
	string name( input, 1, input.size() );
	
	//if soundsource exists - play it
	if (nameToSound.find(name) != nameToSound.end())
	{
		//find soundsource and play it
		nameToSound.find(name)->second->play->set(true);
	}
	//if no match - create soundsource and play it
	else
	{
		createSound(name, language);
		sound = nameToSound.find(name)->second;
		sound->play->set(true);
	}
}

void SoundOutputAgent::handleStopSoundMessage(IvyApplication *app, int argc, const char **argv)
{
	//echo "Stop"
	string s_result = "Stop";
	sendMessage(s_result.data());

	string input( argv[0] );
	//copy all letters but one, starting at position 1
	string name( input, 1, input.size() );

	//if soundsource exists - stop it
	if (nameToSound.find(name) != nameToSound.end())
	{
		sound = nameToSound.find(name)->second;
		sound->stop->set(true);
	}
	//if not - do nothing
}

void SoundOutputAgent::handlePauseSoundMessage(IvyApplication *app, int argc, const char **argv) 
{
	//echo "Pause"
	string s_result = "Pause";
	sendMessage(s_result.data());

	string input( argv[0] );
	//copy all letters but one, startin at position 1
	string name( input, 1, input.size() );

	//if soundsource exists and is playing - pause it
	if ( nameToSound.find(name) != nameToSound.end() )
	{
		sound = nameToSound.find(name)->second;
		sound->play->set(false);
	}
	//if not - do nothing
}

void SoundOutputAgent::handleCancelSoundMessage(IvyApplication *app, int argc, const char **argv)
{
	//echo "Cancel"
	string s_result = "Cancel";
	sendMessage(s_result.data());

	string input( argv[0] );
	//copy all letters but one, starting at position 1
	string name( input, 1, input.size() );

	//if soundsource exists and is playing - cancel it
	if ( nameToSound.find(name) != nameToSound.end() )
	{
		sound = nameToSound.find(name)->second;
		sound->stop->set(true);
	}
	//if not - do nothing
}

void SoundOutputAgent::setGain(string name, float gain )
{
	if ( nameToSound.find(name) != nameToSound.end() )
	{
		sound = nameToSound.find(name)->second;
		sound->gain->set( gain );
	}
}

void SoundOutputAgent::setPitch( string name, float pitch )
{
	if ( nameToSound.find(name) != nameToSound.end() )
	{
		sound = nameToSound.find(name)->second;
		sound->pitch->set( pitch );
	}
}

void SoundOutputAgent::setLoop( string name, bool loop )
{
	if ( nameToSound.find(name) != nameToSound.end() )
	{
		sound = nameToSound.find(name)->second;
		sound->loop->set( loop );
	}
}
