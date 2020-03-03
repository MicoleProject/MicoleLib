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

#ifndef __GUIDANCEAGENT__
#define __GUIDANCEAGENT__
#include "micolelib.h"

#include "FF3DDeviceInputAgent.h"
#include <LinearAlgebra.h> 

#include <list>

enum GuidanceState {PLAYING, PAUSED, STOPPED, RECORDING};

/**
 * This agent manages a waypoints list. The list can be recorded, 
 * played, paused and stopped. Points can be added manually too.
 */
class MICOLELIB_API GuidanceAgent: public FF3DDeviceInputAgent
{
	public:
		/**
		 * default constructor
		 */
		GuidanceAgent(const float &replayDistance = 0.02, const float &_recordDistance = 0.001);
		/**
		 * default destructor
		 */
		~GuidanceAgent();
		
		void onInput();
		
		void handleAdd( MicoleBus *app, int argc, const char **argv );
		void handlePlay( MicoleBus *app, int argc, const char **argv );
		void handlePause( MicoleBus *app, int argc, const char **argv );
		void handleStop( MicoleBus *app, int argc, const char **argv );
		void handleRecord( MicoleBus *app, int argc, const char **argv );
		
	private:
		GuidanceAgent(const GuidanceAgent &);
		
		list<Vec3f> _pointlist; /**< list of points for the playback */
		list<Vec3f>::iterator _pointListIterator; /**< to navigate through the list */
		GuidanceState _state;
		float _replayDistance;
		float _recordDistance;
};

#endif
