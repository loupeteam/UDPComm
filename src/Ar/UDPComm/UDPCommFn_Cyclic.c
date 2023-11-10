/*
 * File: UDPCommFn_Cyclic.c
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

#ifdef __cplusplus
	};
#endif


#include <string.h>


//--------------------------------------------------
// Cyclic function for managing UDP communications
//--------------------------------------------------

// ALL TEST NOTES ARE INVALID AS OF 20160322

/* Check input										DONE and TESTED DFB 20140131 */

/* Handle AcknowledgeData							DONE and TESTED DFB 20140203 */

/* Reset Internal.CMD.Send and DataSent				DONE and TESTED DFB 20140131 */

/* Main state machine								DONE and TESTED DFB 20140203 */

	/* UDPCOMM_ST_CLOSED							DONE and TESTED DFB 20140131 */
		/* Reset FUB enables						DONE and TESTED DFB 20140131 */
		/* Reset STAT								DONE and TESTED DFB 20140131 */
		/* Handle Enable							DONE and TESTED DFB 20140131 */
		
	/* UDPCOMM_ST_OPENING							DONE and TESTED DFB 20140203 */
		/* Open good								DONE and TESTED DFB 20140203 */
		/* Error opening							DONE and TESTED DFB 20140131 */
		
	/* UDPCOMM_ST_RUNNING							DONE and TESTED DFB 20140203 */

		/* Set CommunicationActive					DONE and TESTED DFB 20140131 */

		/* Receive Data								DONE and TESTED DFB 20140131 */
			/* FUB enable							DONE and TESTED DFB 20140131 */
			/* FUB inputs							DONE and TESTED DFB 20140131 */
			/* Data									DONE and TESTED DFB 20140131 */
			/* No data								DONE and TESTED DFB 20140131 */
			/* Error								DONE and TESTED DFB 20140131 */
			
		/* Send Data								DONE and TESTED DFB 20140203 */

			/* UDPCOMM_SEND_ST_IDLE					DONE and TESTED DFB 20140203 */
				/* CMD latching						DONE and TESTED DFB 20140203 */
				/* Data fits						DONE and TESTED DFB 20140203 */
				/* Data too big						DONE and TESTED DFB 20140203 */

			/* UDPCOMM_SEND_ST_SEND					DONE and TESTED DFB 20140203 */
				/* Done								DONE and TESTED DFB 20140203 */
				/* Error							DONE and TESTED DFB 20140203 */

		/* Handle Enable							DONE and TESTED DFB 20140203 */
			
	/* UDPCOMM_ST_CLOSING							DONE and TESTED DFB 20140203 */
		/* Done										DONE and TESTED DFB 20140203 */
		/* Error									DONE will not be tested */
		
	/* UDPCOMM_ST_ERROR								DONE and TESTED DFB 20140203 */

/* Handle AcknowledgeError							DONE and TESTED DFB 20140203 */

	
unsigned short UDPCommFn_Cyclic(struct UDPCommMgr_typ* t)
{

	if (t == 0) return UDPCOMM_ERR_INVALIDINPUT;

	UDPCommManage(t);
	UDPCommReceive(t);
	UDPCommSend(t);
	
	return 0;

} // UDPCommFn_Cyclic()
