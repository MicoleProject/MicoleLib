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
#include "MouseInputAgent.h"
#include "ReachinMicoleApplication.h"
#include <MicoleStringStream.h>

MouseInputAgent::MouseInputAgent()
{
	bindMessage( "^IN MSE : pos=\\((.*), (.*)\\); evt=\\((.*)\\);$", BUS_CALLBACK_OF(MouseInputAgent, handleMouseMove));
	bindMessage( "^IN MSE : pos=\\((.*), (.*)\\);$", BUS_CALLBACK_OF(MouseInputAgent, handleMouseMove));
}

MouseInputAgent::~MouseInputAgent()
{
}

void MouseInputAgent::handleMouseMove(MicoleBus *app, int argc, const char **argv)
{
	if(argc>=2)
	{
		_pos.x = atof(argv[0]);
		_pos.y = atof(argv[1]);
		onMove(_pos.x,_pos.y);
	} 

	if(argc==3) 
	{
		string btn_stat = string(argv[2]);
		if(btn_stat.compare("BT1_PRESSED")==0)
			onButtonDown(1);
		else if(btn_stat.compare("BT1_RELEASED")==0)
			onButtonDown(1);

		if(btn_stat.compare("BT2_PRESSED")==0)
			onButtonDown(2);
		else if(btn_stat.compare("BT2_RELEASED")==0)
			onButtonDown(2);
	}
}
