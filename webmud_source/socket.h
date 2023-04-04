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

#ifndef	__CCXX_SOCKET_H__
#define	__CCXX_SOCKET_H__

#ifndef	__CCXX_THREAD_H__
#include <cc++/thread.h>
#endif

//#undef _CRTIMP
//#define  _DLL

#include <winsock.h>
#include <iostream.h>

typedef	int socklen_t;

typedef enum {
	SOCKET_SUCCESS = 0,
	SOCKET_CREATE_FAILED,
	SOCKET_COPY_FAILED,
	SOCKET_INPUT_INTERRUPT,
	SOCKET_INPUT_ERROR,
	SOCKET_RESOURCE_FAILURE,
	SOCKET_OUTPUT_ERROR,
	SOCKET_OUTPUT_INTERRUPT,
	SOCKET_NOT_CONNECTED,
	SOCKET_CONNECT_REFUSED,
	SOCKET_CONNECT_REJECTED,
	SOCKET_CONNECT_TIMEOUT,
	SOCKET_CONNECT_FAILED,
	SOCKET_CONNECT_BUSY,
	SOCKET_CONNECT_NOROUTE,
	SOCKET_CONNECT_INVALID,
	SOCKET_BINDING_FAILED,
	SOCKET_BROADCAST_DENIED,
	SOCKET_ROUTING_DENIED,
	SOCKET_KEEPALIVE_DENIED,
	SOCKET_SERVICE_DENIED,
	SOCKET_SERVICE_UNAVAILABLE,
	SOCKET_EXTENDED_ERROR
}	sockerror_t;

typedef enum {
	SOCKET_PENDING_INPUT,
	SOCKET_PENDING_OUTPUT,
	SOCKET_PENDING_ERROR
}	sockpend_t;

class InetHostAddress;
class InetMaskAddress;

class __EXPORT InetAddress 
{
protected:
	struct in_addr ipaddr;
	static MutexCounter counter;

public:
	InetAddress();
	InetAddress(struct in_addr addr);
	InetAddress(const char *address);
	
	char	*getHostname(void);
	bool	isInetAddress(void);

	InetAddress &operator=(char *str);
	InetAddress &operator=(struct in_addr);
	bool operator==(InetAddress &a);
	bool operator!=(InetAddress &a);

	inline bool operator!()
		{return !isInetAddress();};

	inline struct in_addr getAddress(void)
		{return ipaddr;};
};	

class __EXPORT InetMaskAddress : public InetAddress 
{
public:
	InetMaskAddress(const char *mask);

	friend InetHostAddress operator&(InetHostAddress &addr, InetMaskAddress &mask);
};

class __EXPORT InetHostAddress : public InetAddress 
{
public:
	InetHostAddress(const char *host = NULL);
	InetHostAddress(struct in_addr addr);

	InetHostAddress &operator&=(InetMaskAddress &mask);

	friend class InetMaskAddress;
	friend InetHostAddress operator&(InetHostAddress &addr, InetMaskAddress &mask);
};

class __EXPORT BroadcastAddress : public InetAddress 
{
public:
	BroadcastAddress(const char *net = "255.255.255.255");
};

class __EXPORT Socket 
{
private:
	// used by exception handlers....

	struct	{
		bool thrown: 1;
		bool broadcast: 1;
		bool route: 1;
		bool keepalive: 1;
	} flags;

	sockerror_t errid;
	char *errstr;

	void setSocket(void);

protected:
	SOCKET so;

	enum {
		SOCKET_INITIAL,
		SOCKET_AVAILABLE,
		SOCKET_BOUND,
		SOCKET_CONNECTED
	} state;

	void endSocket(void);
	sockerror_t connectError(void);
	sockerror_t setBroadcast(bool enable);

	// used to "throw"
	sockerror_t Error(sockerror_t err, char *errstr = NULL);

	inline void Error(char *errstr)
		{Error(SOCKET_EXTENDED_ERROR, errstr);};

	inline void setError(bool enable)
		{flags.thrown = !enable;};

	Socket(int domain, int type, int protocol);
	Socket(SOCKET fd);
	Socket(const Socket &source);
public:
	virtual ~Socket()
		{endSocket();};

	Socket &operator=(const Socket &s);

	InetHostAddress getSender(short *port = NULL);
	InetHostAddress getPeer(short *port = NULL);
	InetHostAddress getLocal(short *port = NULL);

	sockerror_t setRouting(bool enable);
	sockerror_t setKeepAlive(bool enable);
	bool isPending(sockpend_t pend, unsigned long timeout = ~0);

	bool isActive(void)
		{return (state != SOCKET_INITIAL);};

	bool isBroadcast(void)
		{return flags.broadcast;};

	bool isConnected(void)
		{return (state == SOCKET_CONNECTED);};

	bool isRouted(void)
		{return flags.route;};

	sockerror_t getErrorNumber(void)
		{return errid;};

	char *getErrorString(void)
		{return errstr;};
};

class __EXPORT UDPSocket : public Socket 
{
private:
	inline sockerror_t setKeepAlive(bool enable)
		{return Socket::setKeepAlive(enable);};

protected:
	struct sockaddr_in peer;

public:
	UDPSocket(void);
	UDPSocket(InetAddress &bind, short port);
	void setPeer(InetHostAddress &host, short port);
	InetHostAddress getPeer(short *port = NULL);

