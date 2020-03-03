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

#ifndef FF3DGuidanceAgent_H
#define FF3DGuidanceAgent_H
#include "micolelib.h"

#include <Child.h>
#include "FF3DForceModel.h"
#include "ReachinMicoleApplication.h"
#include "MicoleAgent.h"

/**
 * Handle ivy message like  "OUT FF3D GUIDE : " and give order to FF3DDevice via Reachin Collider
 * @see FF3DGuidanceAgentCollider
 *
 * FF3DGuidanceAgent handle this sort of "packet" on the ivybus
 * \code
 *	ama->sendBusMessage("OUT FF3D GUIDE : point=(1, 3, 3);");
 *	ama->sendBusMessage("OUT FF3D GUIDE : line=((0, 0, 0),(0.2, 0.2, 0.2));");
 *	ama->sendBusMessage("OUT FF3D GUIDE : plan=((0, 0, 0),(1, 3, 3));");
 *	ama->sendBusMessage("OUT FF3D GUIDE : none;");
 * \endcode
 *
 */

class MICOLELIB_API FF3DGuidanceAgent : public Child, public MicoleAgent
{
	public:
		/**
		 *
		 * Default Constructor 
		 */
		FF3DGuidanceAgent();
		virtual ~FF3DGuidanceAgent();

		/**
		 * you can specify a force model
		 * @param fm FF3DForceModel who constraint FF3DDevice
		 * @warning ForceModel is deleted automatically if replaced (no clone of object)
		 * @note if you use the MicoleBus, you don't need to use this function
		 */
		void setForceModel(FF3DForceModel * fm) 
		{
			if (_fm=fm) //if it is the same force model
			{
				return;
			} 
			if (_fm) //if _fm is not null
			{
				delete(_fm);
			}
			_fm = fm;
		}

	/**
	 * FF3DGuidanceAgentCollider, add forceModel corresponding to IvyBus instructions
	 */
	struct FF3DGuidanceAgentCollider : public Collider
	{
		FF3DGuidanceAgent *_parent;

		virtual void collide( CollisionState *cs)
		{ 
			Child::Collider::collide( cs ); 
			if (_parent->_fm)
				cs->addAbsoluteLocalFM( _parent->_fm );
		}
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

#endif
