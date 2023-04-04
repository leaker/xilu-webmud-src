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
// The exception is that, if you link the Common C++ library with other files
// to produce an executable, this does not by itself cause the
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

#ifndef	__CCXX_SOCKET_H__
#define	__CCXX_SOCKET_H__

#ifndef	__CCXX_THREAD_H__
#include <cc++/thread.h>
#else
#ifdef	__CCXX_NAMESPACE_H__
#include <cc++/macros.h>
#endif
#endif

#include <iostream.h>

typedef enum
{
	SOCKET_COMPLETION_IMMEDIATE,
	SOCKET_COMPLETION_DELAYED
} sockcomplete_t;

typedef enum
{
	SOCKET_INITIAL,
       	SOCKET_AVAILABLE,
       	SOCKET_BOUND,
       	SOCKET_CONNECTED,
       	SOCKET_CONNECTING
} sockstate_t;

typedef enum
{
	SOCKET_SUCCESS=0,
	SOCKET_CREATE_FAILED,
	SOCKET_COPY_FAILED,
	SOCKET_INPUT_ERROR,
	SOCKET_INPUT_INTERRUPT,
	SOCKET_RESOURCE_FAILURE,
	SOCKET_OUTPUT_ERROR,
	SOCKET_OUTPUT_INTERRUPT,
	SOCKET_NOT_CONNECTED,
	SOCKET_CONNECT_REFUSED,
	SOCKET_CONNECT_REJECTED,
	SOCKET_CONNECT_TIMEOUT,
	SOCKET_CONNECT_FAILED,
	SOCKET_CONNECT_INVALID,
	SOCKET_CONNECT_BUSY,
	SOCKET_CONNECT_NOROUTE,
	SOCKET_BINDING_FAILED,
	SOCKET_BROADCAST_DENIED,
	SOCKET_ROUTING_DENIED,
	SOCKET_KEEPALIVE_DENIED,
	SOCKET_SERVICE_DENIED,
	SOCKET_SERVICE_UNAVAILABLE,
	SOCKET_EXTENDED_ERROR
} sockerror_t;

typedef	enum
{
	SOCKET_IPTOS_LOWDELAY,
	SOCKET_IPTOS_THROUGHPUT,
	SOCKET_IPTOS_RELIABILITY,
	SOCKET_IPTOS_MINCOST,
	SOCKET_IPTOS_INVALID
} socktos_t;

typedef enum
{
	SOCKET_PENDING_INPUT,
	SOCKET_PENDING_OUTPUT,
	SOCKET_PENDING_ERROR
} sockpend_t;

class InetHostAddress;
class InetMaskAddress;

/**
 * binary encoded internet host addresses are held in this special class
 * object.  This class represents the 'generic' internet address data type.
 * Other InetAddress derived objects hold addresses which are used only
 * for specific purposes, such as for network masks, broadcast addresses,
 * etc.
 * 
 * @author David Sugar <dyfet@oste.com>
 * @short Internet Address binary data type.
 */
class InetAddress 
{
protected:
	struct in_addr ipaddr;
	static MutexCounter counter;

public:
	/**
	 * Create an Internet Address object with an empty (0.0.0.0)
	 * address.
	 */
	InetAddress();

	/**
	 * Convert the system internet address data type (struct in_addr)
	 * into a Common C++ InetAddress object.
	 * 
	 * @param addr struct of system used binary internet address.
	 */

	InetAddress(struct in_addr);

	/**
	 * Convert a null terminated ASCII host address string (example: 
	 * "127.0.0.1") directly into a Common C++ InetAddress object.
	 * 
	 * @param address null terminated C string.
	 */
	InetAddress(const char *address);

	/**
	 * Provide a string representation of the value (Internet Address)
	 * held in the InetAddress object.
	 * 
	 * @return string representation of InetAddress.
	 */
	char	*getHostname(void);

	/**
	 * May be used to verify if a given InetAddress returned
	 * by another function contains a "valid" address, or "0.0.0.0"
	 * which is often used to mark "invalid" InetAddress values.
	 *
	 * @return true if address != 0.0.0.0.
	 */
	bool isInetAddress(void);

	/**
	 * Provide a low level system usable struct in_addr object from
	 * the contents of InetAddress.  This is needed for services such
	 * as bind() and connect().
	 * 
	 * @return system binary coded internet address.
	 */
	inline struct in_addr getAddress(void)
		{return ipaddr;};

	InetAddress &operator=(char *str);
	InetAddress &operator=(struct in_addr addr);

	inline bool operator!()
		{return !isInetAddress();};

	/**
	 * Compare two internet addresses to see if they are equal
	 * (if they specify the physical address of the same internet host).
	 */
	bool operator==(InetAddress &a);

	/**
	 * Compare two internet addresses to see if they are not
	 * equal (if they each refer to unique and different physical
	 * ip addresses).
	 */
	bool operator!=(InetAddress &a);
};	

/**
 * Internet addresses used specifically as masking addresses (such as "
 * 255.255.255.0") are held in the InetMaskAddress derived object.  The
 * seperate class is used so that C++ type casting can automatically
 * determine when an InetAddress object is really a mask address object
 * rather than simply using the base class.  This also allows manipulative
 * operators for address masking to operate only when presented with a
 * Masked address as well as providing cleaner and safer source.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short Internet Address Mask such as subnet masks.
 */
