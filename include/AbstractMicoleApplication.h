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

#ifndef ABSTRACMICOLEAPPLICATION_H
#define ABSTRACMICOLEAPPLICATION_H

#include "micolelib.h"

#include "RegistryAgent.h"
#include "NetworkAgent.h"

//for retro-compatibility
#define sendIvyMessage sendBusMessage

/**
 * AbstractMicoleApplication class provide a simple class singleton object with minimum initialisation.
 * You can retrieve it by AbstractMicoleApplication::getInstance function
 * This class init a simple MicoleBus with a RegistryAgent
 * @see RegistryAgent
 */
class MICOLELIB_API AbstractMicoleApplication
{
	public:
		
		/**
		 * default destructor
		 */
		virtual ~AbstractMicoleApplication(void);
	  
		/**
		 * return unique instance of AbstractMicoleApplication
		 * @return unique instance of AbstractMicoleApplication
		 */
		static AbstractMicoleApplication * getInstance()
		{
			if (!_instance)
				_instance = new AbstractMicoleApplication;
			return _instance;
		}

		/**
		 * broadcast a Message on the bus
		 * @param c string to broadcast ont the bus
		 */
		int sendBusMessage(const char * c);

		/**
		 * this function start a network on specified host:port
		 * @param host server IP
		 * @param port server port
		 */
		void startNetwork(char * host = "127.0.0.1", int port = 33483);

		/**
		 * send a network message (no transit on common bus)
		 * @param s string message
		 */
		void sendNetworkMessage(string s);

		/**
		 * helper function, retrieve registryAgent specifie value
		 * @see RegistryAgent
		 */
		static string getVar(string varName)
		{
			return AbstractMicoleApplication::getInstance()->_registry->getVar(varName);
		}

		/**
		 * helper function, set registryAgent specifie value
		 * @see RegistryAgent
		 */
		static void setVar(string varName, string varValue)
		{
			return AbstractMicoleApplication::getInstance()->_registry->setVar(varName,varValue);
		}

		/**
		 * helper function, retrieve registryAgent specifie value
		 * @see RegistryAgent
		 */

		static double getFloatVar(string varName)
		{
			return AbstractMicoleApplication::getInstance()->_registry->getFloatVar(varName);
		}

	protected:
		/**
		 * default constructor may not be called directly, use getInstance
		 * @see AbstractMicoleApplication::getInstance
		 */
		AbstractMicoleApplication(void);
		static AbstractMicoleApplication* _instance; /**< pointer on uniq instance*/

	private:

		RegistryAgent * _registry;/**< RegistryAgent */
		NetworkAgent * _networkAgent;
};

#endif