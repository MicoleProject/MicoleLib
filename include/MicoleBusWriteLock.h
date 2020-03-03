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

#ifndef READWRITELOCK_H_INCLUDED
#define READWRITELOCK_H_INCLUDED

#pragma once

#include "stdafx.h"
#include "windows.h"

/**
 * this file was modified for Micole project
 * original source: http://www.codeguru.com/cpp/w-p/system/misc/article.php/c5679/#more
 */

// MicoleBusReadWriteLock is a read/write mutex.
//
// Any number of readers can acquire the mutex for read (LockRead()).
// Only single writer can acquire the mutex for write (LockWrite()).
// No other writers or readers can hold the mutex while writer uses it.
//
// MicoleBusReadWriteLock does not prefer neither writers nor readers.
// I.e. if both writers and readers are waiting for the lock,
// any of them can grab the lock when it is released by previous owner.
//
// MicoleBusReadWriteLock is recursive, but not upgradable. This means that
//
// if you have a read lock, you can safely acquire another read lock.
// if you have a read lock, you CANNOT acquire a write lock (deadlock!).
//
// if you have a write lock, you can safely acquire another write lock.
// if you have a write lock, you CANNOT acquire a read lock (deadlock!).
//
class MicoleBusReadWriteLock
{
    CMutex m_Access;
    int    m_Readers;
    CMutex m_WriterMutex;
    CEvent m_CanRead;
    CEvent m_CanWrite;

    MicoleBusReadWriteLock( MicoleBusReadWriteLock const& ); // not implemented
    void operator=( MicoleBusReadWriteLock const& ); // not implemented

public:
    MicoleBusReadWriteLock();
    
    bool LockRead( DWORD Timeout = INFINITE );
    bool LockWrite( DWORD Timeout = INFINITE );
    bool UnlockRead( DWORD Timeout = INFINITE );
    bool UnlockWrite( DWORD Timeout = INFINITE );
};


#pragma warning( disable: 4786 )
#include <map>

// MicoleBusReadWriteLockEx is more sofisticated read/write mutex.
// It is recursive and upgradable.
//
// That is, a thread can acquire and release read and write locks in arbitrary order
// The only rule is that every successfully acquired lock must be eventually released
//
// E.g. LockRead(), LockWrite(), UnlockRead(), UnlockWrite() is allowed sequence.
//
//
// MicoleBusReadWriteLockEx prefers writers over readers. E.g., when a writer
// is waiting for the lock, no new readers can acquire the lock. Existing
// readers may continue to use the lock. This policy prevents so-called
// "writer starvation" when writers never get access to the lock because of
// too much readers.
//
class MicoleBusReadWriteLockEx
{
    struct KThreadInfo
    {
        int ReadCount;
        int WriteCount;

        bool IsReader() { return ReadCount > 0; };
        bool IsWriter() { return WriteCount > 0; };
    };

    typedef std::map<DWORD, KThreadInfo> KThreadMap;
    KThreadMap m_Threads;

    // Number of reader threads; 
    // Note: writer threads are not reader threads, even if they have read locks
    int m_TotalReaderThreads; 
    int m_TotalWriterThreads; // current writer + waiting writers

    CMutex m_Access;
    CMutex m_WriterMutex;
    CEvent m_CanRead;
    CEvent m_CanWrite;

    void IncReaders()
        {
            ++m_TotalReaderThreads;
            ResetEvent(m_CanWrite);
        };

    void DecReaders()
        {
            if (--m_TotalReaderThreads == 0)
                SetEvent(m_CanWrite);
        };

    void IncWriters()
        {
            ++m_TotalWriterThreads;
            ResetEvent(m_CanRead);
        };

    void DecWriters()
        {
            if (--m_TotalWriterThreads == 0)
                SetEvent(m_CanRead);
        };

    MicoleBusReadWriteLockEx( MicoleBusReadWriteLockEx const& ); // not implemented
    void operator=( MicoleBusReadWriteLockEx const& ); // not implemented

public:
    MicoleBusReadWriteLockEx();
    
    bool LockRead( DWORD Timeout = INFINITE );
    bool LockWrite( DWORD Timeout = INFINITE );
    bool UnlockRead( DWORD Timeout = INFINITE );
    bool UnlockWrite( DWORD Timeout = INFINITE );

};

// Template class below automatically acquires a read lock in constructor
// and releases it in destructor. It is similar to MFC's CSingleLock class.
// NOTE: unlike CSingleLock, KReadLockTempl will lock the resource by default
// (bInitialLock parameter defaults to true).
//
template <class T>
class KReadLockTempl
{
    bool m_bLocked;
    T&   m_Lock;

    KReadLockTempl( KReadLockTempl const& ); // not implemented
    void operator=( KReadLockTempl const& ); // not implemented


public:
    KReadLockTempl(T& Lock, bool bInitialLock = true)
        :
        m_Lock(Lock),
        m_bLocked(false)
        {
            if (bInitialLock)
            {
                VERIFY(m_Lock.LockRead());
                m_bLocked = true;
            }
        };

    ~KReadLockTempl()
        {
            if (m_bLocked)
                VERIFY(m_Lock.UnlockRead());
        };

    bool Lock(DWORD Timeout = INFINITE)
        {
            ASSERT(!m_bLocked);
            m_bLocked = m_Lock.LockRead(Timeout);
            return m_bLocked;
        };

    bool Unlock(DWORD Timeout = INFINITE)
        {
            ASSERT(m_bLocked);
            m_bLocked = !m_Lock.UnlockRead(Timeout);
            return !m_bLocked;
        }
};

// Template class below automatically acquires a write lock in constructor
// and releases it in destructor. It is similar to MFC's CSingleLock class.
// NOTE: unlike CSingleLock, KWriteLockTempl will lock the resource by default
// (bInitialLock parameter defaults to true).
//
template <class T>
class KWriteLockTempl
{
    bool m_bLocked;
    T&   m_Lock;

    KWriteLockTempl( KWriteLockTempl const& ); // not implemented
    void operator=( KWriteLockTempl const& ); // not implemented


public:
    KWriteLockTempl(T& Lock, bool bInitialLock = true)
        :
        m_Lock(Lock),
        m_bLocked(false)
        {
            if (bInitialLock)
            {
                VERIFY(m_Lock.LockWrite());
                m_bLocked = true;
            }
        };

    ~KWriteLockTempl()
        {
            if (m_bLocked)
                VERIFY(m_Lock.UnlockWrite());
        };

    bool Lock(DWORD Timeout = INFINITE)
        {
            ASSERT(!m_bLocked);
            m_bLocked = m_Lock.LockWrite(Timeout);
            return m_bLocked;
        };

    bool Unlock(DWORD Timeout = INFINITE)
        {
            ASSERT(m_bLocked);
            m_bLocked = !m_Lock.UnlockWrite(Timeout);
            return !m_bLocked;
        }
};

typedef KReadLockTempl<MicoleBusReadWriteLock> KReadLock;
typedef KWriteLockTempl<MicoleBusReadWriteLock> KWriteLock;
typedef KReadLockTempl<MicoleBusReadWriteLockEx> KReadLockEx;
typedef KWriteLockTempl<MicoleBusReadWriteLockEx> KWriteLockEx;


#endif //READWRITELOCK_H_INCLUDED
