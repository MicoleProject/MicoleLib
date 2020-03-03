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
#include "FF3DDiscForceModel.h"

/**
 * Default constructor
 * @param pt1 constrain point
 * @param range spherical size
 * @param stiffness stiffness
 */
FF3DDiscForceModel::FF3DDiscForceModel(const Vec3f &pt1, const Vec3f &pt2 ,const mgFloat &range, const mgFloat &stiffness )
: _pt1(pt1), _pt2(pt2), _range(range), _stiffness(stiffness)
{
}

/**
 * recopy constructor
 */
FF3DDiscForceModel::FF3DDiscForceModel (const FF3DDiscForceModel &fm)
: _pt1(fm._pt1),_pt2(fm._pt2),_range(fm._range),_stiffness(fm._stiffness)
{
}
