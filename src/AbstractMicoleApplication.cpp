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
#include "AbstractMicoleApplication.h"


AbstractMicoleApplication * AbstractMicoleApplication::_instance = NULL;


AbstractMicoleApplication::AbstractMicoleApplication()
:_registry(new RegistryAgent ("RegistryAgent","127.255.255.255:2010"))
{
	
}

AbstractMicoleApplication::~AbstractMicoleApplication()
{
	/** @TODO verify if all agent is really stopped */
	delete(_registry);
	delete(_networkAgent);
}

int AbstractMicoleApplication::sendBusMessage(const char * pStr) 
{
	return _registry->sendMessage(pStr);
}

void AbstractMicoleApplication::startNetwork(char * hostname, int port) {
	_networkAgent = new NetworkAgent(hostname, port);
}

void AbstractMicoleApplication::sendNetworkMessage(string s) {
	if (_networkAgent == NULL) {
		throw "unknown network";
		return;
	}

	_networkAgent->sendOnNetwork(s);
}