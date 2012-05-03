#ifndef _rA_NETWORK_H_
#define _rA_NETWORK_H_

#ifndef WIN32
#include <netinet/in.h>
#endif

#include "../common/cbasetypes.h"
#include "../common/netbuffer.h"


void network_init();
void network_final();

typedef enum SESSION_TYPE {
	NST_FREE = 0, // Free Session / Not Connected / Unused
	NST_INCOMMING,  // Normal Incomming Connection  (Client or whatever..)
	NST_OUTGOING,	// Normal outgoing Connection (created using connect.)
	NST_LISTENER 	// Listener
} SESSION_TYPE;


typedef struct SESSION{
	SESSION_TYPE type;
	
	// Flag to prevent recursive disconnection
	// could happen when using net_send in OnDisconenct handler
	// while the connection has been closed from the opposide
	bool disconnect_in_progress;
	
	
	// Incase of listener, this is the listener 
	// address, otherwise the remote endpoint
	union {
		struct sockaddr_in	v4;
		struct sockaddr_in6	v6;
	} addr;
	bool is_v6; 

	
	// Event Handlers (implemented by the corresponding protocol handler)
	bool (*onRecv)(int connection); // return false = disconnect
	bool (*onSend)(int connection); // return false = disconnect
	
	
	// Event Handlers for LISTENER type Socekts
	// 
	// onConnect  gets called when a connection has been
	//  successfully accepted.
	//  Session entry is available in the Handler.
	//	A returncode of false will reject the connection (disconnect) 
	//  Note: when rejecting a connection in onConnect by returning false
	//		  The onDisconnect handler wont get called!
	//  Note: the onConnect Handler is also responsible for setting 
	//		  the appropriate netparser (which implemens onRecv and onSend ..) [protocol specific] 
	//
	// onDisconnect  gets called when a connection gets disconnected
	//	(by peer as well as by core)
	//
	bool (*onConnect)(int connection);
	bool (*onDisconnect)(int connection);
	
	// Payload for the protocol parser
	void *netparser_data;
	

	///
	/// RO Protocol Specific (payload for the ro packet parser)
	///
	// Note:
	// len includes payload; the netbuffer also contains OP (and if its a dynamic packet the length)
	// buffer is always attached containg op (+len) at beginning, so the packet passtrhu is easier (when writing a proxy / dispatcher)
	// 
	// Note:
	// YOU have to take care, that the netbuf will returned to the pool! (netbuffer_put)
	//
	void (*onPacketComplete)(int connection,  uint16 op,  uint16 len,  netbuf data);
	
	
	///
	/// User Data PTR
	///
	void *data;
	
} SESSION;



#endif