class InetMaskAddress : public InetAddress
{
public:
	/**
	 * Create the mask from a null terminated ASCII string such as
	 * "255.255.255.128".
	 * 
	 * @param mask null terminated ASCII mask string.
	 */
	InetMaskAddress(const char *mask);

	/**
	 * Masks are usually used to coerce host addresses into a specific
	 * router or class domain.  This can be done by taking the Inet
	 * Host Address object and "and"ing it with an address mask.  This
	 * operation can be directly expressed in C++ through the & operator.
	 * 
	 * @return a internet host address that has been masked.
	 * @param addr host address to be masked by subnet.
	 * @param mask inetnet mask address object to mask by.
	 */
	friend InetHostAddress operator&(InetHostAddress &addr, InetMaskAddress &mask);
};

/**
 * This object is used to hold the actual and valid internet address of a 
 * specific host machine that will be accessed through a socket.
 * 
 * @author David Sugar <dyfet@ostel.com>.
 * @short Address of a specific Internet host machine.
 */
class InetHostAddress : public InetAddress
{
private:
	/**
	 * Used by constructors.
	 *
	 * @param host dns or physical address.
	 */
	void setAddress(const char *host);

public:
	/**
	 * Create a new host address for the "local" host.  This is set
	 * to the IP address that represents the interface matching
	 * "gethostname()".
	 */
	InetHostAddress();
 
	/**
	 * Create a new host address for a specific internet host.  The
	 * internet host can be specified in a null terminated ASCII
	 * string and include either the physical host address or the
	 * DNS name of a host machine.  Hence, an InetHostAddress
	 * ("www.voxilla.org") can be directly declaired in this manner.
	 * 
	 * @param host dns or physical address of an Internet host.
	 */
	InetHostAddress(const char *host);

	/**
	 * Convert a system socket binary address such as may be
	 * returned through the accept() call or getsockpeer() into
	 * an internet host address object.
	 * 
	 * @param addr binary address of internet host.
	 */
	InetHostAddress(struct in_addr addr);


	/**
	 * Mask the internet host address object with a network mask address.
	 * This is commonly used to coerce an address by subnet.
	 */
	InetHostAddress &operator&=(InetMaskAddress &mask);

	friend class InetMaskAddress;
	friend InetHostAddress operator&(InetHostAddress &addr, InetMaskAddress &mask);
};

/**
 * The broadcast address object is used to store the broadcast address for
 * a specific subnet.  This is commonly used for UDP broadcast operations.
 */
class BroadcastAddress : public InetAddress
{
public:
	/**
	 * Specify the physical broadcast address to use and create a new
	 * broadcast address object based on a null terminated ASCII
	 * string.
	 * 
	 * @param net null terminated ASCII network address.
	 */
	BroadcastAddress(const char *net = "255.255.255.255");
};

/**
 * The Socket is used as the base for all Internet protocol services
 * under Common C++.  A socket is a system resource (or winsock descriptor)
 * that occupies a specific port address (and may be bound to a specific
 * network interface) on the local machine.  The socket may also be
 * directly connected to a specific socket on a remote internet host.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short base class of all sockets.
 */
class Socket
{
private:
	// used to throw
	sockerror_t errid;
	char *errstr;

	struct
	{
		bool thrown: 1;
		bool broadcast: 1;
		bool route: 1;
		bool keepalive: 1;
	} flags;

	void setSocket(void);

protected:
	sockstate_t state;
	int so;

	/**
	 * This service is used to throw all socket errors which usually
	 * occur during the socket constructor.
	 *
	 * @param error defined socket error id.
	 * @param errstr string or message to pass.
	 */
	sockerror_t Error(sockerror_t error, char *errstr = NULL);

	/**
	 * This service is used to throw application defined socket errors
	 * where the application specific error code is a string.
	 *
	 * @param errstr string or message to pass.
	 */
	inline void Error(char *estr)
		{Error(SOCKET_EXTENDED_ERROR, estr);};
	
	/**
	 * This service is used to turn the error handler on or off for
	 * "throwing" exceptions by manipulating the thrown flag.
	 *
	 * @param true to enable handler.
	 */
	inline void setError(bool enable)
		{flags.thrown = !enable;};

	/**
	 * Used as the default destructor for ending a socket.  This
	 * will cleanly terminate the socket connection.  It is provided
	 * for use in derived virtual destructors.
	 */
	void endSocket(void);

	/**
	 * Used as a common handler for connection failure processing.
	 *
	 * @return correct failure code to apply.
	 */
	sockerror_t connectError(void);

	/**
	 * Set the subnet broadcast flag for the socket.  This enables
	 * sending to a subnet and may require special image privileges
	 * depending on the operating system.
	 *
	 * @return 0 (SOCKET_SUCCESS) on success, else error code.
	 * @param enable when set to true.
	 */
	sockerror_t setBroadcast(bool enable);

	/**
	 * Set the socket routing to indicate if outgoing messages
	 * should bypass normal routing (set false).
	 *
	 * @return 0 on success.
	 * @param enable normal routing when set to true.
	 */
	sockerror_t setRouting(bool enable);

