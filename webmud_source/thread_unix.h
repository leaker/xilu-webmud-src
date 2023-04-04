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
//

#ifndef	__CCXX_THREAD_H__
#define	__CCXX_THREAD_H__
#define	__CCXX_POSIX

#ifndef	_REENTRANT
#define	_REENTRANT
#endif

#ifndef	_THREAD_SAFE
#define	_THREAD_SAFE
#endif

#ifndef __CCXX_CONFIG_H__
#ifdef	PACKAGE
#undef	PACKAGE
#endif
#ifdef	VERSION
#undef	VERSION
#endif
#include "config.h"
#endif

#ifndef	HAVE_PTHREAD_H
#include <pthread.h>
#include <semaphore.h>
#endif

#ifndef	__CCXX_MACROS_H__
#include "macros.h"
#else
#ifdef	__CCXX_NAMESPACE_H__
#include "macros.h"
#endif
#endif

#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

#ifdef	__linux__
#define _SIG_THREAD_STOPCONT
#define	_SIG_THREAD_ALARM
#endif

typedef	pthread_t	cctid_t;
typedef	unsigned long	timeout_t;
typedef	int		signo_t;

// use a define so that if the sys/types.h header already defines caddr_t
// as it may on BSD systems, we do not break it by redefining again.

#undef caddr_t
#define	caddr_t		char *

#define	ENTER_CRITICAL	EnterMutex();
#define	LEAVE_CRITICAL	LeaveMutex();
#define	ENTER_DEFERRED	setCancel(THREAD_CANCEL_DEFERRED);
#define LEAVE_DEFERRED 	setCancel(THREAD_CANCEL_IMMEDIATE);

// These macros override common functions with thread-safe versions. In
// particular the common "libc" sleep() has problems since it normally
// uses SIGARLM (as actually defined by "posix").  The pthread_delay and
// usleep found in libpthread are gaurenteed not to use SIGALRM and offer
// higher resolution.  psleep() is defined to call the old process sleep.

#define	sleep(x)	ccxx_sleep((x) * 1000)
#define	yield()		ccxx_yield()
#define	psleep(x)	(sleep)(x)

typedef enum
{
	THREAD_CANCEL_INITIAL=0,
	THREAD_CANCEL_DEFERRED=1,
	THREAD_CANCEL_IMMEDIATE,
	THREAD_CANCEL_DISABLED,
	THREAD_CANCEL_DEFAULT=THREAD_CANCEL_DEFERRED,
	THREAD_CANCEL_INVALID
} thread_cancel_t;

typedef enum
{
	THREAD_SUSPEND_ENABLE,
        THREAD_SUSPEND_DISABLE
} thread_suspend_t;

#define	 THREAD_SIGNAL_BLOCKED	false
#define	 THREAD_SIGNAL_UNBLOCK	true
	
#ifdef	_SIG_THREAD_STOPCONT
#define _SIG_THREAD_SUSPEND SIGSTOP
#define _SIG_THREAD_RESUME  SIGCONT
#else
#ifndef	SIGUSR3
#ifdef	SIGWINCH
#define	SIGUSR3	SIGWINCH
#else
#define	SIGUSR3	SIGINT
#endif
#endif
#define	_SIG_THREAD_SUSPEND SIGUSR3
#define _SIG_THREAD_RESUME SIGUSR3
#endif

class Thread;

Thread *getThread(void);


/**
 * The ThreadLock class impliments a thread rwlock for optimal reader performance
 * on systems which have rwlock support, and reverts to a simple mutex for those
 * that do not.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short Posix rwlock extension for protected access.
 */
class ThreadLock
{
private:
#undef HAVE_PTHREAD_RWLOCK
#ifdef HAVE_PTHREAD_RWLOCK
	pthread_rwlock_t _lock;
#else
	pthread_mutex_t _lock;
#endif

public:
	/**
	 * Create a process shared thread lock object.
	 */
	ThreadLock();

	/**
	 * Destroy a process shared thread lock object.
	 */
	~ThreadLock();

	/**
	 * Aquire a read lock for the current object.
	 */
	void ReadLock(void);

	/**
	 * Aquire a write lock for the current object.
	 */
	void WriteLock(void);

	/**
	 * Attempt read lock for current object.
	 *
	 * @return true on success.
	 */
	bool TryReadLock(void);

	/**
	 * Attempt write lock for current object.
	 *
	 * @return true on success.
	 */
	bool TryWriteLock(void);

