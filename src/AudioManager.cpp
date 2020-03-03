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

#pragma warning(disable:4786)

#include "stdafx.h"
#include <windows.h>

#include "AudioManager.h"
#include "util.h"

//////////////////////////////////////////////////////

AudioDeviceMap AudioManager::audioDeviceMap;
ErrorCodeMap AudioManager::errorCodeMap;

//////////////////////////////////////////////////////

void AudioManager::FillUpErrorCodeMap (void)
{
	errorCodeMap[0] = "OK";
	errorCodeMap[1] = "Error_MEM";
	errorCodeMap[2] = "Error_FILEOPEN";
	errorCodeMap[3] = "Error_DRIVER";
	errorCodeMap[4] = "Error_BUFLOST";
	errorCodeMap[5] = "Error_HANDLE";
	errorCodeMap[6] = "Error_FORMAT";
	errorCodeMap[7] = "Error_POSITION";
	errorCodeMap[8] = "Error_INIT";
	errorCodeMap[9] = "Error_START";
	errorCodeMap[14] = "Error_ALREADY";
	errorCodeMap[16] = "Error_NOPAUSE";
	errorCodeMap[18] = "Error_NOCHAN";
	errorCodeMap[19] = "Error_ILLTYPE";
	errorCodeMap[20] = "Error_ILLPARAM";
	errorCodeMap[21] = "Error_NO3D";
	errorCodeMap[22] = "Error_NOEAX";
	errorCodeMap[23] = "Error_DEVICE";
	errorCodeMap[24] = "Error_NOPLAY";
	errorCodeMap[25] = "Error_FREQ";
	errorCodeMap[27] = "Error_NOTFILE";
	errorCodeMap[29] = "Error_NOHW";
	errorCodeMap[31] = "Error_EMPTY";
	errorCodeMap[32] = "Error_NONET";
	errorCodeMap[33] = "Error_CREATE";
	errorCodeMap[34] = "Error_NOFX";
	errorCodeMap[35] = "Error_PLAYING";
	errorCodeMap[37] = "Error_NOTAVAIL";
	errorCodeMap[38] = "Error_DECODE";
	errorCodeMap[39] = "Error_DX";
	errorCodeMap[40] = "Error_TIMEOUT";
	errorCodeMap[41] = "Error_FILEFORM";
	errorCodeMap[42] = "Error_SPEAKER";
	errorCodeMap[43] = "Error_VERSION";
	errorCodeMap[44] = "Error_CODEC";
	errorCodeMap[-1] = "Error_UNKNOWN";
}

//////////////////////////////////////////////////////

void AudioManager::CheckError (void)
{
	char message[128];
	uint8 error;

	if(error = BASS_ErrorGetCode())
		sprintf_s(message, "%s", errorCodeMap[error]);
}

//////////////////////////////////////////////////////

void AudioManager::Initialise (void)
{
	FillUpErrorCodeMap();
}

//////////////////////////////////////////////////////

void AudioManager::CleanUp (void)
{
	AudioDeviceMap::const_iterator iter;

	for(iter = audioDeviceMap.begin(); iter != audioDeviceMap.end(); ++iter)
		delete (*iter).second;

	audioDeviceMap.clear();
}

//////////////////////////////////////////////////////

bool AudioManager::SetActiveDevice (std::string id)
{
	AudioDevice* audioDevice;

	if((audioDevice = audioDeviceMap[id]) == (AudioDevice*)(0))
		return false;
	else
		return !!BASS_SetDevice(audioDevice->GetDeviceID());
}

//////////////////////////////////////////////////////

AudioDevice* AudioManager::InitialiseNextDevice (std::string id)
{
	AudioDevice* audioDevice;
	AudioDeviceMap::const_iterator iter;

	if(!(audioDevice = new AudioDevice(id)))
		return (AudioDevice*)0;

	if((iter = audioDeviceMap.find(id)) == audioDeviceMap.end())
		audioDeviceMap[id] = audioDevice;
	else {
		delete (*iter).second;
		audioDeviceMap[id] = audioDevice;
	}
	return audioDevice;
}

//////////////////////////////////////////////////////

bool AudioManager::CloseDevice (std::string id)
{
	AudioDeviceMap::iterator iter;

	if((iter = audioDeviceMap.find(id)) == audioDeviceMap.end())
		return false;
	else
	{
		delete (*iter).second;
		audioDeviceMap.erase(iter);
		return true;
	}
}

//////////////////////////////////////////////////////

AudioDevice* AudioManager::GetAudioDevice (std::string id)
{
	AudioDeviceMap::const_iterator iter;

	if((iter = audioDeviceMap.find(id)) == audioDeviceMap.end())
		return (AudioDevice*)(0);
	else
		return (*iter).second;
}

//////////////////////////////////////////////////////

uint8 AudioManager::GetAudioDeviceNum (void)
{
	return uint8(audioDeviceMap.size());
}

//////////////////////////////////////////////////////