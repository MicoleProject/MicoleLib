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

#ifndef MicoleStringStream_H
#define MicoleStringStream_H

#include "micolelib.h"
#include <string>
#include <stdlib.h>

using namespace std;

/** @ingroup core 
 *  @{
 */


/*!
 * \brief
 * StringStream VC8 replacement
 * 
 * This class is a short implementation of classic StringStream. This
 * version avoid memory leak due to bad Microsoft implementation of string streeam classes. 
 *
 */
class MICOLELIB_API MicoleStringStream
{
	public:
		MicoleStringStream();
		MicoleStringStream(const MicoleStringStream &mss);
		virtual ~MicoleStringStream();
		
		string getString() const;
		string str() const;
		void flush();

		MicoleStringStream & operator= (const MicoleStringStream &mss);
		MicoleStringStream & operator<< (const string &pStr);
		MicoleStringStream & operator<< (int);
		MicoleStringStream & operator<< (double);

	private:
		string _data;
};

/** @}*/ //end core

#endif