	/**
	 * Release any held locks.
	 */
	void Unlock(void);
};

/**
 * The Mutex class is used to protect a section of code so that at any
 * given time only a single thread can perform the protected operation.
 * The Mutex is always recursive in that if the same thread invokes
 * the same mutex lock multiple times, it must release it multiple times.
 * This allows a function to call another function which also happens to
 * use the same mutex lock when called directly.
 * 
 * The Mutex can be used as a base class to protect access in a derived
 * class.  When used in this manner, the ENTER_CRITICAL and LEAVE_CRITICAL
 * macros can be used to specify when code written for the derived class
 * needs to be protected by the default Mutex of the derived class, and
 * hence is presumed to be 'thread safe' from multiple instance execution.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short Mutex lock for protected access.
 */
class Mutex 
{
private:
#ifndef	PTHREAD_MUTEXTYPE_RECURSIVE
	int _level;
#endif

protected:
	/**
	 * Pthread mutex object.  This is protected rather than private
	 * because some mixed mode pthread operations require a mutex as
	 * well as their primary pthread object.  A good example of this
	 * is the Event class, as waiting on a conditional object must be
	 * associated with an accessable mutex.  An alternative would be
	 * to make such classes "friend" classes of the Mutex.
	 */
	pthread_mutex_t	_mutex;

public:
	/**
	 * The mutex is always initialized as a recursive entity.
	 */
	Mutex();

	/**
	 * Destroying the mutex removes any system resources associated
	 * with it.  If a mutex lock is currently in place, it is presumed
	 * to terminate when the Mutex is destroyed.
	 */
	~Mutex()
		{pthread_mutex_destroy(&_mutex);};

	/**
	 * Entering a Mutex locks the mutex for the current thread.  This
	 * also can be done using the ENTER_CRITICAL macro or by using the
	 * ++ operator on a mutex.
	 * 
	 * @see #LeaveMutex
	 */
#ifdef	PTHREAD_MUTEXTYPE_RECURSIVE
	inline void EnterMutex(void)
		{pthread_mutex_lock(&_mutex);};
#else
	void	EnterMutex(void);
#endif

	/**
	 * Tries to lock the mutex for the current thread. Behaves like
	 * #EnterMutex , except that it doesn't block the calling thread
	 * if the mutex is already locked by another thread.
	 *
	 * @return true if locking the mutex was succesful otherwise false
	 *
	 * @see EnterMutex
	 * @see LeaveMutex
	 */
#ifdef	PTHREAD_MUTEXTYPE_RECURSIVE
	inline bool TryEnterMutex(void)
		{ return (pthread_mutex_trylock(&_mutex) == 0) ? true : false; }
#else
	bool TryEnterMutex(void);
#endif

	/**
	 * Leaving a mutex frees that mutex for use by another thread.  If
	 * the mutex has been entered (invoked) multiple times (recursivily)
	 * by the same thread, then it will need to be exited the same number
	 * of instances before it is free for re-use.  This operation can
	 * also be done using the LEAVE_CRITICAL macro or by the -- operator
	 * on a mutex.
	 * 
	 * @see #EnterMutex
	 */
#ifdef	PTHREAD_MUTEXTYPE_RECURSIVE
	inline void LeaveMutex(void)
		{pthread_mutex_unlock(&_mutex);};
#else
	void LeaveMutex(void);
#endif
};

/**
 * The Mutex Counter is a counter variable which can safely be incremented
 * or decremented by multiple threads.  A Mutex is used to protect access
 * to the counter variable (an integer).  An initial value can be specified 
 * for the counter, and it can be manipulated with the ++ and -- operators.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short Thread protected integer counter.
 */
class MutexCounter : public Mutex
{
private:
	int	counter;

public:
	MutexCounter(int initial = 0);
	
	friend int operator ++(MutexCounter &mc);
	friend int operator --(MutexCounter &mc);
};

/**
 * The AtomicCounter class offers thread-safe manipulation of an integer
 * counter.  These are commonly used for building thread-safe "reference"
 * counters for C++ classes.  The AtomicCounter depends on the platforms
 * support for "atomic" integer operations, and can alternately substitute
 * a "mutex" if no atomic support exists.
 *
 * @author Sean Cavanaugh <sean@dimensionalrift.com>
 * @short atomic counter operation.
 */
class AtomicCounter
{
private:
#ifdef	HAVE_ATOMIC
	atomic_t atomic;
#else
	int counter;
	Mutex lock;
#endif

public:
	/**
	 * Initialize an atomic counter to 0.
	 */
	AtomicCounter();

