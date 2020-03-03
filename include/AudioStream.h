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

#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <bass.h>

#include "util.h"

#define SPEAKER(N) (((N) > 30 || (N) < 1) ? ((N) > 30 ? 30 : 1) : (1 << (N-1)))

// For 7.1 sound cards
//
#define FRONT_SPEAKERS	0x00000000
#define REAR_SPEAKERS	0x40000000
#define SPEAKER_CENLFE	0x80000000
#define SPEAKER_REAR2	0xC0000000

//! Audio stream class
class AudioStream {

	private:

		HSTREAM				handle;
		BASS_CHANNELINFO	info;
		uint32				speakers;

		uint64				size;
		uint32				freq;
		uint32				vol;
		sint32				pan;
		bool				looped;

		uint32	AssignSpeakers (uint32 speakers);

	public:

		//! Creates a new stream.
		/*!
		\param filename location in filesystem.
		\param speakers speakers (see SPEAKER(N) macro).
		\param mono play the stream in mono.
		\param looped loop the file.
		*/
		AudioStream (const char* filename, uint32 speakers, bool mono = true, bool looped = false);

		//! Destroys this stream
		~AudioStream (void);

		//! Starts (or resumes) playback of this stream.
		/*!
		\param restart true if you want to restart playback from the beginning (the default value is false).
		*/
		void	Play (bool restart = false);

		//! Pauses this sample.
		void	Pause (void);

		//! Stops this sample.
		void	Stop (void);

		//! Retrieves the size of the channel in bytes.
		uint64	GetSize (void);

		//! Retrieves the length of the stream in seconds. 
		int		GetLength (void);

		//! Retrieves the remaining time in seconds.
		float	GetTimeRemaining (void);

		//! Retrieves the elapsed time in seconds.
		float	GetTimeElapsed (void);

		//! Sets the position of the stream.
		/*!
		\param sec the position is seconds.
		*/
		void	SetPosition (float sec);

		//! Assigns this stream to speakers
		/*!
		\param speakers the assigned speakers.
		*/
		void	SetSpekears (uint32 speakers);

		//! Sets the sample rate (frequency) of this stream.
		/*!
		\param freq the sample rate of this stream.
		*/
		void	SetFrequency (uint32 freq);

		//! Retrieves the sample rate of this stream.
		uint32	GetFrequency (void);

		//! Sets the volume.
		/*!
		\param vol the volume (0 - 10000)
		*/
		void	SetVolume (uint16 vol);

		//! Retrieves the volume.
		uint16	GetVolume (void);

		//! Sets the panning position.
		/*!
		\param pan the panning position (-100 - 100)
		*/
		void	SetPanning (sint8 pan);

		//! Retrieves the panning position
		sint8	GetPanning (void);

		void	PrintChannelAttributes (void);
};

#endif // Do not add stuff beyond this point.