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

#ifndef FF3DPOINTFORCEMODEL_H
#define FF3DPOINTFORCEMODEL_H
#include "micolelib.h"

#include "FF3DForceModel.h"
#include "Playback.h"
using namespace Reachin;

/**
 * constraint FF3DDevice on an invisible point
 */
struct MICOLELIB_API FF3DPointForceModel : FF3DForceModel
{
	mgFloat _stiffness; 
	Playback* _pb;
	
	Vec3f _stick_point;


	FF3DPointForceModel(Playback* pb, const Vec3f &, const mgFloat &stiffness = STIFFNESS);
	FF3DPointForceModel(Playback* pb, const FF3DPointForceModel &fm);
	

	virtual FF3DForceModel * clone() { return new FF3DPointForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pos, const mgFloat &w ) 
	{
		Vec3f diff = _stick_point - pos;
		_pb->stickToPointAndGetForce(pos, _stick_point, _oldForce);
		return _oldForce;
		//fprintf(log, "%f, %f, %f, %f, %f, %f, %f, %f\n", pos.x, pos.y, pos.z, _stick_point.x, _stick_point.y, _stick_point.z);
/*		diff = _stiffness * diff;
		fprintf(log, "%f, %f, %f\n", diff.x, diff.y, diff.z);
		return diff;*/

		//diff.x = 0; only for linear
		//_oldForce= (_oldForce*0.95)+ (_stiffness * diff)*0.05; 
		//return _oldForce;
	//	return _stiffness * diff;
	}
};

#endif
