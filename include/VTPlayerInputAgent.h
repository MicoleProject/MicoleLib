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

#ifndef VTPlayerInputAgent_H
#define VTPlayerInputAgent_H

#include <VTPMouseController.h>
#include <VTPMouseListener.h>
#include "abstractmouseinputagent.h"
#include "micolelib.h"


/** @defgroup vtplayer VTPlayer device
 *  classes grouping all VTPlayer control classes
 *  @{
 */


/** @defgroup input Input classes
 *  Input helper classes 
 *  @{
 */


/*!
 * \brief
 * helper for input messages for VTPlayer
 * 
 * this class bind "IN VTP : pos=|press=|release" messages send by VTPlayerOutputAgent and
 * bind it to onMove, onButtonUp and onButtonDown functions.
 * 
 * \see
 * AbstractMouseInputAgent
 */
class MICOLELIB_API VTPlayerInputAgent : public AbstractMouseInputAgent
{
	public:
		/** 
		 * default constructor
		 */
		VTPlayerInputAgent();
		~VTPlayerInputAgent();

		void handleInputCoord ( MicoleBus *app, int argc, const char **argv );
		void handleInputPressButton ( MicoleBus *app, int argc, const char **argv );
		void handleInputReleaseButton ( MicoleBus *app, int argc, const char **argv );

		virtual void onMove(const int &posx, const int &posy);

		virtual void onButtonUp(const int &button);
		virtual void onButtonDown(const int &button);
};

/** @} */ //end input group

/** @} */ //end vtplayer group

#endif