	/**
	 * Initialize an atomic counter to a known value.
	 *
	 * @param initial value.
	 */
	AtomicCounter(int value);

	int operator++(void);
	int operator--(void);
	int operator+=(int change);
	int operator-=(int change);
	int operator+(int change);
	int operator-(int change);
	int operator=(int value);
	bool operator!(void);
	operator int();
};

/**
 * A semaphore is generally used as a synchronization object between multiple
 * threads or to protect a limited and finite resource such as a memory or
 * thread pool.  The semaphore has a counter which only permits access by
 * one or more threads when the value of the semaphore is non-zero.  Each
 * access reduces the current value of the semaphore by 1.  One or more
 * threads can wait on a semaphore until it is no longer 0, and hence the
 * semaphore can be used as a simple thread synchronization object to enable
 * one thread to pause others until the thread is ready or has provided data
 * for them.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short Semaphore counter for thread synchronization. 
 */
class Semaphore
{
protected:
	sem_t _semaphore;

public:
	/**
	 * The initial value of the semaphore can be specified.  An initial
	 * value is often used When used to lock a finite resource or to 
	 * specify the maximum number of thread instances that can access a 
	 * specified resource.
	 * 
	 * @param resource specify initial resource count or 0 default.
	 */
	Semaphore(size_t resource = 0);

	/**
	 * Destroying a semaphore also removes any system resources
	 * associated with it.  If a semaphore has threads currently waiting
	 * on it, those threads will all continue when a semaphore is
	 * destroyed.
	 */
	~Semaphore()
#ifndef	__linux__
		{sem_destroy(&_semaphore);}
#endif
	;

	/**
	 * Wait is used to keep a thread held until the semaphore counter
	 * is greater than 0.  If the current thread is held, then another
	 * thread must increment the semaphore.  Once the thread is accepted, 
	 * the semaphore is automatically decremented, and the thread 
	 * continues execution.
	 * 
	 * The pthread semaphore object does not support a timed "wait", and
	 * hence to maintain consistancy, neither the posix nor win32 source
	 * trees support "timed" semaphore objects.
	 * 
	 * @see #Post
	 */
	void Wait(void)
#ifndef	__linux__
		{sem_wait(&_semaphore);}
#endif
	;

	/**
	 * TryWait is a non-blocking variant of Wait. If the semaphore counter
	 * is greater than 0, then the thread is accepted and the semaphore
	 * counter is decreased. If the semaphore counter is 0 TryWait returns
	 * immediately with false.
	 *
	 * @return true if thread is accepted otherwise false
	 *
	 * @see #Wait
	 * @see #Post
	 */
	bool TryWait(void)
#ifndef	__linux__
		{ return ( sem_trywait(&_semaphore) == 0 ) ? true : false; }
#endif
	;
	
	/**
	 * Posting to a semaphore increments its current value and releases
	 * the first thread waiting for the semaphore if it is currently at
	 * 0.  Interestingly, there is no support to increment a semaphore by
	 * any value greater than 1 to release multiple waiting threads in
	 * either pthread or the win32 API.  Hence, if one wants to release
	 * a semaphore to enable multiple threads to execute, one must perform
	 * multiple post operations.
	 * 
	 * @see #Wait
	 */
	void Post(void)
#ifndef	__linux__
		{sem_post(&_semaphore);}
#endif
	;
};

/**
 * The Event class implements a feature originally found in the WIN32 API;
 * event notification.  A target thread waits on a resetable Event, and one
 * or more other threads can then signal the waiting thread to resume 
 * execution.  A timeout can be used to specify a wait duration in 
 * milliseconds.  The Event class must be reset before it can be used again 
 * as a trigger.
 * 
 * @author: David Sugar <dyfet@ostel.com>
 * @short Thread synchornization on event notification.
 */
class Event : public Mutex
{
protected:
	pthread_cond_t _cond;
	bool _signaled;
	int _count;

public:
	Event();

	~Event()
		{pthread_cond_destroy(&_cond);};
	
	/**
	 * Once signaled, the Event class must be "reset" before responding
	 * to a new signal.
	 * 
	 * @see #Signal
	 */
	void Reset(void)
		{_signaled = false;};

	/**
	 * Signal the event for the waiting thread.
	 */
	void Signal(void);
	/**
	 * Wait either for the event to be signaled by another thread or
	 * for the specified timeout duration.
	 * 
	 * @see #Signal
	 * @return true if signaled, false if timed out.
	 * @param timer timeout in milliseconds to wait for a signal.
	 */
	bool Wait(timeout_t timer = 0);
};

