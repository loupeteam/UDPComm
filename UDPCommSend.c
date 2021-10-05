/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: UDPComm
 * File: UDPCommSend.c
 * Author: dfblackburn
 * Created: March 18, 2016
 ********************************************************************
 * Implementation of library UDPComm
 ********************************************************************/

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
// Send UDP datagrams
//----------------------------------------------------------

// Passes first tests

unsigned short UDPCommSend(struct UDPCommMgr_typ* t)
{
	
	// Check input
	if (t == 0) return UDPCOMM_ERR_INVALIDINPUT;

	// Reset DataSent, etc.
	if (!t->IN.CMD.Send || t->IN.PAR.AllowContinuousSend) {
		t->OUT.STAT.DataSent = 0;
		t->OUT.STAT.SentDataLength = 0;
	}

	// Main state machine
	switch (t->Internal.CommState) {

		case UDPCOMM_ST_CLOSED:
		case UDPCOMM_ST_OPENING:
		case UDPCOMM_ST_CLOSING:
		case UDPCOMM_ST_ERROR:
			t->Internal.FUB.Send.enable = 0;
			t->OUT.STAT.DataSent = 0;
			t->OUT.STAT.SentDataLength = 0;
			break;
		
		case UDPCOMM_ST_RUNNING:
			
			// Send some data
			t->Internal.FUB.Send.enable = t->IN.CMD.Send && !t->OUT.STAT.DataSent;
			t->Internal.FUB.Send.ident = t->OUT.STAT.Ident;
			t->Internal.FUB.Send.pHost = (UDINT)&(t->IN.CFG.RemoteInterface.IPAddress);
			t->Internal.FUB.Send.port = t->IN.CFG.RemoteInterface.Port;
			t->Internal.FUB.Send.pData = t->IN.PAR.pSendData;
			t->Internal.FUB.Send.datalen = t->IN.PAR.SendLength;
			t->Internal.FUB.Send.flags = t->IN.PAR.SendFlags;
		
			// Check for !Enable
			if (!t->IN.CMD.Enable) {
				t->Internal.FUB.Send.enable = 0;
			}
		
			break;
		
	} // switch(CommState)

	// Call FUB
	UdpSend(&t->Internal.FUB.Send);

	// Check status
	if (t->Internal.FUB.Send.status == ERR_FUB_BUSY) {
		t->OUT.STAT.Sending = 1;
	} else {
		t->OUT.STAT.Sending = 0;
	}
		
	if (t->Internal.FUB.Send.status < ERR_FUB_ENABLE_FALSE) {
		if (t->Internal.FUB.Send.status == 0) {
			t->OUT.STAT.DataSent = 1;
			t->OUT.STAT.SentDataLength = t->Internal.FUB.Send.sentlen;
		} else {
			t->OUT.STAT.Error = 1;
			t->OUT.STAT.ErrorID = t->Internal.FUB.Send.status;
			strncpy(t->OUT.STAT.ErrorString, "Error sending data.", sizeof(t->OUT.STAT.ErrorString) - 1);
		}
	}
	
	return 0;
	
} // UDPCommSend
