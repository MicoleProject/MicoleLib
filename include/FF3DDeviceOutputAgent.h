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

#ifndef FF3DDEVICEOUTPUTAGENT_H
#define FF3DDEVICEOUTPUTAGENT_H
#include "micolelib.h"

#include <Child.h>
#include "FF3DForceModel.h"
#include "ReachinMicoleApplication.h"
#include "MicoleAgent.h"
#include "playback.h"

/** 
 * @addtogroup FF3D
 * @{
 */

/**
 * Handle micolebus message like  "OUT FF3D : " and give order to FF3DDevice via Reachin Collider
 * @sa FF3DDeviceOutputAgentCollider
 *
 * FF3DDeviceOutputAgent handle this sort of "packet" on the micoleBus

 \section FF3DDOA_reco FF3DDeviceOutputAgent recognized messages.
 
This is common FF3DDevice ForceModel command
\code
OUT FF3D : point=(x, y, z)
\endcode
Constraint device on a point.
\code
OUT FF3D : sphere=(x, y, z, radius);
\endcode
Constraint device into a sphere.
\code
OUT FF3D : line=((x1, y1, z1),(x2, y2, z2));
\endcode
Constraint device on a line
\code
OUT FF3D : plan=((x1, y1, z1),(x2, y2, z2));
\endcode
Constraint device on a plan. Second triplet define normal vector on plan.
\code
OUT FF3D : disc=((x1, y1, z1),(x2, y2, z2),radius);
\endcode
Constraint device on a disc. Same as plan force model but with a limitation around base point defined by first triplet.
\code
OUT FF3D : none;
\endcode
Remove current force model. 
 *
 */

class MICOLELIB_API FF3DDeviceOutputAgent : public Child, public MicoleAgent
{
	public:
		/**
		 *
		 * Default Constructor 
		 */
		FF3DDeviceOutputAgent();
		virtual ~FF3DDeviceOutputAgent();
		Playback* pb;
		FILE* log;

		/**
		 * you can specify a force model
		 * @param fm FF3DForceModel who constraint FF3DDevice
		 * @warning ForceModel is deleted automatically if replaced (no clone of object)
		 * @note if you use IvyBus, you don't need to use this function
		 */
		void setForceModel(FF3DForceModel * fm) 
		{
			if (_fm = fm) //dangerous code
				return;
			if (_fm) //if _fm is not null
			{
				//i that possible?
				delete(_fm);
			}
			_fm = fm;
/*			
			//this code must be a joke:
			if (fm != NULL)
			{
				_fm = fm;
			}
			else
			{
				_fm = NULL;
			}*/
		}

	/**
	 * FF3DDeviceOutputAgentCollider, add forceModel corresponding to IvyBus instructions
	 */
	struct FF3DDeviceOutputAgentCollider : public Collider
	{
		FF3DDeviceOutputAgent *_parent;

		virtual void collide( CollisionState *cs)
		{ 
			Child::Collider::collide( cs ); 
			//Vec3f haptic_position = cs->getFinger(); 
			//if( haptic_position * haptic_position < 0.1 ) { 
			if (_parent->_fm)
				cs->addAbsoluteLocalFM( _parent->_fm );
				//cs->addAbsoluteGlobalFM( _parent->_fm ); }
		}
			//} 
	};

	protected:
		/** 
		 * handle point force model order via Micole Bus
		 */
		virtual void handlePointForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle linear model order via Micole Bus
		 */
		virtual void handleLinearForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle planar model order via Micole Bus
		 */
		virtual void handlePlanarForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle none force model order via Micole Bus
		 */
		virtual void handleNoForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle spherical force model order via Micole Bus
		 */
		virtual void handleSphericalForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle disc force model order via Micole Bus
		 */
		virtual void handleDiscForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle linear with Picob model order via Micole Bus
		 */
		virtual void handlePicobHgForceModel ( MicoleBus *app, int argc, const char **argv );
		/** 
		 * handle linear with Picob model order via Micole Bus
		 */
		virtual void handlePicobGForceModel ( MicoleBus *app, int argc, const char **argv );

	private:
		FF3DForceModel * _fm; /**< current force model */
		Transform * _transform; /**<  */
};
/** @} */ // end of FF3D

#endif