/**
 * The buffer class represents an IPC service that is built upon a buffer
 * of fixed capacity that can be used to transfer objects between one or
 * more producer and consumer threads.  Producer threads post objects
 * into the buffer, and consumer threads wait for and receive objects from
 * the buffer.  Semaphores are used to to block the buffer from overflowing
 * and indicate when there is data available, and mutexes are used to protect
 * multiple consumers and producer threads from stepping over each other.
 * 
 * The buffer class is an abstract class in that the actual data being
 * buffered is not directly specified within the buffer class itself.  The
 * buffer class should be used as a base class for a class that actually
 * impliments buffering and which may be aware of the data types actually
 * are being buffered.  A template class could be created based on buffer
 * for this purpose.  Another possibility is to create a class derived
 * from both Thread and Buffer which can be used to implement message passing
 * threads.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short Producer/Consumer buffer for use between threads.
 */
class Buffer
{
private:
	Mutex lock_head, lock_tail;
	Semaphore size_head, size_tail;
	size_t _size;
	size_t _used;

protected:
	/**
	 * Invoke derived class buffer peeking method.
	 * @return size of object found.
	 * @param buf pointer to copy contents of head of buffer to.
	 */
	virtual int OnPeek(void *buf) = 0;
	/**
	 * Invoke derived class object request from buffer.
	 * @return size of object returned.
	 * @param buf pointer to hold object returned from the buffer.
	 */
	virtual int OnWait(void *buf) = 0;
	/**
	 * Invoke derived class posting of object to buffer.
	 * @return size of object posted.
	 * @param buf pointer to object being posted to the buffer.
	 */
	virtual int OnPost(void *buf) = 0;

public:
	/**
	 * Create a buffer object of known capacity.
	 * @param capcity is the integer capacity of the buffer.
	 */
	Buffer(size_t capacity);
	/**
	 * In derived functions, may be used to free the actual memory
	 * used to hold buffered data.
	 */
	virtual ~Buffer()
		{return;};

	/**
	 * Return the capacity of the buffer as specified at creation.
	 * @return size of buffer.
	 */
	inline size_t getSize(void)
		{return _size;};
	
	/**
	 * Return the current capacity in use for the buffer.  Free space
	 * is technically getSize() - getUsed().
	 * @return integer used capacity of the buffer.
	 * @see #getSize
	 */
	inline size_t getUsed(void)
		{return _used;};

	/**
	 * Let one or more threads wait for an object to become available
	 * in the buffer.  The waiting thread(s) will wait forever if no
	 * object is ever placed into the buffer.
	 * 
	 * @return size of object passed by buffer in bytes.
	 * @param buf pointer to store object retrieved from the buffer.
	 */
	int Wait(void *buf);
	/**
	 * Post an object into the buffer and enable a waiting thread to
	 * receive it.
	 * 
	 * @return size of object posted in bytes.
	 * @param buf pointer to object to store in the buffer.
	 */
	int Post(void *buf);
	/**
	 * Peek at the current content (first object) in the buffer.
	 * 
	 * @return size of object in the buffer.
	 * @param buf pointer to store object found in the buffer.
	 */
	int Peek(void *buf);
};

/**
 * A buffer class that holds a known capacity of fixed sized objects defined
 * during creation.
 * 
 * @author David Sugar <dyfet@ostel.com>.
 * @short producer/consumer buffer for fixed size objects.
 */
class FixedBuffer : public Buffer
{
private:
	char *buf, *head, *tail;
	size_t objsize;

protected:
	/**
	 * Return the first object in the buffer.
	 * @return predefined size of this buffers objects.
	 * @param buf pointer to copy contents of head of buffer to.
	 */
	int OnPeek(void *buf);
	/**
	 * Wait for and return a fixed object in the buffer.
	 * @return predefined size of this buffers objects.
	 * @param buf pointer to hold object returned from the buffer.
	 */
	int OnWait(void *buf);
	/**
	 * Post an object of the appropriate size into the buffer.
	 * @return predefined size of this buffers objects.
	 * @param buf pointer to data to copy into the buffer.
	 */
	int OnPost(void *buf);	

public:
	/**
	 * Create a buffer of known capacity for objects of a specified
	 * size.
	 * 
	 * @param capacity of the buffer.
	 * @param objsize for each object held in the buffer.
	 */
	FixedBuffer(size_t capacity, size_t objsize);
	/**
	 * Create a copy of an existing fixed size buffer and duplicate
	 * it's contents.
	 * 
	 * @param fb existing FixedBuffer object.
	 */
	FixedBuffer(const FixedBuffer &fb);
	/**
	 * Destroy the fixed buffer and free the memory used to store objects.
	 */
	~FixedBuffer();