	/**
	 * An unconnected socket may be created directly on the local
	 * machine.  Sockets can occupy both the internet domain (AF_INET)
	 * and UNIX socket domain (AF_UNIX) under unix.  The socket type
	 * (SOCK_STREAM, SOCK_DGRAM) and protocol may also be specified.
	 * If the socket cannot be created, an exception is thrown.
	 * 
	 * @param domain socket domain to use.
	 * @param type base type and protocol family of the socket.
	 * @param protocol specific protocol to apply.
	 */
	Socket(int domain, int type, int protocol = 0);

	/**
	 * A socket object may be created from a file descriptor when that
	 * descriptor was created either through a socket() or accept()
	 * call.  This constructor is mostly for internal use.
	 * 
	 * @param fd file descriptor of an already existing socket.
	 */
	Socket(int fd);

	/**
	 * A socket can also be constructed from an already existing
	 * Socket object.  The socket file descriptor is dup()'d.  This
	 * does not exist under win32.
	 * 
	 * @param source of existing socket to clone.
	 */
	Socket(const Socket &source);

public:
	/**
	 * The socket base class may be "thrown" as a result of an
	 * error, and the "catcher" may then choose to destroy the
	 * object.  By assuring the socket base class is a virtual
	 * destructor, we can assure the full object is properly
	 * terminated.
	 */
	virtual ~Socket()
		{endSocket();};

	/**
	 * Sockets may also be duplicated by the assignment operator.
	 */
	Socket &operator=(const Socket &from);

	/**
	 * May be used to examine the origin of data waiting in the
	 * socket receive queue.  This can tell a TCP server where pending
	 * "connect" requests are coming from, or a UDP socket where it's
	 * next packet arrived from.
	 *
	 * @param ptr to port number of sender.
	 * @return host address, test with "isInetAddress()".
	 */
	InetHostAddress getSender(short *port = NULL);

	/**
	 * Get the host address and port of the socket this socket
	 * is connected to.  If the socket is currently not in a
	 * connected state, then a host address of 0.0.0.0 is
	 * returned.
	 *
	 * @param ptr to port number of remote socket.
	 * @return host address of remote socket.
	 */
	InetHostAddress getPeer(short *port = NULL);

	/**
	 * Get the local address and port number this socket is
	 * currently bound to.
	 *
	 * @param ptr to port number on local host.
	 * @return host address of interface this socket is bound to.
	 */
	InetHostAddress getLocal(short *port = NULL);
	
	/**
	 * Used to specify blocking mode for the socket.  A socket
	 * can be made non-blocking by setting SOCKET_COMPLETION_DELAYED
	 * or set to block on all access with SOCKET_COMPLETION_IMMEDIATE.
	 * I do not believe this form of non-blocking socket I/O is supported
	 * in winsock, though it provides an alternate asynchronous set of
	 * socket services.
	 * 
	 * @param mode specify socket I/O call blocking mode.
	 */
	void setCompletion(sockcomplete_t completion);

	/**
	 * Set the keep-alive status of this socket and if keep-alive
	 * messages will be sent.
	 *
	 * @return 0 on success.
	 * @param enable keep alive messages.
	 */
	sockerror_t setKeepAlive(bool enable);

	/**
	 * Set packet scheduling on platforms which support ip quality
	 * of service conventions.  This effects how packets in the
	 * queue are scheduled through the interface.
	 *
	 * @return 0 on success, error code on failure.
	 * @param type of service enumerated type.
	 */
	sockerror_t setTypeOfService(socktos_t service);

	/**
	 * Can test to see if this socket is "connected", and hence
	 * whether a "catch" can safely call getPeer().  Of course,
	 * an unconnected socket will return a 0.0.0.0 address from
	 * getPeer() as well.
	 *
	 * @return true when socket is connected to a peer.
	 */
	inline bool isConnected(void)
		{return (state == SOCKET_CONNECTED);};

	/**
	 * Test to see if the socket is at least operating or if it
	 * is mearly initialized.  "initialized" sockets may be the
	 * result of failed constructors.
	 *
	 * @return true if not in initial state.
	 */
	inline bool isActive(void)
		{return (state != SOCKET_INITIAL);};

	/**
	 * Operator based testing to see if a socket is currently
	 * active.
	 */
	inline bool operator!()
		{return (state == SOCKET_INITIAL);};

	/**
	 * Return if broadcast has been enabled for the specified
	 * socket.
	 *
	 * @return true if broadcast socket.
	 */
	inline bool isBroadcast(void)
		{return flags.broadcast;};

	/**
	 * Return if socket routing is enabled.
	 *
	 * @return true if routing enabled.
	 */
	inline bool isRouted(void)
		{return flags.route;};

	/**
 	 * Often used by a "catch" to fetch the last error of a thrown
	 * socket.
	 * 
	 * @return error number of sockerror_t error.
 	 */
	inline sockerror_t getErrorNumber(void)
		{return errid;};
	
	/**
 	 * Often used by a "catch" to fetch the user set error string
	 * of a thrown socket.
	 * 
	 * @return string for error message.
 	 */
	inline char *getErrorString(void)
	       	{return errstr;};

	/**
	 * Get the status of pending operations.  This can be used to
	 * examine if input or output is waiting, or if an error has
	 * occured on the descriptor.
	 *
	 * @return true if ready, false on timeout.
	 * @param ready check to perform.
	 * @param timeout in milliseconds, inf. if not specified.
	 */
	bool isPending(sockpend_t pend, timeout_t timeout = ~0);
};

