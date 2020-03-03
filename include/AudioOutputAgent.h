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

#ifndef AUDIOOUTPUTAGENT_H
#define AUDIOOUTPUTAGENT_H

#include <assert.h>
#include <SpatialAudio.h>
#include "micolelib.h"
#include "MicoleAgent.h"

static const char* PAUSE_ALL = "PAUSE_ALL";
static const char* STOP_ALL = "STOP_ALL";
static const char* RESUME_ALL = "RESUME_ALL";

enum MICOLELIB_API SoundType {

	PLAY=1,
	PLAY_INTERRUPT,
	PLAY_INTERRUPT_ALL,
	LOOP,
	LOOP_INTERRUPT,
	LOOP_INTERRUPT_ALL,
	PAUSE,
	STOP,
	RESUME,
	
};

/**
*	AudioOutputAgent plays audio files. The AudioOutputAgent receives messages
*	of form 
*	
*	"OUT SND : name=filename.wav;type=1;" 
*	
*	The parameter type is always accompanied with parameter name. Type tells what to
*	do with the file. See enum SoundType above. Possibilities are:
*	- PLAY: Plays the file, does not interrupt.
*	- PLAY_INTERRUPT: Interrupts playing of filename.wav (if it is already playing.)
*	- PLAY_INTERRUPT_ALL: Interrupts all other sounds.
*	- LOOP, LOOP_INTERRUPT, LOOP_INTERRUPT_ALL: Play looping sound, respectively.
*	- PAUSE: Pauses playing filename.wav
*	- STOP: Stops playing filename.wav
*	- RESUME: Resumes playing filename.wav after PAUSE
*
*	"OUT SND : STOP_ALL|PAUSE_ALL|RESUME_ALL"	
*
*	- STOP_ALL, PAUSE_ALL: Stop or pause all sounds
*	- RESUME_ALL: Resume all sounds after PAUSE_ALL
*/

class MICOLELIB_API AudioOutputAgent:public MicoleAgent{
//class AudioOutputAgent:public MicoleAgent{


public:
	void handleAudioMessage( MicoleBus *app, int argc, const char **argv);
	void handleStopPauseAllMessage(MicoleBus *app, int argc, const char **argv);
	AudioOutputAgent(const string baseDir);
	~AudioOutputAgent();

protected:
	AudioDevice* device;
	string soundBase;
	bool stopped;

};

#endif