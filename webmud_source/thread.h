// Copyright (C) 1999-2000 Open Source Telecom Corporation.
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
// As a special exception to the GNU General Public License, permission is 
// granted for additional uses of the text contained in its release 
// of Common C++.
// 
// The exception is that, if you link the Common C++ library with other
// files to produce an executable, this does not by itself cause the
// resulting executable to be covered by the GNU General Public License.
// Your use of that executable is in no way restricted on account of
// linking the Common C++ library code into it.
// 
// This exception does not however invalidate any other reasons why
// the executable file might be covered by the GNU General Public License.
// 
// This exception applies only to the code released under the 
// name Common C++.  If you copy code from other releases into a copy of
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
// 
// If you write modifications of your own for Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.  
#ifdef __WIN32__

#ifndef	__CCXX_THREAD_H__
#define	__CCXX_THREAD_H__

#include <setjmp.h>

#ifndef	__CCXX_CONFIG_H__
#include "config.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif

typedef	DWORD	cctid_t;
typedef DWORD   timeout_t;

#define	MAX_SEM_VALUE	1000000

#define	ENTER_CRITICAL	EnterMutex();
#define	LEAVE_CRITICAL	LeaveMutex();

typedef	enum {
	THREAD_CANCEL_IMMEDIATE = 1,
	THREAD_CANCEL_DEFERRED,
	THREAD_CANCEL_DEFAULT = THREAD_CANCEL_DEFERRED,
	THREAD_CANCEL_DISABLED,
	THREAD_CANCEL_INITIAL
} thread_cancel_t;

class Thread;
#define sleep(x) delay(x * 1000l)
#undef Yield

#if defined(__MINGW32__) || defined(__CYGWIN32__)

#include <Windows32/CommonFunctions.h>
#else
__declspec(dllimport) long __stdcall InterlockedIncrement(long *);
__declspec(dllimport) long __stdcall InterlockedDecrement(long *);
__declspec(dllimport) long __stdcall InterlockedExchange(long *, long);
__declspec(dllimport) long __stdcall InterlockedExchangeAdd(long *, long);

#endif

BOOL	__EXPORT APIENTRY DllMain(HANDLE hInstance, DWORD dwReason, LPVOID lpReserved);
Thread	__EXPORT *getThread(void);
DWORD	__EXPORT waitThread(HANDLE hRef, timeout_t timeout);
void	__EXPORT yield(void);
void	__EXPORT delay(timeout_t timeout);

class __EXPORT Mutex 
{
private:
	CRITICAL_SECTION _mutex;

public:
	~Mutex(void)
		{DeleteCriticalSection(&_mutex);};
	
	Mutex(void)
		{InitializeCriticalSection(&_mutex);};

	inline void EnterMutex(void)
		{EnterCriticalSection(&_mutex);};

	inline void LeaveMutex(void)
		{LeaveCriticalSection(&_mutex);};

	inline void TryEnterMutex(void)
		{TryEnterCriticalSection(&_mutex);};

	friend inline void operator ++(Mutex &m)
		{EnterCriticalSection(&(m._mutex));};

	friend inline void operator --(Mutex &m)
		{LeaveCriticalSection(&(m._mutex));};
};

class __EXPORT MutexCounter : public Mutex 
{
private:
	int counter;

public:
	MutexCounter();

	friend int operator++(MutexCounter &);
	friend int operator--(MutexCounter &);
};

class __EXPORT AtomicCounter
{
private:
	long atomic;

public:
	inline AtomicCounter()
		{atomic = 0;};

	inline AtomicCounter(int value)
		{atomic = value;};

	inline int operator++(void)
		{return InterlockedIncrement(&atomic);};

	inline int operator--(void)
		{return InterlockedDecrement(&atomic);};

	int operator+=(int change);

	int operator-=(int change);

	inline int operator+(int change)
		{return atomic + change;};

	inline int operator-(int change)
		{return atomic - change;};
	
	inline int operator=(int value)
		{return InterlockedExchange(&atomic, value);};

	inline bool operator!(void)
		{return atomic == 0;};

	inline operator int()
		{return atomic;};
};

class __EXPORT Event 
{
private:
	HANDLE cond;

public:
	Event();

	~Event()
		{CloseHandle(cond);};

	inline void Reset(void)
		{ResetEvent(cond);};

	inline void Signal(void)
		{SetEvent(cond);};

	inline bool Wait(void)
		{return (waitThread(cond, INFINITE) == WAIT_OBJECT_0);};

