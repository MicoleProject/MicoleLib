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

#ifndef LO2K_THREAD_H
#define LO2K_THREAD_H

#include <vtplayer.h>

#ifdef _WIN32
#ifndef _WINDOWS_
/*#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>*/
#endif
#else
#error _WIN32 must be defined before you include thread.h
#endif
namespace lo2k
{

/**
 * Object encapulsation for windows thread.
 */
class Thread
{
	#ifdef _WIN32
		static DWORD WINAPI ThreadFunc(LPVOID pv)
		{
		try
		{
			(reinterpret_cast<Thread *>(pv))->run();
		}
		catch(...)
		{
		}
		return 0;
		}
	#elif defined(__sun)
	#else
	#endif
	public:
		#ifdef _WIN32
			typedef DWORD threadid;
		#elif defined(__sun)
		#else
		#endif
		
		/**
		 * Default constructor
		 */
		Thread()
		{
		}
		
		/**
		 * Default destructor
		 */
		virtual ~Thread()
		{
		}

		/**
		 * return thread identifier
		 * @return identifier
		 */
		static threadid getthreadid()
		{
			#ifdef _WIN32
			return ::GetCurrentThreadId();
			#elif defined(__sun)
			#else
			#endif
		}

		/**
		 * sleep the thread
		 * @param milliseconds milliseconds
		 */
		static void sleep(long milliseconds=1)
		{
			#ifdef _WIN32
			::Sleep(milliseconds);
			#elif defined(__sun)
			#else
			#endif
		}

		/**
		 * start the thread
		 */
		threadid start()
		{
			threadid id;
			#ifdef _WIN32
			::CreateThread(NULL, 0, ThreadFunc, this, 0, &id);
			#elif defined(__sun)
			#else
			#endif
			return id;
		}
		
		/**
		 * run method
		 * you need to <b>implement</b> these function if you inherit lo2k.net::Thread
		 * this function must be a loop
		 */
		virtual void run()=0;
};



/**
 * CriticalSection class for lo2k::Thread
 */
class VTPLAYER_API CriticalSection
{
	#ifdef _WIN32
	typedef CRITICAL_SECTION cs;
	#elif defined(__sun)
	#else
	#endif
private:

	cs m_cs;
	static CriticalSection *_instance;


	CriticalSection()
	{
		#ifdef _WIN32
		::InitializeCriticalSection(&m_cs);
		#elif defined(__sun)
		#else
		#endif
	}

public:

	static CriticalSection * get()
	{
		if (_instance == NULL)
			_instance = new CriticalSection;
		return _instance;
	}


	~CriticalSection()
	{
		#ifdef _WIN32
		::DeleteCriticalSection(&m_cs);
		#elif defined(__sun)
		#else
		#endif
	}
	void enter()
	{
		#ifdef _WIN32
		::EnterCriticalSection(&m_cs);
		#elif defined(__sun)
		#else
		#endif
	}
	void leave()
	{
		#ifdef _WIN32
		::LeaveCriticalSection(&m_cs);
		#elif defined(__sun)
		#else
		#endif
	}

};



};

#endif