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
#include "FF3DAttractivePointForceModel.h"

#include "ReachinMicoleApplication.h"

/**
 * Default constructor
 * @param pPoint constrain point
 * @param stiffness stiffness
 */
FF3DAttractivePointForceModel::FF3DAttractivePointForceModel(const Vec3f &pPoint)
: _stick_point(pPoint)
{
	_stiffness = ReachinMicoleApplication::getFloatVar("stiffness");
	_d = ReachinMicoleApplication::getFloatVar("d");
}

/**
 * recopy constructor
 */
FF3DAttractivePointForceModel::FF3DAttractivePointForceModel (const FF3DAttractivePointForceModel &fm)
: _stick_point(fm._stick_point),_stiffness(fm._stiffness)
{
}