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

#ifndef FF3DAttractivePointForceModel_H
#define FF3DAttractivePointForceModel_H
#include "micolelib.h"

#include "FF3DForceModel.h"
#include "math.h"
using namespace Reachin;

/**
 * constraint FF3DDevice on an invisible point
 */
struct MICOLELIB_API FF3DAttractivePointForceModel : FF3DForceModel
{
	mgFloat _stiffness, _d; 

	Vec3f _stick_point;


	FF3DAttractivePointForceModel(const Vec3f &);
	FF3DAttractivePointForceModel(const FF3DAttractivePointForceModel &fm);

	virtual FF3DForceModel * clone() { return new FF3DAttractivePointForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pos, const mgFloat &) 
	{
		Vec3f diff = _stick_point - pos; 
		Vec3f x = diff;
		x.normalizeSafe();

		mgFloat dist = diff.length();

		//Vec3f res = _stiffness * diff;
		if (diff.length() > _d)
			return (x *_stiffness);

		return (_stiffness * sqrt(dist / _d) * x); 
	}
};

#endif
