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

#ifndef VTPlayerOutputAgent_H
#define VTPlayerOutputAgent_H

#include <VTPMouseController.h>
#include <VTPMouseListener.h>
#include "MicoleAgent.h"
#include "micolelib.h"


/** @defgroup vtplayer
 *  @{
 */
class MICOLELIB_API VTPlayerOutputAgent : public MicoleAgent
{
	public:
		VTPlayerOutputAgent(); 
		~VTPlayerOutputAgent(); 

		void handleDynamic(MicoleBus *app, int argc, const char **argv);
		void handleStatic(MicoleBus *app, int argc, const char **argv);
		void handleDirection(MicoleBus *app, int argc, const char **argv);

	private:
		VTPIcon * _left,* _right;
		VTPMouseController * _vtpctrl;
		VTPStaticIcon *_sN, *_sS, *_sE, *_sW, *_sNE, *_sNW, *_sSE, *_sSW;
		//@TODO: dynamic icons
};

/** @} */ //end vtplayer group

#endif
