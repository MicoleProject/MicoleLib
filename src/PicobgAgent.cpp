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

#include "StdAfx.h"
#include "..\include\PicobgAgent.h"

PicobgAgent::PicobgAgent(void): TimerAgent("PicobgAgent","ThreadedAgent",5)
{
	_timestamp = 0;
}

PicobgAgent::~PicobgAgent(void)
{
	
}

void PicobgAgent::onButtonDown()
{
	/*ostringstream s;
	s << "OUT FF3D : point=("<<_position.x<<", "<<_position.y<<", "<<_position.z<<");";
		MicoleAgent::sendMessage(s.str());*/
	_initPos = _position;
	_timestamp = getElapsedTime();
}

void PicobgAgent::onTimer()
{
	if (_timestamp) {

		ostringstream s;

		Vec3f _finalPos = _initPos;
		Vec3f deviation = Vec3f(0.1,0,0)* ((double)(getElapsedTime()-_timestamp)/1000);

		_finalPos +=deviation;


		s << "OUT FF3D : point=("<<_finalPos.x<<", "<<_finalPos.y<<", "<<_finalPos.z<<");";
		//s << "OUT FF3D : sphere=("<<_finalPos.x<<", "<<_finalPos.y<<", "<<_finalPos.z<<", "<< 0.01 <<");";
		//s << "OUT FF3D : line=(("<<_finalPos.x<<", "<<_finalPos.y<<", "<<_finalPos.z<<"),("<<(_finalPos.x+1)<<", "<<(_finalPos.y)<<", "<<_finalPos.z<<"));";
		//s << "OUT FF3D : plan=(("<<_position.x<<", "<<_position.y<<", "<<_position.z<<"),("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"));";
		//s << "OUT FF3D : disc=(("<<_position.x<<", "<<_position.y<<", "<<_position.z<<"),("<<_grab.x<<", "<<_grab.y<<", "<<_grab.z<<"),"<< 0.03 <<");";
		MicoleAgent::sendMessage(s.str());
		if ((getElapsedTime()-_timestamp)>2000) 
		{
			_timestamp = 0;
			MicoleAgent::sendMessage("OUT FF3D : none;");
		}
	}
}