/**
 * UDP sockets implement the TCP SOCK_DGRAM UDP protocol.  They can be
 * used to pass unverified messages between hosts, or to broadcast a
 * specific message to an entire subnet.  Please note that Streaming of
 * realtime data commonly use UDPDuplex related classes rather than
 * UDPSocket.
 * 
 * @author David Sugar <dyfet@ostel.com>.
 * @short Unreliable Datagram Protocol sockets.
 */
class UDPSocket : public Socket
{
private:
	inline sockerror_t setKeepAlive(bool enable)
		{return Socket::setKeepAlive(enable);};

protected:
	struct sockaddr_in peer;

public:
	/**
	 * Create an unbound UDP socket, mostly for internal use.
	 */
	UDPSocket(void);

	/**
	 * Create a UDP socket and bind it to a specific interface
	 * and port address so that other UDP sockets on remote
	 * machines (or the same host) may find and send UDP messages
	 * to it.  On failure to bind, an exception is thrown.
	 * 
	 * @param bind address to bind this socket to.
	 * @param port number to bind this socket to.
	 */
	UDPSocket(InetAddress &bind, short port);

	/**
	 * Destroy a UDP socket as a socket.
	 */
	~UDPSocket()
		{endSocket();};

	/**
	 * set the peer address to send message packets to.  This can be
	 * set before every Send() call if nessisary.
	 *
	 * @param host address to send packets to.
	 * @param port number to deliver packets to.
	 */
	void setPeer(InetHostAddress &host, short port);

	/**
	 * Send a message packet to a peer host.
	 *
	 * @param pointer to packet buffer to send.
	 * @param len of packet buffer to send.
	 * @return number of bytes sent.
	 */
	inline int Send(void *buf, size_t len)
		{return ::sendto(so, (char *)buf, len, 0, (struct sockaddr *)&peer, (socklen_t)sizeof(peer));};

	/**
	 * Receive a message from any host.
	 *
	 * @param pointer to packet buffer to receive.
	 * @param len of packet buffer to receive.
	 * @return number of bytes received.
	 */
	inline int Recv(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, 0);};

	/**
	 * Examine address of sender of next waiting packet.  This also
	 * sets "peer" address to the sender so that the next "send"
	 * message acts as a "reply".  This additional behavior overides
	 * the standard socket getSender behavior.
	 *
	 * @param pointer to hold port number.
	 */
	InetHostAddress getPeer(short *port = NULL);

	/**
	 * Examine contents of next waiting packet.
	 *
	 * @param pointer to packet buffer for contents.
	 * @param len of packet buffer.
	 * @return number of bytes examined.
	 */
	inline int Peek(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, MSG_PEEK);};
};


/**
 * Representing a UDP socket used for subnet broadcasts, this class
 * provides an alternate binding and setPeer() capability for UDP
 * sockets.
 *
 * @author David Sugar <dyfet@ostel.com>.
 * @short Unreliable Datagram for subnet broadcasts.
 */
class UDPBroadcast : public UDPSocket
{
private:
	void setPeer(InetHostAddress &ia, short port) {};

	sockerror_t setBroadcast(bool enable)
		{return Socket::setBroadcast(enable);};

public:
	/**
	 * Create and bind a subnet broadcast socket.
	 *
	 * @param address to bind socket under locally.
	 * @param port to bind socket under locally.
	 */
	UDPBroadcast(InetAddress &ia, short port);

	/**
	 * Set peer by subnet rather than specific host.
	 *
	 * @param subnet of peer hosts to send to.
	 * @param port number to use.
	 */
	void setPeer(BroadcastAddress &ia, short port);
};	

/**
 * Representing half of a two-way UDP connection, the UDP transmitter
 * can broadcast data to another selected peer host or to an entire
 * subnet.
 * 
 * @author David Sugar <dyfet@ostel.com>.
 * @short Unreliable Datagram Peer Associations.
 */
class UDPTransmit : private UDPSocket
{
protected:
	inline sockerror_t setBroadcast(bool enable)
		{return Socket::setBroadcast(enable);};

	inline sockerror_t setRouting(bool enable)
		{return Socket::setRouting(enable);};

	inline sockerror_t setTypeOfService(socktos_t tos)
		{return Socket::setTypeOfService(tos);};

	/**
	 * Create a UDP transmitter, bind it to a specific interface
	 * and port address so that other UDP sockets on remote
	 * machines (or the same host) may find and send UDP messages
	 * to it, and associate it with a given port on a peer host.  
         * On failure to bind, an exception is thrown.  This class is
	 * only used to build the UDP Duplex.
	 * 
	 * @param bind address to bind this socket to.
	 * @param port number to bind this socket to.
	 * @param port number on peer host to associate with.
	 */
	UDPTransmit(InetAddress &bind, short port, short peer = 0);

public:
	/**
	 * Associate this socket with a specified peer host.  The port
	 * number from the constructor will be used.  All UDP packets
	 * will be sent to and received from the specified host.
	 *
	 * @return 0 on success, -1 on error.
	 * @param host address to connect socket to.
	 */
	sockerror_t Connect(InetHostAddress &host);

