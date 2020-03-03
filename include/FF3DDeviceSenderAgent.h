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

#ifndef FF3DDEVICESENDERAGENT_H
#define FF3DDEVICESENDERAGENT_H
#include "micolelib.h"

#define BUTTON_LAG 0.1 /**< define button lag time*/

#include <config.h>
#include "MicoleAgentThread.h"

#include <Display.h>
#include <Scene.h>
#include <Node.h>
#include <Field.h>

#include <iostream>
//#include <string>

using namespace Reachin;
using namespace std;

/** @defgroup FF3D ForceFeedBack 3d Device
 *  classes grouping all FF3D Device control classes
 *  @{
 */

/** @addtogroup sender @{ */

/** 
 * FF3DDeviceSenderAgent send 3d coordinates of reachin device
 * reachin device is configured into REACHIN API Config tools.<br/>
 * Reachin device is usually a Phantom but can be a delta, 
 * omega or other 3D feedback device 
 


 \output
  Sample messages on the bus:

 \code 
IN FF3D : pos=(5.225023031e-004, -8.042110825e-002, -4.128258514e-002); evt=RELEASED;
IN FF3D : pos=(-5.164337754e-004, -8.183319092e-002, -1.136016369e-002); evt=RELEASED;
IN FF3D : pos=(-7.827099609e-002, -8.194231796e-002, 3.048819923e-002); evt=PRESSED;
 \endcode
 
 
 */

class MICOLELIB_API FF3DDeviceSenderAgent : public Node, MicoleAgentThread
{
	public:
		/**
		 * Force Feedback 3D Device position Reachin Field
		 */
		struct FF3DCoord : public Dependent<EvaldFField< FF3DCoord, SFVec3f, SFVec3f >>
		{
			void evaluate( SFVec3f * constraint_position )
			{
				if (constraint_position)
				{
					Vec3f p = constraint_position->get();
					value.x = p.x;
					value.y = p.y;
					value.z = p.z;
				}
				//cout << p.x << " "<< p.y << " " << p.z << endl;
			}
		};

		/**
		 * Force Feedback 3D Device button state Reachin Field
		 */
		struct ButtonPress : public Dependent< EvaldFField< ButtonPress, SFBool, SFBool > >
		{
				ButtonPress() : pressed(false), skip(false), last_event(0) { }
				void evaluate(SFBool);
				bool eventPressed();
				bool eventReleased();
				bool pressed;
				bool skip;
				Time last_event;
		};
		auto_ptr< FF3DCoord > _FF3DCoord;

		auto_ptr< ButtonPress > _buttonPress;

		/**
		 * default constructor
		 * @param pDelay delay beetween each send on iby bus. Default is 8ms to get around 120hz (scene graph and evaluation is around 60hz)
		 * @note Reachin scene fields are evaluate at 60hz. More than 120Hz and you will see no difference, less than 60Hz and you will get a 'lag effect'.
		 */
		FF3DDeviceSenderAgent(const int &pDelay = 8); 
		
		~FF3DDeviceSenderAgent();

		/**
		 * thread main loop
		 */
		virtual void run();

	protected:
		/**
		 * callback when thread is prepared to be stopped
		 */
		virtual void prepareToStop(int argc, const char **argv);
		/**
		 * callback when thread is prepared to be suspended
		 */ 
		virtual void prepareToSuspend(int argc, const char **argv);

	private:
		int _delay; /**< delay beetween each coordinates send in milisecond*/
};

/** @} */ // end of Sender
/** @} */ // end of FF3D

#endif
