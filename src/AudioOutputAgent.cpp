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
#include <AudioOutputAgent.h>
#include <MicoleStringStream.h>
#include <string.h>

AudioOutputAgent::AudioOutputAgent(const string baseDir):MicoleAgent("SoundOutput","Sound Output"),soundBase(baseDir)
{
	AudioManager::Initialise();
	AudioManager::InitialiseNextDevice("default");
	device = AudioManager::GetAudioDevice("default");
	assert(device);
	
	bindMessage( "^OUT SND : name=(.+);type=([0-9]+);vol=([0-9]+);spk=(.+);pan=(\-?[0-9]+);$", BUS_CALLBACK_OF(AudioOutputAgent,
		handleAudioMessage));
	bindMessage("^OUT SND : (STOP_ALL|PAUSE_ALL|RESUME_ALL);$", BUS_CALLBACK_OF(AudioOutputAgent,
		handleStopPauseAllMessage));
	stopped = false;
	/*pa = "PAUSE_ALL";
	sa = "STOP_ALL";
	ra = "RESUME_ALL";*/
	//"^OUT SND : name=(.*);type=(.*);vol=(.*);$"
}

AudioOutputAgent::~AudioOutputAgent()
{
	AudioManager::CleanUp();
	
}

void AudioOutputAgent::handleStopPauseAllMessage(MicoleBus *app, int argc, const char **argv){


	
	if(strcmp(PAUSE_ALL,argv[0])==0){
		device->Pause();
		//cout<<"PAUSE_ALL"<<endl;
	}

	else if(strcmp(STOP_ALL,argv[0])==0){
		device->Stop();
		stopped = true;
		//cout<<"STOP_ALL"<<endl;
	}

	else if(strcmp(RESUME_ALL,argv[0])==0){
		device->Start();
		//cout<<"RESUME_ALL"<<endl;
	}

}


void AudioOutputAgent::handleAudioMessage(MicoleBus *app, int argc, const char **argv)
{

	MicoleStringStream fileName;
	fileName<<soundBase<<"\\"<<argv[0];
	int type = atoi(argv[1]);
    int vol = atoi(argv[2]);
	string spk = argv[3];
	int pan = atoi(argv[4]);

	uint32 speakers = FRONT_SPEAKERS;
	if(spk.compare("REAR") == 0)
		speakers = REAR_SPEAKERS;
	else if(spk.compare("REAR2") == 0)
		speakers = SPEAKER_REAR2;
	else if(spk.compare("CENSUB") == 0)
		speakers = SPEAKER_CENLFE;

/*	MicoleStringStream st;
	for(int i=0; i<argc; i++){
		st<<"argv["<<i<<"] = "<<argv[i];
		sendMessage(st.str());
		st.flush();
	}
*/

	int n=0;
	MicoleStringStream id;
	switch(type){
		
		case PLAY:
			id<<argv[0];
			while(device->GetStream(id.str())){
				id<<++n;	
			}
			device->CreateStream(id.str(), fileName.str().data(), speakers, false, false);
			device->GetStream(id.str())->SetVolume(vol);
			device->GetStream(id.str())->SetPanning(pan);
			if(stopped){
				device->Start();
				stopped = false;
			}
			device->GetStream(id.str())->Play(true);
			break;

		case PLAY_INTERRUPT:
			device->CreateStream(argv[0], fileName.str().data(), speakers, false, false);
			device->GetStream(argv[0])->SetVolume(vol);
			device->GetStream(argv[0])->SetPanning(pan);
			if(stopped){
				device->Start();
				stopped = false;
			}
			device->GetStream(argv[0])->Play(true);
			break;

		case PLAY_INTERRUPT_ALL:
			device->Stop();
			device->CreateStream(argv[0], fileName.str().data(), speakers, false, false);
			device->Start();
			device->GetStream(argv[0])->SetVolume(vol);
			device->GetStream(argv[0])->SetPanning(pan);
			device->GetStream(argv[0])->Play(true);
			break;

		case LOOP:
			device->CreateStream(argv[0], fileName.str().data(), speakers, false, true);
			device->GetStream(argv[0])->SetVolume(vol);
			device->GetStream(argv[0])->SetPanning(pan);
			if(stopped){
				device->Start();
				stopped = false;
			}
			device->GetStream(argv[0])->Play(true);
			break;

		case LOOP_INTERRUPT:
			device->CreateStream(argv[0], fileName.str().data(), speakers, false, true);
			device->GetStream(argv[0])->SetVolume(vol);
			device->GetStream(argv[0])->SetPanning(pan);
			if(stopped){
				device->Start();
				stopped = false;
			}
			device->GetStream(argv[0])->Play(true);
			break;

		case LOOP_INTERRUPT_ALL:
			device->Stop();
			device->CreateStream(argv[0], fileName.str().data(), speakers, false, true);
			device->GetStream(argv[0])->SetVolume(vol);
			device->GetStream(argv[0])->SetPanning(pan);
			device->Start();
			device->GetStream(argv[0])->Play(true);
			break;

		case PAUSE:
			device->GetStream(argv[0])->Pause();
			break;

		//Stop playing a file given in argv[0]
		case STOP:

			device->GetStream(argv[0])->Stop();
			device->RemoveStream(argv[0]);
			break;
	
		//Resume playing a file given in argv[0]
		case RESUME:
			device->Start();
			if(device->GetStream(argv[0]))
				device->GetStream(argv[0])->Play(true);
			break;

		
		default:

			break;
	}
	


}
