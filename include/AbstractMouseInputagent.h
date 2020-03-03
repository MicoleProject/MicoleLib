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

#ifndef AbstractMouseInputAgent_H
#define AbstractMouseInputAgent_H

#include "MicoleAgent.h"
#include "micolelib.h"

/** @ingroup mouse
 *  @{
 */

/** @ingroup input 
 *  @{
 */


/*!
 * \brief
 * Base class for all Mouse Input agent
 * 
 * This class declare 3 virtual empty functions, onMove, onButtonDown and onButtonUp.
 * 
 */
class MICOLELIB_API  AbstractMouseInputAgent : public MicoleAgent
{
	public:
		AbstractMouseInputAgent();
		~AbstractMouseInputAgent();

		virtual void onMove(const float &x, const float &y){};
		virtual void onButtonDown(const int &button){};
		virtual void onButtonUp(const int &button){};

};
/** @} */ // end of Input
/** @} */ // end of MOUSE

#endif