	FixedBuffer &operator=(const FixedBuffer &fb);
};

/**
 * Every thread of execution in an application is created by deriving
 * a unique class from the Thread class and by implementing the Run
 * method.  The base Thread class supports encapsulation of the generic
 * threading methods implemented on various target operating systems.  
 * This includes the ability to start and stop threads in a synchronized
 * and controllable manner, the ability to specify thread execution priority,
 * and thread specific "system call" wrappers, such as for sleep and yield.
 * A thread exception is thrown if the thread cannot be created.
 * 
 * @author David Sugar <dyfet@tycho.com>
 * @short base class used to derive all threads of execution.
 */
class Thread
{
private:
	static Thread *_main;

#ifndef	_SIG_THREAD_ALARM
	static Thread *_timer;
	static Mutex _arm;
#endif
	
	Thread *_parent;
	pthread_t _tid;
	pthread_attr_t _attr;
	thread_cancel_t	_cancel;
	jmp_buf	_env;
	time_t	_alarm;
	Semaphore *_start;

	static	void Execute(Thread *th);
	static	void SignalHandler(int signo);

protected:
       	/**
	 * All threads execute by deriving the Run method of Thread.
	 * This method is called after Initial to begin normal operation
	 * of the thread.  If the method terminates, then the thread will
	 * also terminate after notifying it's parent and calling it's
	 * Final() method.
	 * 
	 * @see #Initial
	 */
	virtual void Run(void) = 0;

	/**
	 * This method is called for the very first instance of a new thread
	 * being created in a multi-threaded application.  Hence, it is only
	 * called once, and by the derived Thread class that happens to be
	 * created first.
	 */
	virtual void First(void)
		{return;};
	
	/**
	 * A thread that is self terminating, either by invoking Exit() or
	 * leaving it's Run(), will have this method called.  It can be used
	 * to self delete the current object assuming the object was created
	 * with new on the heap rather than stack local, hence one may often
	 * see Final defined as "delete this" in a derived thread class.  A
	 * Final method, while running, cannot be terminated or cancelled by
	 * another thread.
	 * 
	 * @see #Exit
	 * @see #Run
	 */
	virtual void Final(void)
		{return;};

	/**
	 * The initial method is called by a newly created thread when it
	 * starts execution.  This method is ran with deferred cancellation
	 * disabled by default.  The Initial method is given a seperate
	 * handler so that it can create temporary objects on it's own
	 * stack frame, rather than having objects created on Run() that
	 * are only needed by startup and yet continue to consume stack space.
	 * 
	 * @see #Run
	 */
	virtual void Initial(void)
		{return;};
	
	/**
	 * Since getParent() and getThread() only refer to an object of the
	 * Thread "base" type, this virtual method can be replaced in a
	 * derived class with something that returns data specific to the
	 * derived class that can still be accessed through the pointer
	 * returned by getParent() and getThread().
	 *
	 * @return pointer to derived class specific data.
	 */
	virtual void *getExtended(void)
		{return NULL;};

	/**
	 * When a thread terminates, it now sends a notification message
	 * to the parent thread which created it.  The actual use of this
	 * notification is left to be defined in a derived class.
	 * 
	 * @param th the thread that has terminated.
	 */
	virtual void Notify(Thread *th)
		{return;};

	/**
	 * In the Posix version of Common C++, this can be used to send a
	 * signal into the parent thread of the current object.
	 * 
	 * @param signo a posix signal id.
	 */
	inline void SignalParent(signo_t signo)
		{_parent->SignalThread(signo);};

	/**
	 * In the Posix version of Common C++, this can be used to send a
	 * signal into the nain application thread.
	 * 
	 * @param signo a posix signal id.
	 */
	inline void SignalMain(signo_t signo)
		{_main->SignalThread(signo);};

	/**
	 * A derivable method to call when a SIGALRM is being delivered
	 * to a specific thread.
	 */
	virtual void OnTimer(void)
		{return;};

	/**
	 * A derived method to handle hangup events being delivered
	 * to a specific thread.
	 */
	virtual void OnHangup(void)
		{return;};

