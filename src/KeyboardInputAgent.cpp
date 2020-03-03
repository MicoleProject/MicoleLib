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
#include "KeyboardInputAgent.h"

KeyboardInputAgent::KeyboardInputAgent()
:MicoleAgent("KeyboardInputAgent","InputAgent")
{
	bindMessage( "^IN KBD : key=(.*);", BUS_CALLBACK_OF(KeyboardInputAgent, handleKey ));
}

KeyboardInputAgent::~KeyboardInputAgent()
{
}

void KeyboardInputAgent::handleKey (MicoleBus *app, int argc, const char **argv)
{
	if (argc >= 1)
	{
		int key = atoi(argv[0]);
		onKey(key);
	}
}
