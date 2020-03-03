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
#include "AudioDevice.h"

#define MAXVOL 10000

//////////////////////////////////////////////////////

uint8 AudioDevice::nextDevice = 1;
uint8 AudioDevice::activeDevice = 1;

//////////////////////////////////////////////////////

AudioDevice::AudioDevice (std::string id) {

	do {
		currDevice = nextDevice++;
		BASS_Init(currDevice, 44100, BASS_DEVICE_SPEAKERS, 0, NULL);
	} while (BASS_ErrorGetCode() == BASS_ERROR_ALREADY);

	if(BASS_ErrorGetCode() == BASS_OK)
	{
		this->id = id;
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
		BASS_GetInfo(&info);
		vol = uint16(BASS_GetVolume());
	}
	else
		currDevice = 0;
}

//////////////////////////////////////////////////////

AudioDevice::~AudioDevice (void)
{
	AudioStreamMap::const_iterator iter;

	if(currDevice != activeDevice)
	{
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
	}

	for(iter = audioStreamMap.begin(); iter != audioStreamMap.end(); ++iter)
		delete (*iter).second;
	audioStreamMap.clear();

	BASS_Free();
	nextDevice = currDevice;
}

//////////////////////////////////////////////////////

AudioStream* AudioDevice::CreateStream (std::string id, const char* filename, uint32 speakers, bool mono, bool looped)
{
	AudioStream* audioStream;
	AudioStreamMap::const_iterator iter;

	if(currDevice != activeDevice)
	{
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
	}

	if(!(audioStream = new AudioStream(filename, speakers, mono, looped)))
		return (AudioStream*)0;

	if((iter = audioStreamMap.find(id)) == audioStreamMap.end())
		audioStreamMap[id] = audioStream;
	else
	{
		delete (*iter).second;
		audioStreamMap[id] = audioStream;
	}
	return audioStream;
}

//////////////////////////////////////////////////////

AudioStream* AudioDevice::GetStream (std::string id)
{
	AudioStreamMap::const_iterator iter;

	if((iter = audioStreamMap.find(id)) != audioStreamMap.end())
		return (*iter).second;
	else
		return (AudioStream*)0;
}

//////////////////////////////////////////////////////

void AudioDevice::RemoveStream (std::string id)
{
	AudioStreamMap::iterator iter;

	if((iter = audioStreamMap.find(id)) != audioStreamMap.end())
	{
		delete (*iter).second;
		audioStreamMap.erase(iter);
	}
}

//////////////////////////////////////////////////////

void AudioDevice::SetVolume (uint16 volume)
{
	if(currDevice != activeDevice)
	{
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
	}

	if(BASS_SetVolume(volume > MAXVOL ? MAXVOL : volume))
		vol = volume;
}

//////////////////////////////////////////////////////

uint16 AudioDevice::GetVolume (void)
{
	return vol;
}

//////////////////////////////////////////////////////

std::string	AudioDevice::GetDeviceDescription (void)
{
	return std::string(BASS_GetDeviceDescription(currDevice));
}

//////////////////////////////////////////////////////

uint8 AudioDevice::GetDeviceID (void)
{
	return currDevice;
}

//////////////////////////////////////////////////////

void AudioDevice::Start (void)
{
	if(currDevice != activeDevice)
	{
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
	}

	BASS_Start();
}

//////////////////////////////////////////////////////

void AudioDevice::Pause (void)
{
	if(currDevice != activeDevice)
	{
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
	}

	BASS_Pause();
}

//////////////////////////////////////////////////////

void AudioDevice::Stop (void)
{
	if(currDevice != activeDevice)
	{
		BASS_SetDevice(currDevice);
		activeDevice = currDevice;
	}

	BASS_Stop();
}
	
//////////////////////////////////////////////////////

uint8 AudioDevice::GetSpeakersNum (void)
{
	return uint8(info.speakers);
}

//////////////////////////////////////////////////////