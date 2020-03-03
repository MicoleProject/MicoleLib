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

#ifndef MICOLE_DEFINE_H
#define MICOLE_DEFINE_H

#define _FF3D_IDENTIFIER_ "FF3D"
#define _IN_FF3D_ "IN "<<_FF3D_IDENTIFIER_ //could be phantom to suit default naming convention
#define _OUT_FF3D_ "OUT "<<_FF3D_IDENTIFIER_

#ifdef SOLAR_SYSTEM
#define STIFFNESS 100
#else
#define STIFFNESS 800
#endif

#endif