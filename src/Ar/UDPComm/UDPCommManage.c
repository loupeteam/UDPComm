/*
 * File: UDPCommManage.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of UDPComm, licensed under the MIT License.
 * 
 */

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "UDPComm.h"
#include <string.h>

#ifdef __cplusplus
	};
#endif

//----------------------------------------------------------
// Open and close UDP communication on the local interface
//----------------------------------------------------------

unsigned short UDPCommManage(struct UDPCommMgr_typ* t)
{

	// Check input
	if (t == 0) return UDPCOMM_ERR_INVALIDINPUT;
	
	// Default CommunicationActive to 0
	t->OUT.STAT.CommunicationActive = 0;

	// Main state machine
	switch( t->Internal.CommState ){


		case UDPCOMM_ST_CLOSED:
	
			// Reset FUB enables
			t->Internal.FUB.Open.enable = 0;
			t->Internal.FUB.Close.enable = 0;
		
			// Reset status
			memset( &(t->OUT.STAT), 0, sizeof(t->OUT.STAT) );
		
			// Wait for Enable and then open port
			if (t->IN.CMD.Enable) {
		
				t->Internal.FUB.Open.enable = 1;
			
				if (strcmp(t->IN.CFG.LocalInterface.IPAddress, "") == 0) {
					t->Internal.FUB.Open.pIfAddr = 0;
				} else {
					t->Internal.FUB.Open.pIfAddr = (UDINT)&(t->IN.CFG.LocalInterface.IPAddress);
				}
			
				t->Internal.FUB.Open.port = t->IN.CFG.LocalInterface.Port;
				t->Internal.FUB.Open.options = 0;
			
				t->Internal.CommState = UDPCOMM_ST_OPENING;
			
			}
		
			break;
		
	
		case UDPCOMM_ST_OPENING:
	
			// Wait for port to be open and then go to ST_RUNNING
			if (t->Internal.FUB.Open.status < ERR_FUB_ENABLE_FALSE) {
		
				t->Internal.FUB.Open.enable = 0;
			
				if ((t->Internal.FUB.Open.status == 0) || (t->Internal.FUB.Open.status == udpERR_ALREADY_EXIST)) {			
			
					// All good.
					t->OUT.STAT.Ident = t->Internal.FUB.Open.ident;
					t->Internal.CommState = UDPCOMM_ST_RUNNING;
			
				} else {
			
					// Error. Probably fatal.  Wait for Enable to go false then recover.
					t->OUT.STAT.Error = 1;
					t->OUT.STAT.ErrorID = t->Internal.FUB.Open.status;
					strncpy( t->OUT.STAT.ErrorString, "Error opening port. Check CFG values.", sizeof(t->OUT.STAT.ErrorString) - 1 );
				
					t->Internal.CommState=	UDPCOMM_ST_ERROR;
				
				}
			
			}
		
			break;
		
		
		case UDPCOMM_ST_RUNNING:
	
			t->OUT.STAT.CommunicationActive = 1;
		
			if (!t->IN.CMD.Enable) {
		
				// Close socket
				t->Internal.FUB.Open.enable = 0;

				t->Internal.FUB.Close.enable = 1;
				t->Internal.FUB.Close.ident = t->OUT.STAT.Ident;
			
				t->Internal.CommState=	UDPCOMM_ST_CLOSING;
			
			}
		
			break;
		
	
		case UDPCOMM_ST_CLOSING:
	
			// Wait for socket to close and go to ST_CLOSED
			if (t->Internal.FUB.Close.status < ERR_FUB_ENABLE_FALSE) {
		
				t->Internal.FUB.Close.enable = 0;

				t->OUT.STAT.Ident = 0;
			
				if (t->Internal.FUB.Close.status != 0) {
			
					// Error. Weird. This error will be hard to see, since errors are cleared when you 
					// get back to UDPCOMM_ST_CLOSED. 
					
					t->OUT.STAT.Error = 1;
					t->OUT.STAT.ErrorID = t->Internal.FUB.Close.status;
					strncpy( t->OUT.STAT.ErrorString, "Error closing port.", sizeof(t->OUT.STAT.ErrorString) - 1 );
				
				}
			
				t->Internal.CommState=	UDPCOMM_ST_CLOSED;			
			
			}
	
			break;
		

		case UDPCOMM_ST_ERROR:
	
			// Reset FUB enables
			t->Internal.FUB.Open.enable = 0;
			t->Internal.FUB.Close.enable = 0;
		
			// Wait for Enable to be FALSE
			if (!t->IN.CMD.Enable) {
				t->Internal.CommState=	UDPCOMM_ST_CLOSED;
			}
		
			break;
		

	} // switch(CommState) //

	// Call FUBs
	UdpOpen(&t->Internal.FUB.Open);
	UdpClose(&t->Internal.FUB.Close);

	// Handle AcknowledgeError
	if (t->IN.CMD.AcknowledgeError) {
		t->OUT.STAT.Error = 0;
		t->OUT.STAT.ErrorID = 0;
		strcpy(t->OUT.STAT.ErrorString, "");
	}

	t->IN.CMD.AcknowledgeError=	0;

	return 0;
	
} // UDPCommManage()
