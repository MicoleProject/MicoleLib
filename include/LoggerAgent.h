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

#ifndef LOGGERAGENT_H
#define LOGGERAGENT_H
#include "micolelib.h"

#include "MicoleAgent.h"
#include <iostream>
#include <fstream>
using namespace std;

/*!
 * \brief
 * Log messages on the bud
 * 
 * Log all messages on the bus and store it into in file.
 * 
 * \see
 * MicoleAgent
 */
class MICOLELIB_API LoggerAgent: public MicoleAgent
{
	public:
		/**
		 * Default constructor
		 * @remark no param needed
		 */
		LoggerAgent();
		virtual ~LoggerAgent();

		/**
		 * handle all message which transit on the bus
		 */
		void handleAllMessage ( MicoleBus *app, int argc, const char **argv );

	private:
		ofstream _log; /**< output log file stream handler  */
		FILE *_logFile;/**< file handler */
};

#endif
