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

//#include "stdafx.h"
#include "MicoleBusWriteLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

inline bool Wait( int nObjects, HANDLE* pObjects, DWORD Timeout )
{
    DWORD Result = WaitForMultipleObjects(nObjects, pObjects, true, Timeout);
    if ( DWORD(Result - WAIT_OBJECT_0) < nObjects )
        return true;

    // timeout or error
    return false;
};

inline bool Wait1( HANDLE hObj, DWORD Timeout )
{
    DWORD Result = WaitForSingleObject(hObj, Timeout);
    return (Result == WAIT_OBJECT_0);
};

inline bool Wait2( HANDLE hObj1, HANDLE hObj2, DWORD Timeout )
{
    HANDLE hArray[2] = {hObj1, hObj2};
    return Wait(2, hArray, Timeout);
};

inline bool Wait3( HANDLE hObj1, HANDLE hObj2, HANDLE hObj3, DWORD Timeout )
{
    HANDLE hArray[3] = {hObj1, hObj2, hObj3};
    return Wait(3, hArray, Timeout);
};

MicoleBusReadWriteLock::MicoleBusReadWriteLock()
:
m_CanRead(true, true),   // initially signalled, manual reset
m_CanWrite(true, true),  // ditto
m_Readers(0)
{
};

bool MicoleBusReadWriteLock::LockRead( DWORD Timeout )
{
    if (!Wait2( m_Access, m_CanRead, Timeout))
        return false;

    ++m_Readers;
    ResetEvent(m_CanWrite); // cannot write
    ReleaseMutex(m_Access); // let others access the lock
    return true;
};

bool MicoleBusReadWriteLock::UnlockRead( DWORD Timeout )
{
    if (!Wait1( m_Access, Timeout ))
        return false;


    ASSERT(m_Readers > 0);
    --m_Readers;

    if (m_Readers == 0)
    {
        SetEvent(m_CanWrite);
    };

    ReleaseMutex(m_Access); // let others access the lock
    return true;
};

bool MicoleBusReadWriteLock::LockWrite( DWORD Timeout )
{
    if (!Wait3(m_Access, m_CanWrite, m_WriterMutex, Timeout))
        return false;

    ASSERT(m_Readers == 0);
    ResetEvent(m_CanRead); // cannot read
    ReleaseMutex(m_Access); // let others access the lock

    // note: write mutex remains locked
    return true;
};

bool MicoleBusReadWriteLock::UnlockWrite( DWORD Timeout )
{
    if (!Wait1( m_Access, Timeout ))
        return false;

    ASSERT(m_Readers == 0);

    ReleaseMutex(m_WriterMutex);
    SetEvent(m_CanWrite);
    SetEvent(m_CanRead);
    ReleaseMutex(m_Access); // let others access the lock
    return true;
};    

//**********************************************************
MicoleBusReadWriteLockEx::MicoleBusReadWriteLockEx()
:
m_CanRead(true, true),   // initially signalled, manual reset
m_CanWrite(true, true),  // ditto
m_TotalReaderThreads(0),
m_TotalWriterThreads(0)
{
};

bool MicoleBusReadWriteLockEx::LockRead( DWORD Timeout )
{
    DWORD ThreadId = GetCurrentThreadId();

    // gain access to the map and check if we are already a reader or a writer
    if (!Wait1( m_Access, Timeout ))
        return false;

    KThreadMap::iterator pInfo = m_Threads.find(ThreadId);

    if (pInfo != m_Threads.end())
    {
        // we are in the map
        KThreadInfo& Info = pInfo->second;
        ASSERT( Info.IsReader() || Info.IsWriter() );
        ++Info.ReadCount;
    }
    else
    {
        // we are a new reader
        ReleaseMutex(m_Access);

        if (!Wait2(m_Access, m_CanRead, Timeout))
            return false;

        KThreadInfo Info;
        Info.ReadCount = 1;
        Info.WriteCount = 0;
        m_Threads[ThreadId] = Info;
        IncReaders();
    }

    ReleaseMutex(m_Access);

    return true;
};

