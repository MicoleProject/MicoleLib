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

#ifndef MICOLENTRY_H
#define MICOLENTRY_H
#include "micolelib.h"

#include <string>

#include "MicoleAgent.h"

/**
 * Obsolete class
 * @TODO remove this class
 */
class MICOLELIB_API Entry
{
	public:
		Entry();
		Entry(const string &name, const AgentState &state);
		Entry(const Entry &e);
		~Entry();
		
		int getId() const;
		string getName() const;
		AgentState getState() const;
		void setApp(MicoleBus *app);
		Entry &operator =(const Entry &e);

	private:
		string _name;
		MicoleBus *_app;
		int _id;
		AgentState _state;
		static int counter;
};

#endif