	/**
	 * Associate this socket with a subnet of peer hosts for
	 * subnet broadcasting.  The server must be able to assert
	 * broadcast permission for the socket.
	 *
	 * @return 0 on success, -1 on error.
	 * @param subnet address to broadcast into.
	 */
	sockerror_t Connect(BroadcastAddress &subnet);

	/**
	 * Disassociate this socket from any host connection.  No data
	 * should be read or written until a connection is established.
	 */
	sockerror_t Disconnect(void);

	/**
	 * Transmit "send" to use "connected" send rather than sendto.
	 *
	 * @return number of bytes sent.
	 * @param address of buffer to send.
	 * @param len of bytes to send.
	 */
	inline int Send(void *buf, int len)
		{return ::send(so, (char *)buf, len, 0);};

	/**
	 * See if output queue is empty for sending more packets.
	 *
	 * @return true if output available.
	 * @param timeout in milliseconds to wait.
	 */
	inline bool isOutputReady(timeout_t timeout = ~0)
		{return Socket::isPending(SOCKET_PENDING_OUTPUT, timeout);};
};

/**
 * Representing half of a two-way UDP connection, the UDP receiver
 * can receive data from another peer host or subnet.  This class is
 * used exclusivily to derive the UDPDuplex.
 * 
 * @author David Sugar <dyfet@ostel.com>.
 * @short Unreliable Datagram Peer Associations.
 */
class UDPReceive : private UDPSocket
{
protected:
	/**
	 * Create a UDP receiver, bind it to a specific interface
	 * and port address so that other UDP sockets on remote
	 * machines (or the same host) may find and send UDP messages
	 * to it, and associate it with a given port on a peer host.  
         * On failure to bind, an exception is thrown.
	 * 
	 * @param bind address to bind this socket to.
	 * @param port number to bind this socket to.
	 * @param port number on peer host to associate with.
	 */
	UDPReceive(InetAddress &bind, short port, short peer = 0);

	/**
	 * Associate this socket with a specified peer host.  The port
	 * number from the constructor will be used.  All UDP packets
	 * will be sent received from the specified host.
	 *
	 * @return 0 on success, -1 on error.
	 * @param host address to connect socket to.
	 */
	sockerror_t Connect(InetHostAddress &host);

	/**
	 * Disassociate this socket from any host connection.  No data
	 * should be read or written until a connection is established.
	 */
	sockerror_t Disconnect(void);

	/**
	 * Set routing.
	 */
	inline sockerror_t setRouting(bool enable)
		{return Socket::setRouting(enable);};

	/**
	 * Set type of service.
	 */
	inline sockerror_t setTypeOfService(socktos_t tos)
		{return Socket::setTypeOfService(tos);};

public:
	/**
	 * Receive a data packet from the connected peer host.
	 *
	 * @return num of bytes actually received.
	 * @param addr of data receive buffer.
	 * @param size of data receive buffer.
	 */
	inline int Recv(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, 0);};

	/**
	 * See if input queue has data packets available.
	 *
	 * @return true if data packets available.
	 * @param timeout in milliseconds.
	 */
	inline bool isInputReady(timeout_t timeout = ~0)
		{return Socket::isPending(SOCKET_PENDING_INPUT, timeout);};
};

/**
 * UDP duplex connections impliment a bi-directional point-to-point UDP
 * session between two peer hosts.  Two UDP sockets are typically used
 * on alternating port addresses to assure that sender and receiver
 * data does not collide or echo back.  A UDP Duplex is commonly used
 * for full duplex real-time streaming of UDP data between hosts.
 * 
 * @author David Sugar <dyfet@ostel.com>.
 * @short Unreliable Datagram Peer Associations.
 */
class UDPDuplex : public UDPTransmit, public UDPReceive
{
protected:
	/**
	 * Set broadcast state on sending socket only.
	 *
	 * @param true to enable.
	 */
	sockerror_t setBroadcast(bool enable)
		{return UDPTransmit::setBroadcast(enable);};
public:
	/**
	 * Create a UDP duplex as a pair of UDP simplex objects
         * bound to alternating and interconnected port addresses.
	 * 
	 * @param bind address to bind this socket to.
	 * @param port number to bind sender.
	 * @param port number to bind reciever.
	 */
	UDPDuplex(InetAddress &bind, short from, short to);

	/**
	 * Associate the duplex with a specified peer host. Both
	 * the sender and receiver will be interconnected with
	 * the remote host.
	 *
	 * @return 0 on success, error code on error.
	 * @param host address to connect socket to.
	 */
	sockerror_t Connect(InetHostAddress &host);

	/**
	 * Disassociate this duplex from any host connection.  No data
	 * should be read or written until a connection is established.
	 *
	 * @return 0 on success, error code on error.
	 */
	sockerror_t Disconnect(void);

	/**
	 * Set routing for both pairs of the duplex.
	 *
	 * @param true to enable routing.
	 */
	sockerror_t setRouting(bool enable);

	/**
	 * Set type of service for both pairs of sockets.
	 *
	 * @param type of service for pair of sockets.
	 */
	sockerror_t setTypeOfService(socktos_t tos);
};


/**
 * TCP sockets are used for stream based connected sessions between two
 * sockets.  Both error recovery and flow control operate transparently
 * for a TCP socket connection.  The TCP socket base class is primary used
 * to bind a TCP "server" for accepting TCP streams.
 * 
 * @author David Sugar <dyfet@tycho.com>
 * @short bound server for TCP streams and sessions.
 */