	/**
	 * A derived method to call when a SIGABRT is being delivered
	 * to a specific thread.
	 */
	virtual void OnException(void)
		{return;};

	/**
	 * A derived method to call when a SIGPIPE is being delivered
	 * to a specific thread.
	 */
	virtual void OnDisconnect(void)
		{return;};

	/**
	 * A derived method to handle asynchronous I/O requests delivered
	 * to the specified thread.
	 */
	virtual void OnPolling(void)
		{return;};

	/**
	 * A derivable method to call for delivering a signal event to
	 * a specified thread.
	 *
	 * @param signo posix signal id.
	 */
	virtual void OnSignal(int signo)
		{return;};

	/**
	 * A thread-safe sleep call.  On most Posix systems, "sleep()"
	 * is implimented with SIGALRM making it unusable from multipe
	 * threads.  Pthread libraries often define an alternate "sleep"
	 * handler such as usleep(), nanosleep(), or nap(), that is thread
	 * safe, and also offers a higher timer resolution.
	 * 
	 * @param msec timeout in milliseconds.
	 */
	inline void Sleep(timeout_t msec)
		{ccxx_sleep(msec);};

	/**
	 * Used to properly exit from a Thread derived Run() or Initial()
	 * method.  Terminates execution of the current thread and calls
	 * the derived classes Final() method.
	 */
	inline void Exit(void)
		{longjmp(_env, 1);};
	       
	/**
	 * Used to specify a timeout event that can be delivered to the
	 * current thread via SIGALRM.  When the timer expires, the OnTimer() 
	 * method is called for the thread.  At present, only one thread
	 * timer can be active at any given time.  On some operating
	 * systems (including Linux) a timer can be active on each thread.
	 * 
	 * @param timer timeout in milliseconds.
	 */
	void setTimer(timeout_t timer);
	/**
	 * Gets the time remaining for the current threads timer before
	 * it expires.
	 * 
	 * @return time remaining before timer expires in milliseconds.
	 */
	timeout_t getTimer(void);
	/**
	 * Terminates the timer before the timeout period has expired.
	 * This prevents the timer from sending it's SIGALRM and makes
	 * the timer available to other threads.
	 */
	void endTimer(void);
	/**
	 * Used to wait on a Posix signal from another thread.  This can be
	 * used as a crude rondevious/synchronization method between threads.
	 * 
	 * @param signo a posix signal id.
	 */
	void WaitSignal(signo_t signo);
	/**
	 * Yeilds the current thread's CPU time slice to allow another thread to
	 * begin immediate execution.
	 */
	void Yield(void);
	/**
	 * Sets thread cancellation mode.  Threads can either be set immune to
	 * termination (THREAD_CANCEL_DISABLED), can be set to terminate when
	 * reaching specific "thread cancellation points" (THREAD_CANCEL_DEFERRED)
	 * or immediately when Terminate is requested (THREAD_CANCEL_IMMEDIATE).
	 * 
	 * @param mode for cancellation of the current thread.
	 */
	void setCancel(thread_cancel_t mode);
	/**
	 * Sets the thread's ability to be suspended from execution.  The
	 * thread may either have suspend enabled (THREAD_SUSPEND_ENABLE) or
	 * disabled (THREAD_SUSPEND_DISABLE).
	 * 
	 * @param mode for suspend.
	 */
	void setSuspend(thread_suspend_t mode);
	/**
	 * Used to enable or disable a signal within the current thread.
	 *
	 * @param signo posix signal id.
	 * @param active set to true to enable.
	 */
	void setSignal(int signo, bool mode);
	/**
	 * Used by another thread to terminate the current thread.  Termination
	 * actually occurs based on the current setCancel() mode.  When the
	 * current thread does terminate, control is returned to the requesting
	 * thread.  Terminate() should always be called at the start of any
	 * destructor of a class derived from Thread to assure the remaining
	 * part of the destructor is called without the thread still executing.
	 */
	void Terminate(void);
public:
	/**
	 * This is actually a special constructor that is used to create a
	 * thread "object" for the current execution context when that context
	 * is not created via an instance of a derived Thread object itself.
	 * This constructor does not support First.
	 * 
	 * @param bool used if the main "thread" of the application.
	 */
	Thread(bool flag);
	/**
	 * When a thread object is contructed, a new thread of execution
	 * context is created.  This constructor allows basic properties
	 * of that context (thread priority, stack space, etc) to be defined.
	 * The starting condition is also specified for whether the thread
	 * is to wait on a semaphore before begining execution or wait until
	 * it's start method is called.
	 * 
	 * @param start semaphore to wait before executing thread.
	 * @param pri thread base priority relative to it's parent.
	 * @param stack space as needed in some implementations.
	 */
	Thread(Semaphore *start = NULL, int pri = 0, size_t stack = 0);
	/**
	 * A thread of execution can also be specified by cloning an existing
	 * thread.  The existing thread's properties (cancel mode, priority,
	 * etc), are also duplicated.
	 * 
	 * @param th currently executing thread object to clone.
	 */
	Thread(const Thread &th);
	/**
	 * The thread destructor should clear up any resources that have
	 * been allocated by the thread.  The desctructor of a derived
	 * thread should begin with Terminate() and is presumed to then
	 * execute within the context of the thread causing terminaton.
	 */
	virtual ~Thread()
		{Terminate();};
	
