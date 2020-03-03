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

#ifndef REGISTRYAGENT_H
#define REGISTRYAGENT_H
#include "micolelib.h"

#include <map>
#include <string>
using namespace std;

#include "MicoleAgent.h"
#include "Entry.h"

/** @defgroup core Core
 *  Main core classes
 *  All core classes in "needed" by micoleLib. Please do not modify anything into it.
 *  @{
 */

/*
* Registers agents and gives identifiers for them.
*/
class MICOLELIB_API RegistryAgent: public MicoleAgent
{
	private:
		map<int,Entry> agents;
		map<string, string> _envVars;

	public:
		RegistryAgent(const string &name, const string &domain);
		~RegistryAgent();

		void handleRegisterMessage(MicoleBus *app, int argc, const char **argv);
		void handleUnRegisterMessage(MicoleBus *app, int argc, const char **argv);
		void handlePrintRequest(MicoleBus *app, int argc, const char **argv);
		void handleQuitMessage( MicoleBus *app, int argc, const char **argv);
		void handleSetVar(MicoleBus *app, int argc, const char **argv);

		void handleGetVar(MicoleBus *app, int argc, const char **argv);
		

		string getVar(string varName) 
		{
			return _envVars[varName];
		}
		
		double getFloatVar(string varName) 
		{
			return atof(_envVars[varName].c_str());
		}
		
		void setVar(string varName, string value)
		{
			_envVars[varName] = value;
		}
};

/** @} */ //core classes

#endif