class TCPSocket : private Socket
{
protected:
	/**
	 * A method to call in a derived TCPSocket class that is acting
	 * as a server when a connection request is being accepted.  The
	 * server can implement protocol specific rules to exclude the
	 * remote socket from being accepted by returning false.  The
	 * Peek method can also be used for this purpose.
	 * 
	 * @return true if client should be accepted.
	 * @param ia internet host address of the client.
	 * @param port number of the client.
	 */
	virtual bool OnAccept(InetHostAddress &ia, short port)
		{return true;};

	friend class TCPStream;
	friend class SocketPort;
	friend class tcpstream;

public:
	/**
	 * A TCP "server" is created as a TCP socket that is bound
	 * to a hardware address and port number on the local machine
	 * and that has a backlog queue to listen for remote connection
	 * requests.  If the server cannot be created, an exception is
	 * thrown.
	 * 
	 * @param bind local ip address or interface to use.
	 * @param port number to bind socket under.
	 * @param backlog size of connection request queue.
	 */
	TCPSocket(InetAddress &bind, short port, int backlog = 5);
	
	/**
	 * Return address and port of next connection request.  This
	 * can be used instead of OnAccept() to pre-evaluate connection
	 * requests.
	 *
	 * @return host requesting a connection.
	 * @param port number of requestor.
	 */
	inline InetHostAddress getRequest(short *port = NULL)
		{return Socket::getSender(port);};

	/**
	 * Used to reject the next incoming connection request.
	 */
	void Reject(void);

	/**
	 * Used to get local bound address.
	 */
	inline InetHostAddress getLocal(short *port = NULL)
		{return Socket::getLocal(port);};

	/**
	 * Used to wait for pending connection requests.
	 */
	inline bool isPending(timeout_t timeout = ~0)
		{return Socket::isPending(SOCKET_PENDING_INPUT, timeout);};

	/**
	 * Use base socket handler for ending this socket.
	 */
	~TCPSocket()
		{endSocket();};
};

/**
 * TCP streams are used to represent TCP client connections to a server
 * by TCP protocol servers for accepting client connections.  The TCP
 * stream is a C++ "stream" class, and can accept streaming of data to
 * and from other C++ objects using the << and >> operators.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short streamable TCP socket connection.
 */

class TCPStream : public Socket, public streambuf, public iostream
{
private:
	int bufsize;
	char *gbuf, *pbuf;
	
	inline sockerror_t setBroadcast(bool enable)
		{return Socket::setBroadcast(enable);};

	inline InetHostAddress getSender(short *port)
		{return InetHostAddress();};

	int doallocate();

	friend TCPStream& crlf(TCPStream&);
	friend TCPStream& lfcr(TCPStream&);

protected:
	/**
	 * The constructor required for "tcpstream", a more C++ style
	 * version of the TCPStream class.
	 */
	TCPStream();

	/**
	 * Used to allocate the buffer space needed for iostream
	 * operations.  This function is called by the constructor.
	 *
	 * @param size of stream buffers from constructor.
	 */
	void Allocate(int size);

	/**
	 * Used to terminate the buffer space and cleanup the socket
	 * connection.  This fucntion is called by the destructor.
	 */
	void endStream(void);

	/**
	 * This streambuf method is used to load the input buffer
	 * through the established tcp socket connection.
	 *
	 * @return char from get buffer, EOF if not connected.
	 */
	int underflow(void);

	/**
	 * This streambuf method is used to write the output
	 * buffer through the established tcp connection.
	 *
	 * @param char to push through.
	 * @return char pushed through.
	 */
	int overflow(int ch);

	/**
	 * Used in derived classes to refer to the current object via
	 * it's iostream.  For example, to send a set of characters
	 * in a derived method, one might use *tcp() << "test".
	 *
	 * @return stream pointer of this object.
	 */
	iostream *tcp(void)
		{return ((iostream *)this);};

public:
	/**
	 * Create a TCP stream by accepting a connection from a bound
	 * TCP socket acting as a server.  This performs an "accept"
	 * call.
	 *
	 * @param size of streaming input and output buffers.
	 */
	TCPStream(TCPSocket &server, int size = 512);

	/**
	 * Create a TCP stream by connecting to a TCP socket (on
	 * a remote machine).
	 *
	 * @param host address of remote TCP server.
	 * @param port number to connect.
	 * @param size of streaming input and output buffers.
	 */
	TCPStream(InetHostAddress &host, short port, int size = 512);

	/**
	 * A copy constructor creates a new stream buffer.
	 *
	 * @param source of copy.
	 *
	 */
	TCPStream(const TCPStream &source);

	/**
	 * Flush and empty all buffers, and then remove the allocated
	 * buffers.
	 */
	~TCPStream()
		{endStream();};

	/**
	 * Flushes the stream input and output buffers, writes
	 * pending output.
	 *
	 * @return 0 on success.
	 */
	int sync(void);

	/**
	 * Return the size of the current stream buffering used.
	 *
	 * @return size of stream buffers.
	 */
	int getBufferSize(void)
		{return bufsize;};
};

