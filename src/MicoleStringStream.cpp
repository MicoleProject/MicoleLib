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

#include <cmath>

#include "MicoleStringStream.h"


MicoleStringStream::MicoleStringStream()
{
	_data = string("");
}

MicoleStringStream::MicoleStringStream(const MicoleStringStream &mss)
{
	_data = mss._data;
}

MicoleStringStream::~MicoleStringStream()
{
}

string MicoleStringStream::getString() const
{
	return _data;
}

string MicoleStringStream::str() const
{
	return _data;
}

void MicoleStringStream::flush()
{
	_data = "";
}

MicoleStringStream & MicoleStringStream::operator= (const MicoleStringStream &mss) 
{
	if (this == &mss) 
	{
		return *this;
	}
	_data = mss._data;
	return *this;
}

MicoleStringStream & MicoleStringStream::operator<< (const string &pStr)
{
	_data = _data + pStr;
	return *this;
}

MicoleStringStream & MicoleStringStream::operator<< (int i)
{
	int nbchars;
	//1 for the \0 and 1 for the - possible
	if (i == 0)
		nbchars = 3;
	else
		nbchars = int(floor(log10(fabs(static_cast<float>(i))))) + 3;
	char *buffer = new char[nbchars];

	_itoa_s (i, buffer, nbchars, 10);

	_data = _data + string(buffer);
	delete []buffer;
	return *this;
}

MicoleStringStream & MicoleStringStream::operator<< (double f)
{
	//1 for the \0 and 1 for the - possible, 1 for the possible .
	// and 10 decimals max
	int nbchars;
	if (fabs(f) <= 1)
		nbchars = 20;
	else
		nbchars = int(floor(log10(fabs(f)))) + 20;
	char *buffer = new char[nbchars];

	//gcvt (f,8,buffer);
	//int nbdecim, sign;
	//_ecvt_s(buffer, nbchars, f, 10, &nbdecim, &sign);
	_gcvt_s(buffer, nbchars, f, 10);

	_data = _data + string(buffer);
	delete []buffer;
	return *this;
}
