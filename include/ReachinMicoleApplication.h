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

#pragma once
#include "micolelib.h"

#include <config.h>

#include "AbstractMicoleApplication.h"

#include <Vrml.h>

#include <DeviceInfo.h>
#include <Scene.h>
#include <AutoRef.h>
#include <Display.h>

using namespace Reachin;

class MICOLELIB_API ReachinMicoleApplication: public AbstractMicoleApplication//, public AutoRefBase
{
	public:
	
		static ReachinMicoleApplication * getInstance()
		{
			if (_riInstance == NULL && _instance == NULL)
			{
				_riInstance = new ReachinMicoleApplication;
				_instance = _riInstance;
			}
			return _riInstance;
		}
		
		virtual ~ReachinMicoleApplication();

		virtual void startScene();
		Display * getDisplay() { return _display.get(); }

		//void setDisplay(Display * pDis) { _display = pDis; }
	private:
		ReachinMicoleApplication();
		static ReachinMicoleApplication* _riInstance;
		AutoRef<Display> _display;
};
