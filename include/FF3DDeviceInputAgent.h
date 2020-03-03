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

#ifndef FF3DDEVICEINPUTAGENT_H
#define FF3DDEVICEINPUTAGENT_H
#include "micolelib.h"

#include "MicoleAgent.h"
#include <LinearAlgebra.h> 



using namespace Reachin;
/** @addtogroup FF3D
 *  @{
 */

/** @defgroup input Input classes
 *  Input helper classes 
 *  @{
 */

/*!
 * \brief
 * helper and autobind of FF3DDeviceOutputAgent messages
 * 
 * plugged on the micole bus, FF3DDeviceInputAgent provide auto parsing of "IN FF3D :" messages.
 * Inherited class can define onInput, onButtonDown and onButtonUp function
 * 
 */
class MICOLELIB_API FF3DDeviceInputAgent: public MicoleAgent
{
	public:
		/**
		 * default constructor
		 */
		FF3DDeviceInputAgent(void);

		/**
		 * default destructor
		 */
		virtual ~FF3DDeviceInputAgent(void);

		/**
		 * handle input store position and button state and call callback functions if needed
		 */
		void handleInput( MicoleBus *app, int argc, const char **argv );

		/**
		 * onInput is called after each update of position
		 */
		virtual void onInput() {} 
		/**
		 * onButtonDown is called after each "button down" event
		 */
		virtual void onButtonDown() {}
		/**
		 * onButtondown is called after each time you release 3D device button
		 */
		virtual void onButtonUp() {}

	protected:
		Vec3f _position; /**< FF3D Device position*/
		bool _buttonPressed; /**< Button state */
};

/** @} */ // end of input
/** @} */ // end of FF3D

#endif