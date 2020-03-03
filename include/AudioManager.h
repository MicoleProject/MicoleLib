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

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#pragma warning(disable:4786)

#include <map>
#include <string>

#include <bass.h>

#include "AudioDevice.h"

typedef std::map<std::string, AudioDevice*> AudioDeviceMap;
typedef std::map<int, std::string> ErrorCodeMap;

//! Audio manager class
/*!
Singleton class.
*/
class AudioManager {

	private:

		static AudioDeviceMap audioDeviceMap;
		static ErrorCodeMap errorCodeMap;

		static void FillUpErrorCodeMap (void);
		static void CheckError (void);

	public:

		//! Initialization function.
		/*!
		This function must be called before using any other functions.
		*/
		__declspec(dllexport) static void Initialise (void);

		//! Clean up function.
		/*!
		Frees all resources.
		*/
		static void CleanUp (void);

		//! Initializes an audio device.
		/*!
		\param id the identifier of the device which will be initialized.
		*/
		static AudioDevice*	InitialiseNextDevice (std::string id);

		//! Closes an audio device.
		/*!
		\param id the identifier of the device which will be closed.
		*/
		static bool	CloseDevice (std::string id);

		//! Retrieves an audio device.
		/*!
		\param id the identifier of the device which will be retrieved.
		*/
		static AudioDevice* GetAudioDevice (std::string id);

		//! Returns the number of the initialised audio devices.
		static uint8 GetAudioDeviceNum (void);

		//! Sets an audio device to use for subsequent calls.
		/*!
		\param id the identifier of the device
		*/
		static bool	SetActiveDevice (std::string id);
};

#endif // Do not add stuff beyond this point