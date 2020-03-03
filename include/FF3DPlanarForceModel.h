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

#ifndef FF3DPlanarForceModel_H
#define FF3DPlanarForceModel_H
#include "micolelib.h"

#include "FF3DForceModel.h"
using namespace Reachin;

/**
 * constraint FF3DDevice on an invisible plan
 */
struct MICOLELIB_API FF3DPlanarForceModel : FF3DForceModel
{
	mgFloat _stiffness; 

	Vec3f _pt1;
	Vec3f _pt2;
	Vec3f _pt3;

	/**
	 * default constructor
	 * @param pt1 first point to define base of plan
	 * @param pt2 second point define vector and "norm" of plan
	 */
	FF3DPlanarForceModel(const Vec3f &pt1, const Vec3f &pt2, const mgFloat &stiffness = STIFFNESS);
	FF3DPlanarForceModel(const FF3DPlanarForceModel &fm);

	virtual FF3DForceModel * clone() {return new FF3DPlanarForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pPos, const mgFloat &w )
	{
		Vec3f pos = pPos; //duplicate original pos
		pos.x = 0; //ignore y pos
		pos.z = 0; //ignore y pos
		Vec3f diff = Vec3f(0,0,0) - pos;
		diff.x = 0; //only for linear
		diff.z = 0; //only for planar
		return _stiffness * diff; 
	}
};

#endif
