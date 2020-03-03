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

#ifndef FF3DFORCEMODEL_H
#define FF3DFORCEMODEL_H

#include "micolelib.h"
#include <ForceModel.h>
#include "define.h"

using namespace Reachin;

/** 
 * generic force model
 */
struct MICOLELIB_API FF3DForceModel : ForceModel3DOF
{
	static Vec3f _oldForce; 
	/**
	 * clone method
	 */
	virtual FF3DForceModel * clone() = 0;
};

#endif