/**
 * A more natural C++ "tcpstream" class for use by non-threaded
 * applications.  This class behaves a lot more like fstream and
 * similar classes.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short C++ "fstream" style tcpstream class.
 */
class tcpstream : public TCPStream
{
public:
	/**
	 * Construct an unopened "tcpstream" object.
	 */
	tcpstream();

	/**
	 * Construct and "open" (connect) the tcp stream to a remote
	 * socket.
	 *
	 * @param addr string address in form addr:port.
	 * @param buffer size for streaming (optional).
	 */
	tcpstream(char *addr, int buffer = 512);

	/**
	 * Construct and "accept" (connect) the tcp stream through
	 * a server.
	 *
	 * @param tcp socket to accept from.
	 * @param buffer size for streaming (optional).
	 */
	tcpstream(TCPSocket &tcp, int buffer = 512);

	/**
	 * Open a tcp stream connection.  This will close the currently
	 * active connection first.
	 *
	 * @param addr string address in form addr:port.
	 * @param buffer size for streaming (optional)
	 */
	void open(char *addr, int buffer = 512);

	/**
	 * Open a tcp stream connection by accepting a tcp socket.
	 *
	 * @param tcp socket to accept from.
	 * @param buffer size for streaming (optional)
	 */
	void open(TCPSocket &tcp, int buffer = 512);

	/**
	 * Close the active tcp stream connection.
	 */
	void close(void);

	/**
	 * Test to see if stream is open.
	 */
	inline bool operator!()
		{return (Socket::state != SOCKET_CONNECTED);};
};		

/**
 * The TCP session is used to primarily to represent a client connection
 * that can be managed on a seperate thread.  The TCP session also supports
 * a non-blocking connection scheme which prevents blocking during the
 * constructor and moving the process of completing a connection into the 
 * thread that executes for the session.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short Threaded streamable socket with non-blocking constructor.
 */
class TCPSession : public TCPStream, public Thread
{
protected:
	/**
	 * Normally called during the thread Initial() method by default,
	 * this will wait for the socket connection to complete when
	 * connecting to a remote socket.  One might wish to use
	 * setCompletion() to change the socket back to blocking I/O
	 * calls after the connection completes.  To implement the
	 * session one must create a derived class which implements
	 * Run().
	 * 
	 * @return 0 if successful, -1 if timed out.
	 * @param timeout to wait for completion in milliseconds.
	 */
	int WaitConnection(timeout_t timeout = ~0);

	/**
	 * The initial method is used to esablish a connection when
	 * delayed completion is used.  This assures the constructor
	 * terminates without having to wait for a connection request
	 * to complete.
	 */
	void Initial(void);

	/**
	 * TCPSession derived objects can be freely created with "new"
	 * and safely terminate their "Run" method on their own by
	 * self-deleting when the thread terminates.
	 */
	void Final(void)
		{delete this;};
public:
	/**
	 * Create a TCP socket that will be connected to a remote TCP
	 * server and that will execute under it's own thread.
	 * 
	 * @param start semaphore as per Thread startup.
	 * @param host internet address of remote TCP server.
	 * @param port number of remote server.
	 * @param size of streaming buffer.
	 * @param pri execution priority relative to parent.
	 * @param stack allocation needed on some platforms.
	 */
	TCPSession(Semaphore *start, InetHostAddress &host, short port, int size = 512, int pri = 0, int stack = 0);

	/**
	 * Create a TCP socket from a bound TCP server by accepting a pending
	 * connection from that server and execute a thread for the accepted
	 * connection.
	 * 
	 * @param start semapore as per Thread startup.
	 * @param server tcp socket to accept a connection from.
	 * @param size of streaming buffer.
	 * @param pri execution priority relative to parent.
	 * @param stack allocation needed on some platforms.
	 */
	TCPSession(Semaphore *start, TCPSocket &server, int size = 512, int pri = 0, int stack = 0);
};

class SocketPort;
class SocketService;

/**
 * The socket port is an internal class which is attached to and then
 * serviced by a specific SocketService "object".  Derived versions of
 * this class offer specific functionality for specific protocols.  Both
 * Common C++ supporting frameworks and application objects may be derived
 * from related protocol specific base classes.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short base class for realtime and thread pool serviced protocols.
 */
class SocketPort : public Socket, public TimerPort
{
private:
	SocketPort *next, *prev;
	SocketService *service;
	struct timeval porttimer;

	friend class SocketService;

protected:
	/** 
	 * Construct an accepted TCP socket connection from a specific
	 * bound TCP server.  This is meant to derive advanced application
	 * specific TCP servers that can be thread pooled.
	 *
	 * @param svc pool thread object.
	 * @param tcp socket object to accept.
	 */
	SocketPort(SocketService *svc, TCPSocket &tcp);

	/**
	 * Construct a bound UDP socket for use in deriving realtime
	 * UDP streaming protocols handled by thread pool objects.
	 *
	 * @param svc pool thread object.
	 * @param ia address of interface to bind.
	 * @param port number to bind to.
	 */
	SocketPort(SocketService *svc, InetAddress &ia, short port);

	/**
	 * Disconnect the socket from the service thread pool and
	 * the remote connection.
	 */	
	virtual ~SocketPort();

	/**
	 * Called by the service thread pool when the objects timer
	 * has expired.  Used for timed events.
	 */
	virtual void Expired(void)
		{return;};

