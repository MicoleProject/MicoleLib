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

#ifndef MicoleBus_H
#define MicoleBus_H
#include "micolelib.h"

#include "micolebus.h"

//#include "MicoleBusWriteLock.h"

#include <string>
#include <list>
#include <queue>

#include <boost/regex.hpp>

using namespace boost;
using namespace std;

#include <lo2kThread.h>

#include <process.h>
#include <windows.h>

#define MAXREADERS 12

//define MicoleCallBack class defined later
class MicoleCallback;

/** 
 * store regular expression for MicoleBus
 */
struct MicoleBusRegexp
{
	regex _regexp;
	list <MicoleCallback *> _cbList;
};


/** @defgroup core Core
 *  Main core classes
 *  All core classes in "needed" by micoleLib. Please do not modify anything into it.
 *  @{
 */

/**
 * Main MICOLE LIB class.
 * this class define Micole Bus machanism, bind method etc...
 */

class MICOLELIB_API MicoleBus : lo2k::Thread
{
	private:
		bool _sync; /**< define if bus synchronous or not */

		HANDLE _isWriting;

		HANDLE _semLock;

	protected:
		int _regexpNumber; /**< number of regexp (for stat)*/
		int _bindNumber;
		bool _run;

		virtual void run();

		map<string, MicoleBusRegexp> _regexpList; /**< regexp List */
		queue<string> _buffer;


	public:
		/**
		 * Default constructor
		 * @param sync define if bus is synchronous (immediate interpretation) or not
		 */
		MicoleBus(bool sync);
		MicoleBus(const MicoleBus &mb);

		virtual ~MicoleBus();
		
		/**
		 * stop the bus
		 */
		void stop();

		/**
		 * ask interpretation of a string 
		 */
		void interpret(string);
		
		/**
		 * send a string on the bus
		 * @param s string message
		 */
		void sendMsg(string s);

		/**
		 * Bind a message
		 * @param regexp regular expressions
		 * @param cb CallBack method
		 * @see MicoleCallback
		 */
		int bindMsg(string regexp, MicoleCallback * cb);

		void lockSlot() { WaitForSingleObject( _semLock, INFINITE ); }

		void lockAllSlots()
		{
			WaitForSingleObject( _isWriting, INFINITE );
			for (int i = 0; i< MAXREADERS-1; i++)
				lockSlot();
		}

		void unlockSlot() { ReleaseSemaphore(_semLock,1,NULL); }

		void unlockAllSlots()
		{
			ReleaseSemaphore( _isWriting, 1, NULL );
			for (int i = 0; i< MAXREADERS-1; i++)
				unlockSlot();
		}

		/** 
		 * Unbind a MicoleCallback
		 */
		void unbindMsg(MicoleCallback * cb);
};

/**
 * MicoleCallback class
 * abstract class. Define execute virtual function for execution of MicoleCallback
 */
class MicoleCallback
{
	public:
		/**
		 * Empty default constructor
		 */
		MicoleCallback() {};

		/**
		 * Empty default destructor
		 */
		virtual ~MicoleCallback() {};

		/**  
		 * virtual method for callback execution
		 * @param bus MicoleBus (this param stay for Ivy retro compatibility, MicoleBus param is not really needed for normal usage)
		 * @param argc number of argument catched by callback's regular expression
		 * @param argv list of argument as const char array
		 */
		virtual void execute (MicoleBus *bus, int argc, const char **argv) = 0;
};


class MicoleAgent;

/**
 * Abstract template class for MicoleCallback
 * this is a complex class which provide callback system in MicoleBus
 * this class was largely inspired by Ivy source code
 */
template <class T>  class MicoleCallbackOf: public MicoleCallback
{
	public:
		typedef void (T::*MicoleAgentMemberFn) (MicoleBus * bus, int argc , const char ** argv);

		MicoleAgentMemberFn _memberFn;
		T * _obj;

		MicoleCallbackOf(T * obj, MicoleAgentMemberFn fn)
		:_memberFn(fn),_obj(obj)
		{
		}

		MicoleCallbackOf(const MicoleCallbackOf &b)
		:_memberFn(b._memberFn),_obj(b._obj)
		{
		}

		void execute (MicoleBus *bus, int argc, const char **argv)
		{
			(_obj->*_memberFn) (bus, argc, argv);
		}
};

/** @} */ // end core

#define BUS_CALLBACK_OF( cl, m ) new MicoleCallbackOf<cl>( this, (&cl::m) )

#endif
