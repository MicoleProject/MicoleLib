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
#include "FF3DPicobHGForceModel.h"

/**
 * Default constructor
 * @param pPoint constrain point
 * @param stiffness stiffness
 */
FF3DPicobHGForceModel::FF3DPicobHGForceModel(const Vec3f &pt1, const Vec3f &pt2, vector<Vec3f> &bumps, const float &bumpLength, const float &pauseLength, const mgFloat &stiffness )
: _pt1(pt1), _pt2(pt2), _stiffness(stiffness), _p(bumpLength, pauseLength, bumps), _oldForce(0,0,0)
{
}

/**
 * copy constructor
 */
FF3DPicobHGForceModel::FF3DPicobHGForceModel (const FF3DPicobHGForceModel &fm)
:_pt1(fm._pt1), _pt2(fm._pt2), _stiffness(fm._stiffness), _p(fm._p)
{
}
