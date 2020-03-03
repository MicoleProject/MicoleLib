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

#ifndef PICOBGAGENT_H
#define PICOBGAGENT_H
#include "TimerAgent.h"
#include "FF3DDeviceInputAgent.h"

class PicobgAgent :
	public TimerAgent, public FF3DDeviceInputAgent
{
public:
	PicobgAgent(void);
	Vec3f _initPos;
	int _timestamp;
public:
	virtual ~PicobgAgent(void);
	virtual void onButtonDown();
	virtual void onTimer();
};

#endif