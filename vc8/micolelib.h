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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MICOLELIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MICOLELIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifndef __MICOLELIB_API_TEST__

#ifdef MICOLELIB_EXPORTS
#define MICOLELIB_API __declspec(dllexport)
#else
#define MICOLELIB_API __declspec(dllimport)
#endif

#else

#define MICOLELIB_API 

#endif

#pragma warning (disable : 4251)

#define IvyApplication MicoleBus 

/** @defgroup devices devices
 * Supported devices in MicoleLib
 * MicoleLib can handle several devices. Of course Keyboard and Mouse but you can also handle haptic devices like VTPlayer or FF3D ForceFeeback device configured in ReachinAPI.
 * All devices classes was separated in three parts. Input/Ouput and Sender.
 * All agents named sender send devices state information on the bus. For example the KeyboardSenderAgent sends the key pressed and released. These agents have no bind on the bus but "quit".

Agents named input will catch generic messages from sender agents. Input agents are helpers for developers: they bind "standard messages" from the related sender agent. To help development, we highly recommend to inherit the corresponding input agent when we want informations from a device.

Output agents handle messages to render incoming informations on the output devices. 
 */

/** @defgroup FF3D ForceFeedBack 3d Device
 *  @ingroup devices
 *  classes grouping all FF3D Device control classes
 */

/** @defgroup vtplayer VTPlayer device
 *  @ingroup devices
 *  classes grouping all VTPlayer control classes
 */

/** @defgroup sender SenderAgent 
 *  Send information on the bus.
 */

/** @defgroup keyboard Keyboard
 *  @ingroup devices
 */

/** @defgroup mouse Mouse
 *  @ingroup devices
 */
 
/** @defgroup network Network
 *  @ingroup core
 */

