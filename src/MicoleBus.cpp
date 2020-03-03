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
#include "MicoleBus.h"
#include "MicoleAgent.h"
#include "NetworkAgent.h"

#include "MicoleStringStream.h"

#include "Lo2kThread.h"

using namespace std;


MicoleBus::MicoleBus(bool sync = true)
{
	_semLock = CreateSemaphore( NULL, MAXREADERS, MAXREADERS, NULL );
	_isWriting = CreateSemaphore( NULL, 1, 1, NULL );
	_regexpNumber = 0;
	_bindNumber = 0;
	_run = true;
	_sync = sync;

	if (!_sync)
		start();
}

MicoleBus::~MicoleBus()
{
}

void MicoleBus::run()
{

	while(_run) 
	{
		#ifdef _FULL_DEBUG
/*		char *buffer = new char[10];
		_itoa_s(_buffer.size(),buffer,9,10);
		OutputDebugString((string(buffer)+"\n").c_str());
		delete []buffer;*/
		#endif
		
		while (_buffer.size() > 0)  //while buffer is not empty
		{
			//retrieve first message
			string message;
			lo2k::CriticalSection * cs = lo2k::CriticalSection::get(); //put critical section
			cs->enter();
			//remove first element from stl list
			message = _buffer.front(); 
			_buffer.pop();
			cs->leave();	

			//intepretation of message
			this->interpret(message); 
		}
	}
}

void MicoleBus::interpret(string message)
{
	/**
	 * crawl all regular expression registered in micole bus
	 * each regular expression as a list of callbacks (to avoir reparse of similar regulars expressions)
	 */
	lockSlot();
	for (map<string,MicoleBusRegexp>::iterator i = _regexpList.begin(); i != _regexpList.end(); i++)
	{
		#ifdef _FULL_DEBUG
		//OutputDebugString(((*i).second._regexp+"\n").c_str());
		#endif	

		//create a  cmatch objet (see boost regexp usage)
		cmatch what;
		if(regex_match(message.c_str(), what, (*i).second._regexp))
		{
			int whatSize = (int) what.size();
			char **  pArgv = new char*[whatSize];
			
			//foreach catched string by regular expression
			for (int j = 1; j < whatSize; j++)
			{
				//allocate arg
				int argvSize = what[j].second - what[j].first + 1;
				pArgv[j - 1] = (char *) malloc(sizeof(char) * argvSize);
				
				//extract parsed chain
				memcpy(pArgv[j-1],what[j].first,argvSize-1);
				pArgv[j-1][argvSize-1]=0; //add 0 terminal value
			}

			//apply to all callback
			for (list<MicoleCallback*>::iterator k = (*i).second._cbList.begin() ; k != (*i).second._cbList.end() ; k++)
				(*k)->execute(this, whatSize - 1, (const char **)pArgv);
			
			//free
			for (int j = 1; j < whatSize; j++)
				free(pArgv[j-1]);
			
			delete []pArgv;
		}
	}
	unlockSlot();
}

void MicoleBus::stop()
{
	_run = false;
}

/**
 * copy constructor
 */
MicoleBus::MicoleBus (const MicoleBus &mb)
{
}

int MicoleBus::bindMsg(string regexp, MicoleCallback * cb)
{
	lo2k::CriticalSection * cs = lo2k::CriticalSection::get();
	
	lockAllSlots();
	if(_regexpList.find(regexp) == _regexpList.end())
	{
		MicoleBusRegexp elt;
		elt._regexp = regex(regexp);
		elt._cbList.push_back(cb);
	
		cs->enter();
		_regexpList[regexp]=elt;
		cs->leave();
		_regexpNumber++;
		_bindNumber++;
	}
	else //entry already exist, add callback to callback list
	{
		_regexpList[regexp]._cbList.push_back(cb);
		_bindNumber++;
	}
	unlockAllSlots();

	MicoleStringStream ms;
	ms << "bind number:" << _bindNumber << " regexp number: " << _regexpNumber << " regexp: " << regexp <<"\n";

	OutputDebugStringA(ms.str().c_str());
	return 0;
}

void MicoleBus::unbindMsg(MicoleCallback * cb) {
	lockAllSlots();
	for (map<string,MicoleBusRegexp>::iterator i = _regexpList.begin() ; i != _regexpList.end() ; i++)
		(*i).second._cbList.remove(cb);

	unlockAllSlots();
}

void MicoleBus::sendMsg(string s) 
{
	if (_sync)  
		this->interpret(s);
	else
	{
		lo2k::CriticalSection * cs = lo2k::CriticalSection::get();
		cs->enter();
		_buffer.push(s);
		cs->leave();
	}
}
