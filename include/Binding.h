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

#ifndef BINDING_H
#define BINDING_H
#include "micolelib.h"

#include <string>
using namespace std;
#include "MicoleBus.h"


/*!
 * \brief
 * Encapsulating class for MicoleBus Bindind
 * 
 * Encapsulation of binding mechanism in this class that can provide bind and unbind functionnality.
 * Only usefull if you use unbind functionnality (other programmer can just go on)
 * 
 * \see
 * MicoleBus
 */
class MICOLELIB_API MicoleBinding 
{
	public:
		/**
		 * constructor
		 */
		MicoleBinding(const char *regexp, MicoleBus *bus, MicoleCallback *);
		/**
		 * copy constructor
		 */
		MicoleBinding(const MicoleBinding &b);

		/**
		 * desctructor
		 */
		~MicoleBinding();

		/**
		 * specify a regexp
		 * @param pRegexp regular expression string
		 * @warning pRegexp string is duplicate, take care and free original by yourself if needed
		 */
		void setRegexp(const char * pRegexp); 

		/**
		 * return regular expression
		 * @return regular expression
		 */
		const char * getRegexp() { return _regexp; }

		/** 
		 * unbing this binding from the bus
		 */
		void unbindMsg();

	private:
		/**
		 * default constructor
		 */
		MicoleBinding();

		char * _regexp; /**< regexp string */
		MicoleCallback * _cb; /**< micole callback for unbind */
		MicoleBus * _bus; /**< pointer on the MicoleBus */
};

#endif
