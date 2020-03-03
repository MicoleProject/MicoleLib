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
#include "Binding.h"
#include <string.h>

//Should not be used...
MicoleBinding::MicoleBinding()
:_regexp(_strdup("")), _cb(NULL), _bus(NULL)
{
}

MicoleBinding::MicoleBinding(const char *regexp, MicoleBus *bus, MicoleCallback * cb)
:_regexp(_strdup(regexp)), _cb(cb), _bus(bus)
{
	bus->bindMsg(string(regexp), cb);
}


void MicoleBinding::unbindMsg() {
	_bus->unbindMsg(_cb);
}

MicoleBinding::~MicoleBinding()
{
	free(_regexp);
}

MicoleBinding::MicoleBinding(const MicoleBinding &b)
:_regexp(_strdup(b._regexp))
{
/*
	_regexp = NULL;
	setRegexp(b._regexp);
	callback = b.callback;
	bind_id = b.bind_id;*/
}

/**
 * set regular expression
 * @param pRegexp: regular expression. This string is duplicate into object. If a string already
 * exist in this objet, function will free the old string
 */
void MicoleBinding::setRegexp(const char * pRegexp)
{
	if(_regexp != NULL) 
		free(_regexp);

	//char * tmpS = (char *)malloc((strlen(pRegexp)+1)*sizeof(char));
	//strcpy(tmpS, pRegexp);
	//_regexp = tmpS;
	_regexp = _strdup(pRegexp);
}
