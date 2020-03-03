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

#ifndef VTPlayerSenderAgent_H
#define VTPlayerSenderAgent_H

#include <VTPMouseController.h>
#include <VTPMouseListener.h>
#include "MicoleAgent.h"
#include "micolelib.h"


/** @defgroup vtplayer
 *  @{
 */

class MICOLELIB_API VTPlayerSenderAgent : public MicoleAgent, VTPMouseListener
{
	public:
		VTPlayerSenderAgent();
		~VTPlayerSenderAgent();

		virtual void onMouseMove(int posx, int posy);

		virtual bool onPressed(int button);
		virtual bool onReleased(int button);

	private:
		VTPlayerSenderAgent(const VTPlayerSenderAgent &);
};

/** @} */ //end vtplayer group

#endif
