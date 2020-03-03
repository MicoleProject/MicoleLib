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
#include "FF3DDeviceSenderAgent.h"
#include "ReachinMicoleApplication.h"
#include "MicoleAgentThread.h"
#include "MicoleStringStream.h"


FF3DDeviceSenderAgent::FF3DDeviceSenderAgent(const int &pDelay)
: MicoleAgentThread("FF3DDeviceSenderAgent","SenderAgent"), _FF3DCoord(new FF3DCoord), _buttonPress(new ButtonPress), _delay(pDelay)
{
	Display * dis = ReachinMicoleApplication::getInstance()->getDisplay();
	
	dis->tracker_pos->route( _FF3DCoord );
	dis->button->route( _buttonPress );

	start(); //Start thread*/
}

FF3DDeviceSenderAgent::~FF3DDeviceSenderAgent()
{}

void FF3DDeviceSenderAgent::run ()
{
	Vec3f p = Vec3f(0,0,0);
	MicoleStringStream s;
	while(getState()==ACTIVE)
	{
		p = _FF3DCoord->get();

		if (p.x < 100 && p.x > -100) 
		{ //if reachin doesn't start
			//value can be really big
			//bus packet example: "IN PHANTOM : (pos=(4.5, 5.6, 2.3));"
			s<< "IN FF3D : pos=(" << p.x << ", "<< p.y << ", " << p.z<<");";

			if (_buttonPress->eventPressed())
				s << " evt=PRESSED;";


			if (_buttonPress->eventReleased())
				s << " evt=RELEASED;";


			sendMessage(s.str());
		}
		::Sleep(_delay);
		//Sleep(_delay);
		s.flush();
	}
}

void FF3DDeviceSenderAgent::ButtonPress::evaluate( SFBool _pressed )
{
	//we skip if the time difference between the last click event is too small
	pressed = !pressed;
	skip = !((Time(last_event + BUTTON_LAG)).inPast());
	last_event.setToNow();
	value = (!skip && pressed);
}

bool FF3DDeviceSenderAgent::ButtonPress::eventPressed()
{
	return (pressed && !skip);
}

bool FF3DDeviceSenderAgent::ButtonPress::eventReleased()
{
	return (!pressed && !skip);
}

void FF3DDeviceSenderAgent::prepareToStop(int argc, const char **argv)
{
}

void FF3DDeviceSenderAgent::prepareToSuspend(int argc, const char **argv)
{
}
