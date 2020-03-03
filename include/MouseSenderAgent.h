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

#ifndef MouseSenderAgent_H
#define MouseSenderAgent_H

/**
 * Listens the mouse movements and sends a message to channel when 
 * a button is pressed: 

 *
 */

/** @ingroup mouse
 *  @{
 */

/** @ingroup input 
 * @{
 */

#include <ctime>
#include <Mouse.h>
using namespace Reachin;

#include "MicoleAgentThread.h"
#include "micolelib.h"

/*!
 * \brief
 * Send mouse coordinates/buttons status on the bus
 * 
 * This class send information on mouse coordinates on the bus

 \section MSA_samlpe MouseSenderAgent messages example

 \code
 IN MSE : pos=(-8.305664062e-004, -2.768554687e-002); evt=(BT1_PRESSED);
 IN MSE : pos=(-1.439648437e-002, -8.305664062e-004); evt=(BT1_RELEASED);
  
 IN MSE : pos=(-1.439648437e-002, -8.305664062e-004); evt=(BT2_PRESSED);
 IN MSE : pos=(8.305664062e-004, -3.349951172e-002); evt=(BT2_RELEASED);
 \endcode
 * 
 * 
 * \see
 * MouseInputAgent
 */
class MICOLELIB_API MouseSenderAgent : public MicoleAgentThread
{
public:
	MouseSenderAgent(int coord_interval=4);
	~MouseSenderAgent();
	void setCoordinateUpdateInterval(int interval); // 0 - no coordinates
	void run();

	auto_ptr< SFVec3f > mouse_coordinates;

private:

	int _coordUpdInterval;

	struct MouseMoveListener : public  EvaldFField<MouseMoveListener, SFVec2f, SFVec2f> 
	{
		public:
			MouseMoveListener();
			void evaluate(SFVec2f *coord);
			MouseSenderAgent *_mssa;
			
		private:
			DWORD _mtime;
	};

	struct MouseButtonListener : public Dependent<EvaldFField<MouseButtonListener, SFBool, SFBool> >
	{
		public:
			MouseButtonListener(string button);
			void evaluate(SFBool *lbutton);
			void sendCoordinates( string state );
			MouseSenderAgent *_mssa;
			auto_ptr< SFBool > m_button;
			auto_ptr< SFVec2f > mouse_coordinates;
			string m_strButton;
	};

	auto_ptr< MouseMoveListener > _mouseMoveListener;
	auto_ptr< MouseButtonListener > _leftMouseButtonListener;
	auto_ptr< MouseButtonListener > _rightMouseButtonListener;
	auto_ptr< Mouse > _mouse;
};

/** @} */ // end of INPUT
/** @} */ // end of MOUSE

#endif
