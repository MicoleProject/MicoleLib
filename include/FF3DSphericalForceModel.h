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

#ifndef FF3DSPHERICALFORCEMODEL_H
#define FF3DSPHERICALFORCEMODEL_H
#include "micolelib.h"

#include "define.h"

#include "FF3DForceModel.h"
#include <LinearAlgebra.h>
using namespace Reachin;

/**
 * constraint FF3DDevice on an invisible plan
 */
struct MICOLELIB_API FF3DSphericalForceModel : FF3DForceModel
{
	mgFloat _stiffness; 

	Vec3f _pt1;
	Vec3f _pt2;
	Vec3f _pt3;

	mgFloat _range;


	/**
	 * default constructor
	 * @param pt1 first point to define base of plan
	 * @param pt2 second point define vector and "norm" of plan
	 */
	FF3DSphericalForceModel(const Vec3f &pt, const mgFloat &pRange, const mgFloat &stiffness = STIFFNESS);
	FF3DSphericalForceModel (const FF3DSphericalForceModel &fm);

	virtual FF3DForceModel * clone() {return new FF3DSphericalForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pos, const mgFloat &w )
	{
		Vec3f diff = Vec3f(0,0,0) - pos; 
		//diff.x = 0; //only for linear
		//diff.z = 0; //only for planar
		if (diff.length()< _range)  
			return Vec3f(0,0,0);

		Vec3f normaldiff = diff;
		normaldiff.normalizeSafe();

		return (_stiffness * (diff-(_range * normaldiff))); 
	}
};

#endif