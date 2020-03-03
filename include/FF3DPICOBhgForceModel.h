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

#ifndef FF3DPicobHGForceModel_H
#define FF3DPicobHGForceModel_H
#include "micolelib.h"

#include <iostream>
#include "ReachinMicoleApplication.h"
#include "Picob.h"

#include "FF3DForceModel.h"
using namespace Reachin;
using namespace std;

/**
 * linear force model constraint FF3DDevice on an invisible line
 */
struct MICOLELIB_API FF3DPicobHGForceModel : FF3DForceModel
{
	/**
	 * default constructor
	 * @param pt1 first point to define line
	 * @param pt2 second point
	 */
	FF3DPicobHGForceModel(const Vec3f &pt1, const Vec3f &pt2, vector<Vec3f> &bumps, const float &bumpLength, const float &pauseLength, const mgFloat &stiffness = STIFFNESS);
	FF3DPicobHGForceModel (const FF3DPicobHGForceModel &fm);

	virtual FF3DForceModel * clone() { return new FF3DPicobHGForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pPos, const mgFloat &w )
	{
		Vec3f vect = _pt2 - _pt1;
		Vec3f diff = _p.getDeviation(pPos.y + (_pt2 - _pt1).length()/2, (_pt2 - _pt1).length()) - pPos; 
		diff.y = 0;

		Vec3f force = _stiffness * diff;
		return force; 
	}
	private:
		mgFloat _stiffness; 

		Vec3f _pt1;
		Vec3f _pt2;
		Vec3f _oldForce;
		Picob _p;
};

#endif
