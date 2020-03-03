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

//reachin in test

// ugly patch against the inclusion loop problem...
#include "stdafx.h"

#include "RegistryAgent.h"
#include "TimerAgent.h"
#include "LoggerAgent.h"
#include "ReachinMicoleApplication.h"
#include "FF3DDeviceSenderAgent.h"
#include "FF3DDeviceInputAgent.h"
#include "FF3DDeviceOutputAgent.h"
#include "FF3DGuidanceAgent.h"

#include "KeyboardSenderAgent.h"

#include "InputAgentExample.h"
#include "PicobgAgent.h"

#include "VTPlayerSenderAgent.h"
#include "VTPlayerInputAgent.h"
#include "VTPlayerOutputAgent.h"

#include "MouseSenderAgent.h"
#include "MouseInputAgent.h"

#include "GuidanceAgent.h"

#include <Vrml.h>
#include <Display.h>
#include <Scene.h>
#include <Node.h>
#include <Field.h>

#include <iostream>

using namespace Reachin;
using namespace std;

int main(int argc, char* argv[])
{
	ReachinMicoleApplication * ama = ReachinMicoleApplication::getInstance();
	//ama->startNetwork();

	FF3DGuidanceAgent ga;

	ama->sendBusMessage("Set stiffness = 1.5;");
	ama->sendBusMessage("Set d = 0.02;");

	LoggerAgent la;

	//AbstractMicoleApplication * ama = AbstractMicoleApplication::getInstance();
	InputAgentExample ia;

	//FF3DDeviceInputAgent * ia = new PicobgAgent();

	//AdditionAgent a;
	//MultiplicationAgent m;
	//HelloAgent ha("Hello");

/*	VTPlayerSenderAgent * vtsa = new VTPlayerSenderAgent();
	VTPlayerInputAgent * vtia = new VTPlayerInputAgent();

	VTPlayerOutputAgent * vtoa = new VTPlayerOutputAgent();*/

	KeyboardSenderAgent ksa;

	FF3DDeviceOutputAgent oa = FF3DDeviceOutputAgent();

	//ama->sendBusMessage("OUT FF3D : point=(0,0,0.01);");
	//ama->sendBusMessage("OUT FF3D : line=((0, 0, 0),(0.2, 0.2, 0.2));");
	//ama->sendBusMessage("OUT FF3D : plan=((0, 0, 0),(1, 3, 3));");

//	Sleep(1000);

	/*while(1){
		ama->sendBusMessage("Time");
	}*/

	FF3DDeviceSenderAgent sa = FF3DDeviceSenderAgent(8);
	MouseSenderAgent msa = MouseSenderAgent(); 


	//ama->sendBusMessage("OUT VTP : right=(0001000100010001);");
	//ama->sendBusMessage("OUT VTP : left=(0001000100010001,0010001000100010,0100010001000100,1000100010001000); dur=200;");

	//Sleep(1000);

	//ama->sendBusMessage("Net: OUT VTP : right=(0000000000001111,0000000011110000,0000111100000000,1111000000000000); dur=100;");

	GuidanceAgent gagent;

	//ama->sendIvyMessage("Net: Test network");
	ama->startScene();

	//Sleep(100000);
}
