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

#ifndef INPUTAGENTEXAMPLE_H
#define INPUTAGENTEXAMPLE_H
#include "micolelib.h"

#include <Node.h>
#include "FF3DDeviceInputAgent.h"
#include "FF3DDeviceOutputAgent.h"
#include "KeyboardInputAgent.h"
#include "TimerAgent.h"

#include "MouseInputAgent.h"

using namespace std;

#include <DeviceInfo.h>
#include <Scene.h>
#include <AutoRef.h>
#include <Display.h>
#include <Shape.h>
#include <Appearance.h>
#include <Sphere.h>
#include <list>

using namespace std;

/**
 * implementation of an input agent for test purpose
 * @warning this class is not part of the api. It's only an example of MicoleApi use.
 */
class MICOLELIB_API InputAgentExample : public KeyboardInputAgent
//public FF3DDeviceInputAgent, public KeyboardInputAgent, public MouseInputAgent, public TimerAgent
{
	public:
		InputAgentExample();

		virtual ~InputAgentExample();
		//virtual void onInput();
//		Vec3f _grab;
		//short int _recording;
		//short int _replay;

		//Vec3f _magneticPoint;

		//list<Vec3f> _path;
		
		//virtual void onButtonDown();
		//virtual void onButtonUp();
		virtual void onKey(int key);
		//virtual void onInput();

		//virtual void onMove(Vec2f pos);
		//virtual void onEvt(string evt);

		//virtual void onTimer();
	private:
		//int _timePassed;
};

#endif
