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

//#include "StdAfx.h"
#include "stdafx.h"
#include "LoggerAgent.h"
#include "MicoleStringStream.h"

#include <time.h>

LoggerAgent::LoggerAgent()
: MicoleAgent("LoggerAgent", "Logger")
{
	time_t tim = time (NULL);
	struct tm t;
	gmtime_s(&t, &tim);
	MicoleStringStream s;
	s << "MicoleLog-" << 1900 + t.tm_year << "_" << t.tm_mon << "_" << t.tm_mday << "-" << 
		t.tm_hour << "_" << t.tm_min << "_" << t.tm_sec << ".log";
	_log.open(s.str().c_str());
	bindMessage("(.*)", BUS_CALLBACK_OF(LoggerAgent, handleAllMessage));
}

LoggerAgent::~LoggerAgent()
{
	_log.close();
}

void LoggerAgent::handleAllMessage ( MicoleBus *app, int argc, const char **argv )
{
	if (argc > 0)
	{
		time_t tim = time (NULL);
		struct tm t;
		gmtime_s(&t, &tim);
		_log << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " : " << argv[0] << endl;
	}
}
