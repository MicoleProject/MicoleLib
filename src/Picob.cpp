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
#include "Picob.h"

Picob::Picob(const float &bumpLength, const float &pauseLength, vector<Vec3f> &bumps)
:_bumpLength(bumpLength), _pauseLength(pauseLength), _bumps(bumps)
{
}

Picob::Picob(const Picob &p)
:_bumpLength(p._bumpLength), _pauseLength(p._pauseLength), _bumps(p._bumps)
{
}

Picob::~Picob()
{
}

Vec3f Picob::getDeviation(const float &pos) const
{
	unsigned int n = pos / (_bumpLength + _pauseLength);
	if (n >= _bumps.size() || n < 0 ||pos < 0)
		return Vec3f(0.0, 0.0, 0.0);
	float relpos = pos - n * (_bumpLength + _pauseLength);
	if (relpos > _bumpLength)
		return Vec3f(0.0, 0.0, 0.0);
	//Vec3f temp = sin(relpos/_bumpLength * 2 * M_PI - M_PI) * _bumps[n];
	//Vec3f temp = sin(relpos/_bumpLength * M_PI) * _bumps[n];
	Vec3f temp = (cos((relpos/_bumpLength) * 2 * M_PI - M_PI) + 1) / 2 * _bumps[n];
	//return sin((pos - n * (_bumpLength + _pauseLength)) * M_PI/_bumpLength) * _bumps[n];
	return temp;
}

Vec3f Picob::getDeviation(const float &pos, const float &size) const
{
	float sequenceLength = _bumps.size() * (_bumpLength + _pauseLength);
	return getDeviation(pos - (size - sequenceLength) / 2);
}

bool Picob::isFinished(const float &pos) const
{
	return pos > _bumps.size() * (_bumpLength + _pauseLength) || pos < 0;
}

bool Picob::isFinished(const float &pos, const float &size) const
{
	float sequenceLength = _bumps.size() * (_bumpLength + _pauseLength);
	return pos > (size - sequenceLength) / 2 || pos > (size + sequenceLength) / 2;
}

void Picob::readPicobList(const char *str, vector<Vec3f> &bumpList)
{
	char *part;
	float vals[3];
	int step=0, lastpos = 0;
	for (unsigned int i = 0; i < strlen(str) ; i++)
	{
		switch(step)
		{
			//waiting for a special char
			case 0:
				if (str[i] != '(' && str[i] != ' ' && str[i] != ',')
					return;
				lastpos++;
				step++;
				break;
			//reading the three number of a triplet
			case 1: case 2: case 3:
				if (str[i] == ',' || str[i] == ')')
				{
					part = new char[i - lastpos + 1];
					for (unsigned int j = 0 ; j < i - lastpos ; j++)
						part[j] = str[lastpos + j];
					part[i - lastpos] = '\0';
					vals[step - 1] = atof(part);
					delete []part;
					if (step == 3)
					{
						step = 0;
						bumpList.push_back(Vec3f(vals[0], vals[1], vals[2]));
					}
					else
						step++;
					lastpos = i + 1;
				}
				else if (str[i] == '(')
					lastpos++;
				break;
		}
	}
}
