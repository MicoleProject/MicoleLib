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

#ifndef FF3DLinearForceModel_H
#define FF3DLinearForceModel_H
#include "micolelib.h"

#include <iostream>
#include "ReachinMicoleApplication.h"


#include "FF3DForceModel.h"
using namespace Reachin;
using namespace std;

/**
 * linear force model constraint FF3DDevice on an invisible line
 */
struct MICOLELIB_API FF3DLinearForceModel : FF3DForceModel
{
	mgFloat _stiffness; 

	Vec3f _pt1;
	Vec3f _pt2;
	//Vec3f _oldForce;

	/**
	 * default constructor
	 * @param pt1 first point to define line
	 * @param pt2 second point
	 */
	FF3DLinearForceModel(const Vec3f &pt1, const Vec3f &pt2, const mgFloat &stiffness = STIFFNESS);
	FF3DLinearForceModel (const FF3DLinearForceModel& fm);

	virtual FF3DForceModel * clone() { return new FF3DLinearForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pPos, const mgFloat &w )
	{
		
		Vec3f pos = pPos; //duplicate original pos
		pos.y = 0; //ignore y pos
		Vec3f diff = Vec3f(0,0,0) - pos; 
		diff.y = 0; //only for linear

		Vec3f force = _stiffness * diff;

		//test to avoid unhook of phantom

	
/*			if ((force - _oldForce).length() > 0.5) {
			
			//_oldForce 
			//s << "force: "<< force.length();
			//s << " difference : " << (force - _oldForce).length();
			//force.normalizeSafe();
			//force=force*10; //max length unit
			//s << " send force : " << force.length();
			//ReachinMicoleApplication * ama = ReachinMicoleApplication::getInstance();
			//ama->sendBusMessage(s.str().data());

			//while ((force - _oldForce).length() > 0.3) 
			while ((force-_oldForce).length() > 0.5) 
			{
				force = 0.9*force+_oldForce*0.1  ; //Vec3f(
			}
			
		}


		_oldForce = force;
		*/		
		
		_oldForce = 0.7*_oldForce + 0.3* force;
		
		return _oldForce; 
		//return Vec3f(100,0,0);
	}
};

#endif