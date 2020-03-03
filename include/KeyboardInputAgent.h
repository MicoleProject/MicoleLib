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

#ifndef KeyboardInputAgent_H
#define KeyboardInputAgent_H

#include "MicoleAgent.h"
#include "micolelib.h"

/** @defgroup keyboard Keyboard
 *  Keyboard control classes
 *  @{
 */

/** @defgroup input Input classes
 *  Input helper classes 
 *  @{
 */


class MICOLELIB_API KeyboardInputAgent : public MicoleAgent
{
	public:
		KeyboardInputAgent();
		~KeyboardInputAgent();

		void handleKey ( MicoleBus *app, int argc, const char **argv );

		virtual void onKey(int keycode) = 0;
};

/** @} */ //end input
/** @} */ //end keyboard

#endif