	/**
	 * When a new thread is created, it does not begin immediate
	 * execution.  This is because the derived class virtual tables
	 * are not properly loaded at the time the C++ object is created
	 * within the constructor itself, at least in some compiler/system 
	 * combinations.  The thread can either be told to wait for an
	 * external semaphore, or it can be started directly after the
	 * constructor completes by calling the Start() method.
	 * 
	 * @return error code if execution fails.
	 * @param start optional starting semaphore to alternately use.
	 */
	int Start(Semaphore *start = NULL);

	/**
	 * Gets the pointer to the Thread class which created the current
	 * thread object.
	 * 
	 * @return a Thread *, or "(Thread *)this" if no parent.
	 */
	inline Thread *getParent(void)
		{return _parent;};
		
	/**
	 * Delivers a Posix signal to the current thread.
	 * 
	 * @param signo a posix signal id.
	 */
	inline void SignalThread(int signo)
		{pthread_kill(_tid, signo);};

	/**
	 * Suspends execution of the selected thread.  Pthreads do not
	 * normally support suspendable threads, so the behavior is
	 * simulated with signals.  On systems such as Linux that
	 * define threads as processes, SIGSTOP and SIGCONT may be used.
	 */
#ifdef _THR_SUNOS5
	inline void Suspend(void)
		{thr_suspend((thread_t)_tid);};
#else
	inline void Suspend(void)
		{pthread_kill(_tid, _SIG_THREAD_SUSPEND);};
#endif

	/**
	 * Resumes execution of the selected thread.
	 */
#ifdef	_THR_SUNOS5
	inline void Resume(void)
		{thr_continue((thread_t)_tid);};
#else
	inline void Resume(void)
		{pthread_kill(_tid, _SIG_THREAD_RESUME);};
#endif

	/**
	 * Used to retrieve the cancellation mode in effect for the
	 * selected thread.
	 * 
	 * @return cancellation mode constant.
	 */
	inline int getCancel(void)
		{return _cancel;};

	/**
	 * Verifies if the thread is still running or has already been
	 * terminated but not yet deleted.
	 * 
	 * @return true if the thread is still executing.
	 */
	inline bool isRunning(void)
		{return _tid != 0;};

	/**
	 * Tests to see if the current execution context is the same as
	 * the specified thread object.
	 * 
	 * @return true if the current context is this object.
	 */
	inline bool isThread(void)
		{return _tid == pthread_self();};

	/**
	 * Thread safe sleep call replacement.  This is mapped into sleep().
	 * 
	 * @param msec timeout in millisecond time range.
	 */
	friend void ccxx_sleep(timeout_t msec);

	/**
	 * Suspend the execution of the specified thread.
	 * 
	 * @param th specified thread.
	 */
	friend void suspend(Thread &th)
		{pthread_kill(th._tid, _SIG_THREAD_SUSPEND);};

	/**
	 * Resume execution of the specified thread.
	 * 
	 * @param th specified thread.
	 */
	friend void resume(Thread &th)
		{pthread_kill(th._tid, _SIG_THREAD_RESUME);};

	/**
	 * Signal the semaphore that the specified thread is waiting for
	 * before beginning execution.
	 * 
	 * @param th specified thread.
	 */
	friend inline void operator++(Thread &th)
		{th._start->Post();};

	/**
	 * Start execution of a specified thread.
	 */
	friend inline int start(Thread &th, Semaphore *start)
		{return th.Start(start);};

	/**
	 * Install a signal handler for use by threads and
	 * the OnSignal() event notification handler.
	 *
	 * @param signo posix signal id.
	 */

	friend void siginstall(int signo);
};

