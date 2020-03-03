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

#ifndef SOUNDOUTPUTAGENT_H
#define SOUNDOUTPUTAGENT_H

#include "micolelib.h"


#define _WINSOCKAPI_ //prevent windows.h winscock inclusion (already present in cc++)
#include <config.h>

#include "MicoleAgentThread.h"

#include <Display.h>
#include <Scene.h>
#include <Node.h>
#include <Field.h>
#include <SoundSource.h>

#include <iostream>
#include <map>

using namespace Reachin;
using namespace std;

//Required dependencies (Project Properties - Linker input)
//ALSound_d.lib
//ALut.lib
//OpenAL32.lib
//
//Upon creation of a SoundSource, the language of choice is specified, 
//effectively resulting in the creation of a language specific sound. 
//By controlling which language version that should be played 
//with a variable, the choice of language can be made by the user 
//when the application is started. This simplifies development of 
//applications that utilizes a lot of speech as output, as changing 
//language output does not require rewriting of code.
//
//The user should choose which language to use when starting a 
//speech enabled application. The interface for choosing language  
//will be application specific.
//
//When creating the SoundOutputAgent, specify which sound library to use
//e.g. 	
//string language = "Greek";
//SoundOutputAgent * sa = new SoundOutputAgent(language);
//
//This will map to: 
//"urn:inet:reachin.se:/Sounds/Greek/"
//
//If there is a file called greek1.wav in the specified sound library,
//play it by running IvyProbe.exe and typing PlaySound greek1.wav.
//
//!! Please note that there should be exactly 1 whitespace between the 
//specified action and the sound file of choice.
//
//Available Actions:
//PlaySound greek1.wav
//StopSound greek1.wav
//CancelSound greek1.wav
//PauseSound greek1.wav
//
//The pitch, gain and loop for specific sounds should be 
//tweaked by the application engineer, allowing for the 
//user to only have to adjust the master volume of 
//an application. 

//The gain, loop, pitch can be set with the functions
//setGain
//setLoop
//setPitch

class MICOLELIB_API SoundOutputAgent: public Node, MicoleAgent
{
	public:
		SoundOutputAgent(const string &lan);

		//*********************************************************************
		//TODO: put the datamembers in private !
		//
		//*********************************************************************
		//stores language as specified when the 
		//SoundOutputAgent is created
		string language;

		SoundSource* sound;
		map< string, SoundSource* > nameToSound;
		typedef pair < string, SoundSource* > pair_soundname_to_soundsource;
		Display* dis;

		void handlePlaySoundMessage( MicoleBus *app, int argc, const char **argv );
		void handleStopSoundMessage( MicoleBus *app, int argc, const char **argv );
		void handlePauseSoundMessage( MicoleBus *app, int argc, const char **argv );
		void handleCancelSoundMessage( MicoleBus *app, int argc, const char **argv );

		void setGain(string soundname, float gain);
		void setPitch(string soundname, float pitch);
		void setLoop(string soundname, bool loop);

		void createSound( const string &soundname, const string &language )
		{
			SoundSource* soundsource = new SoundSource();
			soundsource->play->set(false);
			soundsource->loop->set(true);
			soundsource->pitch->set(1.0);
			soundsource->gain->set(1.0);
			
			nameToSound.insert( pair_soundname_to_soundsource( soundname, soundsource ) );

			//Ex: "urn:inet:reachin.se:/Sounds/Greek/greek1.wav"
			string url = "urn:inet:reachin.se:/Sounds/" + language + "/" + soundname;

			soundsource->url->set(url);
			dis->children->add(soundsource);
		}
	private:
};

#endif