	inline void Wait(timeout_t timer)
		{waitThread(cond, timer);};

	friend inline void reset(Event &ev)
		{ResetEvent(ev.cond);};

	friend inline void signal(Event &ev)
		{SetEvent(ev.cond);};

	friend inline void wait(Event &ev)
		{waitThread(ev.cond, INFINITE);};

	friend inline void wait(Event &ev, timeout_t timer)
		{waitThread(ev.cond, timer);};
};

class __EXPORT Semaphore 
{
private:
	HANDLE	semObject;

public:
	~Semaphore()
		{CloseHandle(semObject);};

	Semaphore(size_t count = 0)
		{semObject = CreateSemaphore((LPSECURITY_ATTRIBUTES)NULL, count, MAX_SEM_VALUE, (LPCTSTR)NULL);};

	void Post(void)
		{ReleaseSemaphore(semObject, 1, (LPLONG)NULL);};

	void Wait(void)
		{waitThread(semObject, INFINITE);};
};

class __EXPORT Buffer 
{
private:
	Mutex lock_head, lock_tail;
	Semaphore size_head, size_tail;
	size_t _size, _used;

protected:
	virtual int OnWait(void *buf) = 0;
	virtual int OnPost(void *buf) = 0;
	virtual int OnPeek(void *buf) = 0;

public:
	Buffer(size_t capacity);

	virtual ~Buffer()
		{return;};

	inline size_t getSize(void)
		{return _size;};

	inline size_t getUsed(void)
		{return _used;};

	int Wait(void *buf);
	int Post(void *buf);
	int Peek(void *buf);
};

class __EXPORT FixedBuffer : public Buffer 
{
private:
	char *buf, *head, *tail;
	size_t objsize;

protected:
	int OnPeek(void *data);
	int OnWait(void *data);
	int OnPost(void *data);

public:
	FixedBuffer(size_t capacity, size_t objsize);
	~FixedBuffer();
};

class __EXPORT Thread 
{
private:
	static Thread *_main;
	Thread *_parent;
	DWORD _tid;
	HANDLE _cancellation;
	thread_cancel_t	_cancel;
	jmp_buf _env;
	Semaphore *_start;

	static unsigned __stdcall Execute(Thread *th);

protected:
	HANDLE	_hThread;

	virtual void Initial(void)
		{return;};

	virtual void Final(void)
		{return;};

	virtual void Run(void) = 0;

	inline void Exit(void)
		{longjmp(_env, 1);};

	void setCancel(thread_cancel_t cancel);
	void Yield(void);
	void Terminate(void);
	void Sleep(timeout_t timeout);
	DWORD WaitHandle(HANDLE obj, timeout_t timeout);

public:
	inline Thread *getParent(void)
		{return _parent;};

	inline thread_cancel_t getCancel(void)
		{return _cancel;};

	inline bool isRunning(void)
		{return (_tid != 0);};

	inline bool isThread(void)
		{return (_tid == GetCurrentThreadId());};

	Thread(Semaphore *start = (Semaphore *)NULL, int pri = 0, unsigned stack = 0);
	Thread(bool flag);

	int Start(Semaphore *sem = (Semaphore *)NULL);

	virtual ~Thread()
		{Terminate();};

	virtual void *getExtended(void)
		{return NULL;};

	virtual void Notify(Thread *th)
		{return;};

	friend inline int start(Thread &th, Semaphore *sem = (Semaphore *)NULL)
		{return th.Start(sem);};

	friend inline void operator++(Thread &th)
		{th._start->Post();};

	friend DWORD waitThread(HANDLE hRef, timeout_t timeout);
	friend void delay(timeout_t timeout);
	friend void yield(void);
};

class ThreadKey 
{
private:
	DWORD	key;

public:
	ThreadKey();
	~ThreadKey();

	void *getKey(void);
	void setKey(void *key);

	friend inline void *getKey(ThreadKey &th)
		{return th.getKey();};

	friend inline void setKey(ThreadKey &th, void *ptr)
		{th.setKey(ptr);};
};

inline int get(Buffer &b, void *o)
	{return b.Wait(o);};

inline int put(Buffer &b, void *o)
	{return b.Post(o);};

inline int peek(Buffer &b, void *o)
	{return b.Peek(o);};

inline void operator++(Semaphore &s)
	{s.Post();};

inline void operator--(Semaphore &s)
	{s.Wait();};

#endif
#else //__WIN32__
#include "thread_unix.h"
#endif //__WIN32__ 

