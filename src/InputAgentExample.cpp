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
#include "InputAgentExample.h"
#include "ReachinMicoleApplication.h"

#include "MicoleStringStream.h"

#include "FF3DPointForceModel.h"

#include <sstream>


using namespace std;

InputAgentExample::InputAgentExample()
{
/*
	_timePassed= 0;
	_recording = 0;
	_replay = 0;*/
}

InputAgentExample::~InputAgentExample(void)
{
}

void InputAgentExample::onKey(int keycode)
{
	switch(keycode)
	{
		// P
		case 112:
			sendMessage("GUIDE PLAY;");
			break;
		// S
		case 115:
			sendMessage("GUIDE STOP;");
			break;
		// L
		case 108:
			sendMessage("GUIDE PAUSE;");
			break;
		// R
		case 114:
			sendMessage("GUIDE REC;");
			break;
		// A
		case 97:
			sendMessage("GUIDE ADD : pos=(0.0, 0.0, 0.0);");
			break;
	}
/*
	if (keycode==114) {
		FF3DDeviceInputAgent::sendMessage("OUT FF3D : none;");
		FF3DDeviceInputAgent::sendMessage("OUT FF3D GUIDE : none;");
		_recording = (!_recording);
		_replay = 0;

		if (_recording) {
			FF3DDeviceInputAgent::sendMessage("OUT VTP : right=(1110100111101001);");
		} else {
			FF3DDeviceInputAgent::sendMessage("OUT VTP : right=(0000000000000000);");
		}
	}
	else
	if (keycode==112) {
		FF3DDeviceInputAgent::sendMessage("OUT FF3D : none;");
		FF3DDeviceInputAgent::sendMessage("OUT FF3D GUIDE : none;");

		_replay = (!_replay);
		_recording = 0;

		if (_path.size() > 0) {
				_magneticPoint = _path.front();
				_path.pop_front();

				MicoleStringStream s;
				s << "OUT FF3D GUIDE : point=("<<_magneticPoint.x<<", "<<_magneticPoint.y<<", "<<_magneticPoint.z<<");";
				FF3DDeviceInputAgent::sendMessage(s.str());


				FF3DDeviceInputAgent::sendMessage("OUT VTP : right=(1110100111101000);");
		} else { //no record
			_magneticPoint = Vec3f(0,0,0);
		}
	}*/
}

#if 0
void InputAgentExample::onInput()
{
	
	if (_recording) {
		_path.push_back(_position);
	}

	if (_replay) {

		Vec3f _diff = _magneticPoint - _position;
		if (_diff.length()<=0.025) {

			if (_path.size() > 0) {
				_magneticPoint = _path.front();
				_path.pop_front();

				MicoleStringStream s;
				s << "OUT FF3D GUIDE : point=("<<_magneticPoint.x<<", "<<_magneticPoint.y<<", "<<_magneticPoint.z<<");";
				FF3DDeviceInputAgent::sendMessage(s.str());
			} else { //no more point in path
				_replay = 0;
				FF3DDeviceInputAgent::sendMessage("OUT FF3D GUIDE : none;");
				FF3DDeviceInputAgent::sendMessage("OUT VTP : left=(0000000000000000);");
				FF3DDeviceInputAgent::sendMessage("OUT VTP : right=(0000000000000000);");
			}
		}
	}

}

void InputAgentExample::onButtonDown() 
{
	/*_grab = Vec3f(_position.x,_position.y,_position.z);
	FF3DDeviceInputAgent::sendMessage("OUT FF3D : none;");
	FF3DDeviceInputAgent::sendMessage("OUT FF3D GUIDE : none;");*/
}

void InputAgentExample::onButtonUp()
{
		//MicoleStringStream s;
		//s << "OUT FF3D : point=("<<_position.x<<", "<<_position.y<<", "<<_position.z<<");";
		//s << "OUT FF3D : sphere=("<<_position.x<<", "<<_position.y<<", "<<_position.z<<", "<< 0.02 <<");";
		//s << "OUT FF3D : line=(("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),("<<_position.x<<", "<<_position.y<<", "<<_position.z<<"));";
		/*s << "OUT FF3D : picobhg=(("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),("<<_position.x<<", "<<_position.y<<", "<<_position.z<<")); bumps=((0, 1, 0),(0, -0.5, 0)); bumplength=0.1; pauselength=0.001;";
		FF3DDeviceInputAgent::sendMessage(s.str());
		s.flush();

		s << "OUT FF3D GUIDE : point=("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<");";
		FF3DDeviceInputAgent::sendMessage(s.str());*/
		//s << "OUT FF3D : plan=(("<<_position.x<<", "<<_position.y<<", "<<_position.z<<"),("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"));";
		//s << "OUT FF3D : disc=(("<<_position.x<<", "<<_position.y<<", "<<_position.z<<"),("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),"<< 0.03 <<");";
		//s << "OUT FF3D : picobhg=(("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),("<<_position.x<<", "<<_position.y<<", "<<_position.z<<")); bumps=((0, 0, 0.005),(0, 0, 0.01),(0, 0.005, 0),(0, 0.01, 0),(0.005, 0, 0),(0.01, 0, 0));";
		//s << "OUT FF3D : picobhg=(("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),("<<_position.x<<", "<<_position.y<<", "<<_position.z<< \
			")); bumps=((0, 0, 0.005),(0, 0, 0.01));";
		//s << "OUT FF3D : picobhg=(("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),("<<_position.x<<", "<<_position.y<<", "<<_position.z<<")); bumps=((0, 0.005, 0),(0, 0.01, 0));";
		//s << "OUT FF3D : picobhg=(("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),("<<_position.x<<", "<<_position.y<<", "<<_position.z<<")); bumps=((0.005, 0, 0),(0.01, 0, 0));";
	
}


void InputAgentExample::onMove(Vec2f pos)
{
		MicoleStringStream ss;
		ss << "OUT FF3D GUIDE : point=("<<_pos.x<< ", " << _pos.y << ", 0.0);";
		MouseInputAgent::sendMessage(ss.getString().c_str());
}

void InputAgentExample::onEvt(string evt)
{
	if(evt.compare("BT1_PRESSED")==0)
	{
	}
}

void InputAgentExample::onTimer()
{
	/*MicoleStringStream ss;
	ss << "Net: elapsed time: "<<_timePassed++;
		MouseInputAgent::sendMessage(ss.getString().c_str());*/
}
#endif