	inline int Send(void *buf, size_t len)
		{return ::sendto(so, (const char *)buf, len, 0, (struct sockaddr *)&peer, (int)sizeof(peer));};

	inline int Recv(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, 0);};

	inline int Peek(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, MSG_PEEK);};

	~UDPSocket()
		{endSocket();};
};

class __EXPORT UDPBroadcast : public UDPSocket 
{
private:
	sockerror_t setBroadcast(bool enable)
		{return Socket::setBroadcast(enable);};

	void setPeer(InetHostAddress &ia, short port) {};

public:
	UDPBroadcast(InetAddress &ia, short port);
	void setPeer(BroadcastAddress &subnet, short port);
};

class __EXPORT UDPTransmit : private UDPSocket 
{
protected:
	inline sockerror_t setBroadcast(bool enable)
		{return Socket::setBroadcast(enable);};

	inline sockerror_t setRouting(bool enable)
		{return Socket::setRouting(enable);}; 

	UDPTransmit(InetAddress &bind, short port, short peer = 0);

public:
	sockerror_t Connect(BroadcastAddress &subnet);
	sockerror_t Connect(InetHostAddress &host);
	sockerror_t Disconnect(void);

	inline int Send(void *buf, size_t len)
		{return ::send(so, (const char *)buf, len, 0);};

	inline bool isOutputReady(unsigned long timeout = 0l)
		{return Socket::isPending(SOCKET_PENDING_OUTPUT, timeout);};
};

class __EXPORT UDPReceive : private UDPSocket 
{
protected:
	UDPReceive(InetAddress &bind, short port, short peer = 0);

	inline sockerror_t setRouting(bool enable)
		{return Socket::setRouting(enable);};

public:
	sockerror_t Connect(InetHostAddress &host);
	sockerror_t Disconnect(void);

	inline int Recv(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, 0);};

	inline bool isInputReady(unsigned long timeout = 0l)
		{return Socket::isPending(SOCKET_PENDING_INPUT, timeout);};
};

class __EXPORT UDPDuplex : public UDPTransmit, public UDPReceive 
{
protected:
	sockerror_t setBroadcast(bool enable)
		{return UDPTransmit::setBroadcast(enable);};
public:
	UDPDuplex(InetAddress &bind, short send, short recv);

	sockerror_t Connect(InetHostAddress &host);
	sockerror_t Disconnect(void);
	sockerror_t setRouting(bool enable);
};

class __EXPORT TCPSocket : private Socket 
{
protected:
	virtual bool OnAccept(InetHostAddress &ia, short port)
		{return true;};

	friend class TCPStream;
	friend class tcpstream;

public:
	TCPSocket(InetAddress &bind, short port, int backlog = 5);
	void Reject(void);

	inline InetHostAddress getRequest(short *port = NULL)
		{return Socket::getSender(port);};

	inline InetHostAddress getLocal(short *port = NULL)
		{return Socket::getLocal(port);};

	inline bool isPending(unsigned long timeout)
		{return Socket::isPending(SOCKET_PENDING_INPUT, timeout);};

	~TCPSocket()
		{endSocket();};
	
};

class __EXPORT TCPStream : public Socket, public streambuf, public iostream 
{
private:
	int bufsize;
	char *gbuf, *pbuf;

	sockerror_t setBroadcast(bool enable)
		{return Socket::setBroadcast(enable);};

	inline InetHostAddress getSender(short *port) 
		{return InetHostAddress();};

	int doallocate();	

	friend TCPStream& crlf(TCPStream&);
	friend TCPStream& lfcr(TCPStream&);

protected:
	TCPStream();
	void Allocate(int size);
	void endStream(void);
	int underflow(void);
	int overflow(int ch);
	int sync(void);

	iostream *tcp(void)
		{return ((iostream *)this);};
public:
	TCPStream(TCPSocket &server, int size = 512);
	TCPStream(InetHostAddress &host, short port, int size = 512);
	~TCPStream()
		{endStream();};

	int getBufferSize(void)
		{return bufsize;};
};

class __EXPORT tcpstream : public TCPStream
{
public:
	tcpstream();
	tcpstream(char *addr, int buffer = 512);
	tcpstream(TCPSocket &tcp, int buffer = 512);
	void open(char *addr, int buffer = 512);
	void open(TCPSocket &tcp, int buffer = 512);
	void close(void);
	inline bool operator!()
		{return (Socket::state != SOCKET_INITIAL);};
};

class __EXPORT TCPSession : public TCPStream, public Thread 
{
protected:
	void Final(void)
		{delete this;};
public:
	TCPSession(Semaphore *start, TCPSocket &server, int size = 512,  int pri = 0, int stack = 0);
};

extern __EXPORT ostream &operator<<(ostream &os, InetAddress &ia);

inline struct in_addr getaddress(InetAddress &ia)
	{return ia.getAddress();};

class init_WSA 
{
public:
	init_WSA();
private:
	WSADATA				wsaData;
};
#endif

#else //__WIN32__
#include "socket_unix.h"
#endif //__WIN32__
