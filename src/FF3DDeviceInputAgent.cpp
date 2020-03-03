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
#include "FF3DDeviceInputAgent.h"
#include <stdlib.h>
/*#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>*/

FF3DDeviceInputAgent::FF3DDeviceInputAgent()
: MicoleAgent("FF3DDeviceInputAgent","INPUT"), _buttonPressed(true)
{
	bindMessage("^IN FF3D : pos=\\((.*), (.*), (.*)\\);( evt=(.*);)?$", BUS_CALLBACK_OF(FF3DDeviceInputAgent, handleInput));
}

FF3DDeviceInputAgent::~FF3DDeviceInputAgent()
{
}

void FF3DDeviceInputAgent::handleInput (MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3)
		return;
	_position.x = atof(argv[0]);
	_position.y = atof(argv[1]);
	_position.z = atof(argv[2]);

	if (argc > 3)
	{
		if(!strcmp(argv[4],"PRESSED"))
		{
			if (_buttonPressed == false) //if we not press it
			{
				_buttonPressed = true;
				onButtonDown();
			}
		}
		else
		{
			if (_buttonPressed == true) //if we press it
			{
				_buttonPressed = false;
				onButtonUp();
			}
		}
	}
	else
	{
		if (_buttonPressed == true) //if we not press it
		{
				_buttonPressed = false;
				onButtonUp();
		}
	}

	onInput();
}