/**
 * This class allows the creation of a thread context unique "pointer"
 * that can be set and retrieved and can be used to create thread specific
 * data areas for implementing "thread safe" library routines.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short container for thread specific data storage.
 */
class ThreadKey
{
private:
	pthread_key_t key;

public:
	/**
	 * Create a unique thread specific container.
	 */
	ThreadKey();
	/**
	 * Destroy a thread specific container and any contents reserved.
	 */
	~ThreadKey();
	/**
	 * Get the value of the pointer for the thread specific data
	 * container.  A unique pointer can be set for each execution
	 * context.
	 * 
	 * @return a unique void * for each execution context.
	 */
	void *getKey(void);
	/**
	 * Set the value of the pointer for the current thread specific
	 * execution context.  This can be used to store thread context
	 * specific data.
	 * 
	 * @param ptr to thread context specific data.
	 */
	void setKey(void *);
};

/**
 * Timer ports are used to provide synchronized timing events when managed
 * under a "service thread" such as SocketService.  This is made into a
 * stand-alone base class since other derived libraries (such as the
 * serial handlers) may also use the pooled "service thread" model
 * and hence also require this code for managing timing.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short synchronized millisecond timing for service threads.
 */
class TimerPort
{
	struct timeval timer;
	bool active;

protected:
	/**
	 * Create a timer, mark it as inactive, and set the initial
	 * "start" time to the creation time of the timer object.  This
	 * allows "incTimer" to initially refer to time delays relative
	 * to the original start time of the object.
	 */
	TimerPort();

public:
	/**
	 * Set a new start time for the object based on when this call is
	 * made and optionally activate the timer for a specified number
	 * of milliseconds.  This can be used to set the starting time
	 * of a realtime session.
	 *
	 * @param timeout delay in milliseconds from "now"
	 */
	void setTimer(timeout_t timeout = 0);

	/**
	 * Set a timeout based on the current time reference value either
	 * from object creation or the last setTimer().  This reference
	 * can be used to time synchronize realtime data over specified
	 * intervals and force expiration when a new frame should be
	 * released in a synchronized manner.  
	 *
	 * @param timeout delay in milliseconds from reference.
	 */
	void incTimer(timeout_t timeout);

	/**
	 * This is used to "disable" the service thread from expiring
	 * the timer object.  It does not effect the reference time from
	 * either creation or a setTimer().
	 */
	void endTimer(void);

	/**
	 * This is used by service threads to determine how much time
	 * remains before the timer expires based on a timeout specified
	 * in setTimer() or incTimer().  It can also be called after
	 * setting a timeout with incTimer() to see if the current timeout
	 * has already expired and hence that the application is already
	 * delayed and should skip frame(s).
	 *
	 * return time remaining in milliseconds, or -1 if inactive.
	 */
	timeout_t getTimer(void);
};

inline void *getKey(ThreadKey &tk)
	{return tk.getKey();};

inline void setKey(ThreadKey &tk, void *ptr)
	{tk.setKey(ptr);};

inline void operator ++(Mutex &m)
	{m.EnterMutex();};
		
inline void operator --(Mutex &m)
	{m.LeaveMutex();};

inline void operator ++(Semaphore &s)
	{s.Post();};

inline void operator --(Semaphore &s)
	{s.Wait();};

inline void operator ++(Event &s)
	{s.Signal();};

inline void operator --(Event &s)
	{s.Wait();};

inline void signal(Thread &th, int signo)
	{th.SignalThread(signo);};

inline void signal(Event &ev)
	{ev.Signal();};

inline void signal(Semaphore &sem)
	{sem.Post();};

inline void wait(Semaphore &sem)
	{sem.Wait();};

inline void wait(Event &ev, timeout_t timer)
	{ev.Wait(timer);};

inline void reset(Event &ev)
	{ev.Reset();};

inline int get(Buffer &b, void *o)
	{return b.Wait(o);};

inline int put(Buffer &b, void *o)
	{return b.Post(o);};

inline int peek(Buffer &b, void *o)
	{return b.Peek(o);};

int operator++(MutexCounter &mc);
int operator--(MutexCounter &mc);

struct	timespec *gettimeout(struct timespec *spec, timeout_t timeout);	
void	ccxx_sleep(timeout_t msec);
void	ccxx_yield(void);
void	wait(signo_t signo);
void	pdetach(void);

#ifdef	__CCXX_NAMESPACE_H__
#undef	__CCXX_NAMESPACE_H__
#include <cc++/namespace.h>
#endif

#endif