bool MicoleBusReadWriteLockEx::UnlockRead( DWORD Timeout )
{
    DWORD ThreadId = GetCurrentThreadId();

    // gain access to the map
    if (!Wait1( m_Access, Timeout ))
        return false;

    KThreadMap::iterator pInfo = m_Threads.find(ThreadId);

    // we **MUST** be in the map    
    ASSERT(pInfo != m_Threads.end());

    KThreadInfo& Info = pInfo->second;
    ASSERT(Info.IsReader());

    --Info.ReadCount;
    if (!Info.IsWriter() && !Info.IsReader())
    {
        DecReaders();
        m_Threads.erase(pInfo);
    }

    ReleaseMutex(m_Access);
    return true;
};

bool MicoleBusReadWriteLockEx::LockWrite( DWORD Timeout )
{
    DWORD ThreadId = GetCurrentThreadId();

    // gain access to the map and check if we are already a reader or a writer
    if (!Wait1( m_Access, Timeout ))
        return false;

    KThreadMap::iterator pInfo = m_Threads.find(ThreadId);

    if (pInfo != m_Threads.end())
    {
        // we are in the map - must be either writer or reader
        KThreadInfo& Info = pInfo->second;
        ASSERT( Info.IsReader() || Info.IsWriter() );

        if (!Info.IsWriter())
        {
            ASSERT(Info.IsReader());

            // Since we are an active reader, there can be no other writers
            // Grab writer mutex - it MUST be available
            VERIFY( Wait1(m_WriterMutex, 0 ) );

            // denounce our reader status
            DecReaders();

            // We are now a potential writer
            // Increment total writers count to include ourselves
            // This also prevents new readers from coming in
            IncWriters();

            if (m_TotalReaderThreads != 0)
            {
                // we are not the only reader
                // wait for others to finish

                // release access and wait for last reader to signal CanWrite event
                ReleaseMutex(m_Access);
                if (!Wait2( m_Access, m_CanWrite, Timeout ))
                {
                    // we did not make it; must wait for access and restore our reader status
                    VERIFY( Wait1(m_Access, INFINITE) );

                    // restore our reader status
                    IncReaders();

                    // withdraw our writer request
                    DecWriters();

                    ReleaseMutex(m_WriterMutex);
                    ReleaseMutex(m_Access);
                    return false;
                }
            }
        }
    }
    else
    {
        // we are not in the map - we are neither writer nor reader

        // Increment total writers count to include ourselves
        // This also prevents new readers from coming in
        IncWriters();

        // wait for readers to finish and for any writers to release writer mutex
        ReleaseMutex(m_Access);

        if (!Wait3(m_Access, m_CanWrite, m_WriterMutex, Timeout))
        {
            // we did not make it - restore everything
            VERIFY( Wait1(m_Access, INFINITE) );
            DecWriters();
            ReleaseMutex(m_Access);
            return false;
        }

        KThreadInfo Info;
        Info.ReadCount = 0;
        Info.WriteCount = 0;

        std::pair<KThreadMap::iterator, bool> InsertResult =
            m_Threads.insert(std::make_pair(ThreadId, Info));

        ASSERT(InsertResult.second);
        pInfo = InsertResult.first;

        // all mutexes are acquired - fall through to IncrementWriteCount
    }

    // we are already a writer - just update the counter
    ++pInfo->second.WriteCount;
    ReleaseMutex(m_Access);
    return true;
};

bool MicoleBusReadWriteLockEx::UnlockWrite( DWORD Timeout )
{
    DWORD ThreadId = GetCurrentThreadId();
    
    // gain access to the map
    if (!Wait1( m_Access, Timeout ))
        return false;

    // we **MUST** be in the map and we must be a writer
    KThreadMap::iterator pInfo = m_Threads.find(ThreadId);
    ASSERT(pInfo != m_Threads.end());

    KThreadInfo& Info = pInfo->second;
    ASSERT(Info.IsWriter());

    --Info.WriteCount;
    if (!Info.IsWriter())
    {
        // we are no longer a writer
        ReleaseMutex(m_WriterMutex);
        DecWriters();

        if (Info.IsReader())
        {
            // restore our reader status
            IncReaders();
        }
        else
        {
            // we don't hold any more locks - remove from the map
            m_Threads.erase(pInfo);
        }
    }

    ReleaseMutex(m_Access);
    return true;
};

