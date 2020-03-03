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

#ifndef SOUNDPLAYERAGENT_H
#define SOUNDPLAYERAGENT_H

#include "micolelib.h"

#include "MicoleAgent.h"
// plus other inputs...

class MICOLELIB_API SoundPlayerAgent: public MicoleAgent
{
	public:
		//add any necessary methods
		void handleOutputMessage ( MicoleBus *app, int argc, const char **argv );
		void handleQuitMessage ( MicoleBus *app, int argc, const char **argv );
		SoundPlayerAgent(string name, string type, const char* domain);
		SoundPlayerAgent(void);
		bool cont;
};

#endif