	/**
	 * Called by the service thread pool when input data is pending
	 * for this socket.
	 */
	virtual void Pending(void)
		{return;};

	/**
	 * Called by the service thread pool when a disconnect has
	 * occured.
	 */
	virtual void Disconnect(void)
		{return;};

	/**
	 * Connect a Socket Port to a known peer host.  This is normally
	 * used with the UDP constructor.  This is also performed as a
	 * non-blocking operation under Posix systems to prevent delays
	 * in a callback handler.
	 *
	 * @return 0 if successful.
	 * @param ia address of remote host or subnet.
	 * @param port number of remote peer(s).
	 */
	sockerror_t Connect(InetAddress &ia, short port);

	/**
	 * Transmit "send" data to a connected peer host.  This is not
	 * public by default since an overriding protocol is likely to
	 * be used in a derived class.
	 *
	 * @return number of bytes sent.
	 * @param address of buffer to send.
	 * @param len of bytes to send.
	 */
	inline int Send(void *buf, int len)
		{return ::send(so, (char *)buf, len, 0);};

	/**
	 * Receive a message from any host.  This is used in derived
	 * classes to build protocols.
	 *
	 * @param pointer to packet buffer to receive.
	 * @param len of packet buffer to receive.
	 * @return number of bytes received.
	 */
	inline int Recv(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, 0);};

	/**
	 * Examine the content of the next packet.  This can be used
	 * to build "smart" line buffering for derived TCP classes.
	 *
	 * @param pointer to packet buffer to examine.
	 * @param len of packet buffer to examine.
	 * @return number of bytes actually available.
	 */
	inline int Peek(void *buf, size_t len)
		{return ::recv(so, (char *)buf, len, MSG_PEEK);};

public:
	/**
	 * Derived setTimer to notify the service thread pool of change
	 * in expected timeout.  This allows SocketService to 
	 * reschedule all timers.  Otherwise same as TimerPort.
	 *
	 * @param timeout in milliseconds.
	 */
	void setTimer(timeout_t timeout = 0);

	/**
	 * Derived incTimer to notify the service thread pool of a
	 * change in expected timeout.  This allows SocketService to
	 * reschedule all timers.  Otherwise same as TimerPort.
	 *
	 * @param timeout in milliseconds.
	 */
	void incTimer(timeout_t timeout);
};

/**
 * The SocketService is a thread pool object that is meant to service
 * attached socket ports.  Multiple pool objects may be created and
 * multiple socket ports may be attached to the same thread of execution.
 * This allows one to balance threads and sockets they service rather than
 * either using a single thread for all connections or a seperate thread
 * for each connection.  Features can be added through supported virtual
 * methods.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short Thread pool service object for socket ports.
 */
class SocketService : public Thread, private Mutex
{
private:
	fd_set connect;
	int iosync[2];
	int hiwater;
	int count;
	SocketPort *first, *last;

	/**
	 * Attach a new socket port to this service thread.
	 *
	 * @param port of SocketPort derived object to attach.
	 */
	void Attach(SocketPort *port);
	/**
	 * Detach a socket port from this service thread.
	 *
	 * @param port of SocketPort derived object to remove.
	 */
	void Detach(SocketPort *port);
	
	/**
	 * The service thread itself.
	 */
	void Run(void);

	friend class SocketPort;

protected:
	/**
	 * Handles all requests other than "termination".
	 *
	 * @param request id as posted from Update().
	 */
	virtual void OnUpdate(unsigned char buf)
		{return;};

	/**
	 * Called once each time the service thread is rescheduled.
	 * This is called after the mutex is locked and can be used to
	 * slip in additional processing.
	 */
	virtual void OnEvent(void)
		{return;};

	/**
	 * Called for each port that is being processed in response to
	 * an event.  This can be used to add additional notification
	 * options during callback in combination with Update().
	 *
	 * @param SocketPort who's callback events are being evaluated.
	 */
	virtual void OnCallback(SocketPort *port)
		{return;};

public:
	/**
	 * Notify service thread that a port has been added or
	 * removed, or a timer changed, so that a new schedule
	 * can be computed for expiring attached ports.  A "0"
	 * is used to terminate the service thread, and additional
	 * values can be specified which will be "caught" in the
	 * OnUpdate() handler.
	 *
	 * @param Update flag value.
	 */
	void Update(unsigned char flag = 0xff);

	/**
	 * Create a service thread for attaching socket ports.  The
	 * thread begins execution with the first attached socket.
	 *
	 * @param pri of this thread to run under.
	 */
	SocketService(int pri = 0);

	/**
	 * Terminate the thread pool and eliminate any attached
	 * socket ports.
	 */
	~SocketService();

	/**
	 * Get current reference count.  This can be used when selecting
	 * the least used service handler from a pool.
	 *
	 * @return count of active ports.
	 */
	inline int getCount(void)
		{return count;};
};

ostream &operator<<(ostream &os, InetAddress &ia);

bool operator==(InetAddress &i1, InetAddress &i2);

inline struct in_addr getaddress(InetAddress &ia)
	{return ia.getAddress();};

#ifdef	__CCXX_NAMESPACE_H__
#undef	__CCXX_NAMESPACE_H__
#include <cc++/namespace.h>
#endif

#endif
