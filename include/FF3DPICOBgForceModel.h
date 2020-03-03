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

#ifndef FF3DPICOBGFORCEMODEL_H
#define FF3DPICOBGFORCEMODEL_H
#include "micolelib.h"

#include <Vrml.h>
#include <Time.h>
using namespace Reachin;

#include "FF3DForceModel.h"
#include "Picob.h"

/**
 * constraint FF3DDevice on an invisible point
 */
struct MICOLELIB_API FF3DPicobGForceModel : FF3DForceModel
{


	FF3DPicobGForceModel(const Vec3f &, vector<Vec3f> &bumps, const float &bumpTime, const float &pauseTime, const mgFloat &stiffness = STIFFNESS);
	FF3DPicobGForceModel(const FF3DPicobGForceModel& fm);

	virtual FF3DForceModel * clone() { return new FF3DPicobGForceModel(*this); }

	virtual Vec3f evaluate( const Vec3f &pos, const mgFloat &w ) 
	{
		Time currentTime;
		currentTime.setToNow();
		
		if (_p.isFinished(currentTime - _begin))
			_begin.setToNow();
		
		Vec3f diff = (_stick_point + _p.getDeviation(currentTime - _begin)) - pos;
		return _stiffness * diff;
	}
	private:
		mgFloat _stiffness; 
		Vec3f _stick_point;
		Time _begin;
		Picob _p;
};

#endif
