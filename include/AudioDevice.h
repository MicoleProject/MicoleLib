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

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#pragma warning(disable:4786)

#include <string>
#include <map>
#include <bass.h>

#include "AudioStream.h"
#include "util.h"

typedef std::map<std::string, AudioStream*> AudioStreamMap;

//! Audio device class.
class AudioDevice {

	private:

		static uint8 nextDevice;
		static uint8 activeDevice;

		AudioStreamMap audioStreamMap;
		std::string id;
		uint8 currDevice;
		BASS_INFO info;

		uint16 vol;

	public:

		//! Creates a new audio device.
		AudioDevice (std::string id);

		//! Destroys this audio device.
		~AudioDevice (void);

		//! Creates a new stream which is assigned to this device.
		/*!
		\param id the identifier of the newly created stream.
		\param filename location the filename.
		\param speakers speakers (see SPEAKER(N) macro).
		\param mono play the steam in mono.
		\param looped loop the file.
		*/
		AudioStream*	CreateStream (std::string id, const char* filename, uint32 speakers, bool mono = true, bool looped = false);

		//! Retrieves a stream from this device.
		/*!
		\param id the identifier of the stream which will be retrieved.
		*/
		AudioStream*	GetStream (std::string id);

		//! Removes the specified stream from this device.
		/*!
		\param id the identifier of the stream which will be removed.
		*/
		void			RemoveStream (std::string id);

		//! Sets the output volume for this device.
		/*!
		\param volume the volume level (0 - 10000).
		*/
		void			SetVolume (uint16 volume);

		//! Retrieves the output volume for this device.
		uint16			GetVolume (void);

		//! Retrieves the handle of this device
		uint8			GetDeviceID (void);

		//! Retrieves the text description of this device
		std::string		GetDeviceDescription (void);

		//! Starts or resumes the output of this device.
		/*!
		The output is automatically started by constructor,
		so there is no need for using this function unless
		the output is paused or stopped.
		*/
		void			Start (void);

		//! Pauses the output of this device.
		void			Pause (void);

		//! Stops the output of this device.
		void			Stop (void);

		//! Retrieves the number of speakers the device supports.
		uint8			GetSpeakersNum (void);
};

#endif // Do not add stuff beyond this point