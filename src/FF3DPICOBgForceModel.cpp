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

#include "stdafx.h"
#include "FF3DPicobGForceModel.h"

/**
 * Default constructor
 * @param pPicobG constrain point
 * @param stiffness stiffness
 */
FF3DPicobGForceModel::FF3DPicobGForceModel(const Vec3f &pPoint, vector<Vec3f> &bumps, const float &bumpTime, const float &pauseTime, const mgFloat &stiffness )
: _stick_point(pPoint), _stiffness(stiffness), _p(bumpTime, pauseTime, bumps)
{
	_begin.setToNow();
}

/**
 * recopy constructor
 */
FF3DPicobGForceModel::FF3DPicobGForceModel (const FF3DPicobGForceModel &fm)
: _stick_point(fm._stick_point), _stiffness(fm._stiffness), _p(fm._p), _begin(fm._begin)
{
}
