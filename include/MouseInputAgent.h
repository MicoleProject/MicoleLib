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

#ifndef MOUSEINPUTAGENT_H
#define MOUSEINPUTAGENT_H

#include "abstractmouseinputagent.h"
#include "micolelib.h"

#include <Mouse.h>
using namespace Reachin;

/** @defgroup mouse Mouse
 *  Mouse control classes
 *  @{
 */

/** @defgroup input Input classes
 *  Input helper classes 
 *  @{
 */

class MICOLELIB_API  MouseInputAgent : public AbstractMouseInputAgent
{
	public:
		MouseInputAgent();
		~MouseInputAgent();

		void handleMouseMove( MicoleBus *app, int argc, const char **argv );

		//virtual void onMove(int x, int y){};
		//virtual void onEvt(string){};

	protected:
		Vec2f _pos;
};
/** @} */ // end of Input
/** @} */ // end of MOUSE

#endif
