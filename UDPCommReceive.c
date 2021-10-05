/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: UDPComm
 * File: UDPCommReceive.c
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
// Receive UDP datagrams
//----------------------------------------------------------

// Passes first tests

unsigned short UDPCommReceive(struct UDPCommMgr_typ* t)
{

	// Check input
	if (t == 0) return UDPCOMM_ERR_INVALIDINPUT;

	// Reset NewDataAvailable, etc.
	if (t->IN.CMD.AcknowledgeData || t->IN.PAR.AllowContinuousReceive) {
		t->OUT.STAT.NewDataAvailable = 0;
		t->OUT.STAT.ReceivedDataLength = 0;
		memset(&(t->OUT.STAT.ReceivedFromInterface.IPAddress), 0, sizeof(t->OUT.STAT.ReceivedFromInterface.IPAddress));
		t->OUT.STAT.ReceivedFromInterface.Port = 0;
	}
	
	t->IN.CMD.AcknowledgeData = 0;

	// Main state machine
	switch (t->Internal.CommState) {

		case UDPCOMM_ST_CLOSED:
		case UDPCOMM_ST_OPENING:
		case UDPCOMM_ST_CLOSING:
		case UDPCOMM_ST_ERROR:
			t->Internal.FUB.Receive.enable = 0;
			t->OUT.STAT.NewDataAvailable = 0;
			t->OUT.STAT.ReceivedDataLength = 0;
			memset(&(t->OUT.STAT.ReceivedFromInterface.IPAddress), 0, sizeof(t->OUT.STAT.ReceivedFromInterface.IPAddress));
			t->OUT.STAT.ReceivedFromInterface.Port = 0;
			break;
		
		case UDPCOMM_ST_RUNNING:
			
			// Listen for data
			t->Internal.FUB.Receive.enable = !t->OUT.STAT.NewDataAvailable;
			t->Internal.FUB.Receive.ident = t->OUT.STAT.Ident;
			t->Internal.FUB.Receive.pData = t->IN.PAR.pReceiveData;
			t->Internal.FUB.Receive.datamax = t->IN.PAR.MaxReceiveLength;
			t->Internal.FUB.Receive.flags = t->IN.PAR.ReceiveFlags;
			t->Internal.FUB.Receive.pIpAddr = (UDINT)&(t->Internal.ReceivedFromIPAddress);
				
			// Check for !Enable
			if (!t->IN.CMD.Enable) {
				t->Internal.FUB.Receive.enable = 0;
			}
		
			break;
	
	} // switch(CommState)

	// Call FUB
	UdpRecv(&t->Internal.FUB.Receive);

	// Check status
	if (t->Internal.FUB.Receive.status < ERR_FUB_ENABLE_FALSE) {
		if (t->Internal.FUB.Receive.status == 0) {
			t->OUT.STAT.NewDataAvailable = 1;
			t->OUT.STAT.ReceivedDataLength = t->Internal.FUB.Receive.recvlen;
			memcpy(&(t->OUT.STAT.ReceivedFromInterface.IPAddress), &(t->Internal.ReceivedFromIPAddress), sizeof(t->OUT.STAT.ReceivedFromInterface.IPAddress));
			t->OUT.STAT.ReceivedFromInterface.Port = t->Internal.FUB.Receive.port;
		} else if( t->Internal.FUB.Receive.status == udpERR_NO_DATA ){
			// No data. Keep listening.
		} else {
			t->OUT.STAT.Error = 1;
			t->OUT.STAT.ErrorID = t->Internal.FUB.Receive.status;
			strncpy(t->OUT.STAT.ErrorString, "Error receiving data. Disable communications and check CFG values.", sizeof(t->OUT.STAT.ErrorString) - 1);
		}		
	}
	
	return 0;

} // UDPCommReceive()
