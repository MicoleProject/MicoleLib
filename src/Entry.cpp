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
#include "Entry.h"

int Entry::counter = 1;

Entry::Entry()
:_name("Invalid"), _app(NULL), _id(-1), _state(UNKNOWN)
{
}

Entry::Entry(const Entry &e)
:_name(e._name), _app(e._app), _id(e._id), _state(e._state)
{
}

Entry::Entry(const string &name, const AgentState &state)
:_name(name), _app(NULL), _state(state), _id(counter++)
{
}

Entry::~Entry() 
{
}

int Entry::getId() const
{
	return _id;
}

string Entry::getName() const
{
	return _name;
}

AgentState Entry::getState() const
{
	return _state;
}

void Entry::setApp(MicoleBus *app)
{
	_app = app;
}

Entry &Entry::operator =(const Entry &e)
{
	_name = e._name;
	_app = e._app;
	_id = e._id;
	_state = e._state;
	return *this;
}
