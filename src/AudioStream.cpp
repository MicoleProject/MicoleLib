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
#include <iostream>
using namespace std;

#include <bass.h>
#include <cassert>

#include "AudioStream.h"

#define MAX_SPEAKER_NUM	30

//////////////////////////////////////////////////////

AudioStream::AudioStream (const char* filename, uint32 speakers, bool mono, bool looped)
{
	assert(filename);

	uint32 BASS_Speakers = AssignSpeakers(speakers);
	uint32 flags =	(looped ? BASS_SAMPLE_LOOP : 0) |
					(mono ? BASS_SAMPLE_MONO : 0) |
					BASS_Speakers;

	handle = BASS_StreamCreateFile(false, filename, 0, 0, flags);

	assert(BASS_ErrorGetCode() == BASS_OK);
	size = BASS_ChannelGetLength(handle);
	BASS_ChannelGetInfo(handle, &info);
	BASS_ChannelGetAttributes(handle, (unsigned long*)&freq, (unsigned long*)&vol, &pan);
	assert(BASS_ErrorGetCode() == BASS_OK);
}

//////////////////////////////////////////////////////

AudioStream::~AudioStream (void)
{
	BASS_StreamFree(handle);
}

//////////////////////////////////////////////////////

uint32 AudioStream::AssignSpeakers (uint32 speakers)
{
	uint32 BASS_Speakers = 0;
	uint8 i;

	switch(speakers & 0xFFFFFFFF) {
		case 0x0:
			BASS_Speakers = BASS_SPEAKER_FRONT;
			break;
		case 0x40000000:
			BASS_Speakers = BASS_SPEAKER_REAR;
			break;
		case 0x80000000:
			BASS_Speakers = BASS_SPEAKER_CENLFE;
			break;
		case 0xC0000000:
			BASS_Speakers = BASS_SPEAKER_REAR2;
			break;
		default:
			for(i = 1; i <= MAX_SPEAKER_NUM; ++i) {
				if(speakers & SPEAKER(i)) {
					BASS_Speakers = (BASS_SPEAKER_N((i+1)/2) |
									(i%2 ? BASS_SPEAKER_LEFT : BASS_SPEAKER_RIGHT));
					break;
				}
			}
	}
	return BASS_Speakers;
}

//////////////////////////////////////////////////////

void AudioStream::Play (bool restart) {

	BASS_ChannelPlay(handle, restart);
	assert(BASS_ErrorGetCode() == BASS_OK);
}

//////////////////////////////////////////////////////

void AudioStream::Pause (void) {

	BASS_ChannelPause(handle);
}

//////////////////////////////////////////////////////

void AudioStream::Stop (void) {

	BASS_ChannelStop(handle);
}

//////////////////////////////////////////////////////

uint64 AudioStream::GetSize (void)
{
	return size;
}

//////////////////////////////////////////////////////

int AudioStream::GetLength (void)
{
	return int(BASS_ChannelGetLength(handle));
}

//////////////////////////////////////////////////////

float AudioStream::GetTimeRemaining (void)
{
	return (GetLength() - GetTimeElapsed());
}

//////////////////////////////////////////////////////

float AudioStream::GetTimeElapsed (void)
{
	return BASS_ChannelBytes2Seconds(handle, BASS_ChannelGetPosition(handle));
}

//////////////////////////////////////////////////////

void AudioStream::SetPosition (float sec)
{
	uint16 len = uint16(BASS_ChannelGetLength(handle));
	if(sec > 0 && sec < len)
		BASS_ChannelSetPosition(handle, BASS_ChannelSeconds2Bytes(handle, sec));
}

//////////////////////////////////////////////////////

void AudioStream::SetSpekears (uint32 speakers)
{
	uint32 BASS_Speakers = AssignSpeakers(speakers);
	info.flags &= ~0x3F000000; // clear all speaker flags.
	info.flags |= BASS_Speakers;
	BASS_ChannelSetFlags(handle, info.flags);
}

//////////////////////////////////////////////////////

void AudioStream::SetFrequency (uint32 freq)
{
	if(BASS_ChannelSetAttributes(handle, freq, -1, -101))
		this->freq = freq;
}

//////////////////////////////////////////////////////

uint32 AudioStream::GetFrequency (void)
{
	return freq;
}

//////////////////////////////////////////////////////

void AudioStream::SetVolume (uint16 vol)
{
	if(BASS_ChannelSetAttributes(handle, -1, vol, -101))
		this->vol = vol;
}

//////////////////////////////////////////////////////

uint16 AudioStream::GetVolume (void)
{
	return vol;
}

//////////////////////////////////////////////////////

void AudioStream::SetPanning (sint8 pan)
{
	if(BASS_ChannelSetAttributes(handle, -1, -1, pan))
		this->pan = pan;
}

//////////////////////////////////////////////////////

sint8 AudioStream::GetPanning (void)
{
	return pan;
}

//////////////////////////////////////////////////////

void AudioStream::PrintChannelAttributes (void)
{
	cerr << "============= Audio Stream: " << handle << " =========" << endl;
	cerr << "Frequency: " << freq << endl << "Volume: " << vol << endl << "Panning:" << pan << endl;
	cerr << "freq: " << info.freq << endl << "Channels:" << info.chans << endl;
	cerr << "length: " << BASS_ChannelGetLength(handle) << endl << "size:" << size << endl;
}

//////////////////////////////////////////////////////