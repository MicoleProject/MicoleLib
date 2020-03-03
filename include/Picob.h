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

#ifndef __PICOB__
#define __PICOB__
#include "micolelib.h"

#include <Vrml.h>
using namespace Reachin;


class MICOLELIB_API Picob
{
	public:
		Picob(const float &bumpLength, const float &pauseLength, vector<Vec3f> &bumps);
		Picob(const Picob &p);
		~Picob();
		Vec3f getDeviation(const float &pos) const;
		Vec3f getDeviation(const float &pos, const float &size) const;
		bool isFinished(const float &pos) const;
		bool isFinished(const float &pos, const float &size) const;
		/** 
		 * parses a point list
		 */
		static void readPicobList(const char *str, vector<Vec3f> &bumpList);

	private:
		float _bumpLength, _pauseLength;
		vector<Vec3f> _bumps;
};

#endif
