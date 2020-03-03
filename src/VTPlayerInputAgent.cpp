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
#include "VTPlayerInputAgent.h"

VTPlayerInputAgent::VTPlayerInputAgent()
{
	bindMessage( "^IN VTP : pos=\\((.*), (.*)\\);$", BUS_CALLBACK_OF(VTPlayerInputAgent, handleInputCoord ));
	bindMessage( "^IN VTP : press=\\((.*)\\);$", BUS_CALLBACK_OF(VTPlayerInputAgent, handleInputPressButton  ));
	bindMessage( "^IN VTP : release=\\((.*)\\);$", BUS_CALLBACK_OF(VTPlayerInputAgent, handleInputReleaseButton ));
}

VTPlayerInputAgent::~VTPlayerInputAgent()
{
}

void VTPlayerInputAgent::handleInputCoord ( MicoleBus *app, int argc, const char **argv )
{
	if (argc>=2)
	{
		int x = atoi(argv[0]);
		int y = atoi(argv[1]);
		onMove(x,y);
	}
}

void VTPlayerInputAgent::handleInputPressButton ( MicoleBus *app, int argc, const char **argv )
{
	if (argc>=1)
	{
		int button = atoi(argv[0]);
		onButtonDown(button);
	}
}

void VTPlayerInputAgent::handleInputReleaseButton ( MicoleBus *app, int argc, const char **argv )
{
	if (argc>=1)
	{
		int button = atoi(argv[0]);
		onButtonUp(button);
	}
}



void VTPlayerInputAgent::onMove(const int &posx, const int &posy)
{
}

void VTPlayerInputAgent::onButtonDown(const int &button)
{
}

void VTPlayerInputAgent::onButtonUp(const int &button)
{
}
