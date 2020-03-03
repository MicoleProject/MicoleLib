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
#include "GuidanceAgent.h"

#include "MicoleStringStream.h"

GuidanceAgent::GuidanceAgent(const float &replayDistance, const float &_recordDistance)
:_state(STOPPED), _replayDistance(replayDistance), _recordDistance(_recordDistance)
{
	_pointListIterator = _pointlist.begin();
	bindMessage("^GUIDE PLAY;", BUS_CALLBACK_OF(GuidanceAgent, handlePlay));
	bindMessage("^GUIDE STOP;", BUS_CALLBACK_OF(GuidanceAgent, handleStop));
	bindMessage("^GUIDE PAUSE;", BUS_CALLBACK_OF(GuidanceAgent, handlePause));
	bindMessage("^GUIDE REC;", BUS_CALLBACK_OF(GuidanceAgent, handleRecord));
	bindMessage("^GUIDE ADD : pos=\\((.*), (.*), (.*)\\);", BUS_CALLBACK_OF(GuidanceAgent, handleAdd));
}

GuidanceAgent::~GuidanceAgent()
{
}

void GuidanceAgent::onInput()
{
	Vec3f p;
	if (!_pointlist.empty())
		p = _position - (*_pointListIterator);;
	
	switch(_state)
	{
		// change the point if the current position is close to the
		// current point
		case PLAYING:
			if (_pointlist.empty())
				return;
			if (p.length() < _replayDistance)
			{
				_pointListIterator++;
				if (_pointListIterator == _pointlist.end())
				{
					sendMessage("OUT FF3D GUIDE : none;");
					_state = STOPPED;
					_pointListIterator = _pointlist.begin();
					return;
				}
				MicoleStringStream s;
				s << "OUT FF3D GUIDE : point=(" 
					<< (*_pointListIterator).x << ", " 
					<< (*_pointListIterator).y << ", " 
					<< (*_pointListIterator).z << ");";
				sendMessage(s.getString());
			}
			break;
		// add the current position if the position is far enough
		// from the last point added
		case RECORDING:
			if (_pointlist.empty() || p.length() > _recordDistance)
			{
				_pointlist.push_front(_position);
				_pointListIterator = _pointlist.begin();
			}
			break;
		default:
			break;
	}
}

// add a point in the list
void GuidanceAgent::handleAdd( MicoleBus *app, int argc, const char **argv )
{
	if (argc < 3)
		return;
	_pointListIterator = _pointlist.insert(_pointListIterator, Vec3f(atof(argv[0]), atof(argv[1]), atof(argv[2])));
	MicoleStringStream s;
	s << "OUT FF3D GUIDE : point=("
		<< atof(argv[0]) << ", " 
		<< atof(argv[1]) << ", " 
		<< atof(argv[2]) << ");";
	sendMessage(s.getString());
}

// play the recording or resumes
void GuidanceAgent::handlePlay( MicoleBus *app, int argc, const char **argv )
{
	_state = PLAYING;
	_pointListIterator = _pointlist.begin();
	if (_pointlist.empty())
		return;
	MicoleStringStream s;
	s << "OUT FF3D GUIDE : point=("
		<< (*_pointListIterator).x << ", " 
		<< (*_pointListIterator).y << ", " 
		<< (*_pointListIterator).z << ");";
	sendMessage(s.getString());
}

// pauses the recording
void GuidanceAgent::handlePause( MicoleBus *app, int argc, const char **argv )
{
	_state = PAUSED;
	sendMessage("OUT FF3D GUIDE : none;");
}

// stop the playing or the recording
void GuidanceAgent::handleStop( MicoleBus *app, int argc, const char **argv )
{
	_state = STOPPED;
	_pointListIterator = _pointlist.begin();
	sendMessage("OUT FF3D GUIDE : none;");
}

// starts the recording
void GuidanceAgent::handleRecord( MicoleBus *app, int argc, const char **argv )
{
	_state = RECORDING;
	_pointlist.clear();
	_pointListIterator = _pointlist.begin();
}
