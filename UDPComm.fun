(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: UDPComm
 * File: UDPComm.fun
 * Author: davidblackburn
 * Created: January 28, 2014
 ********************************************************************
 * Functions and function blocks of library UDPComm
 ********************************************************************)

FUNCTION UDPCommFn_Cyclic : UINT (*Cyclic function for managing UDP communications*) (*$GROUP=User*)
	VAR_IN_OUT
		t : UDPCommMgr_typ;
	END_VAR
END_FUNCTION

FUNCTION UDPCommManage : UINT (*Open and close UDP communication on the local interface*) (*$GROUP=User*)
	VAR_IN_OUT
		t : UDPCommMgr_typ;
	END_VAR
END_FUNCTION

FUNCTION UDPCommReceive : UINT (*Receive UDP datagrams*) (*$GROUP=User*)
	VAR_IN_OUT
		t : UDPCommMgr_typ;
	END_VAR
END_FUNCTION

FUNCTION UDPCommSend : UINT (*Send UDP datagrams*) (*$GROUP=User*)
	VAR_IN_OUT
		t : UDPCommMgr_typ;
	END_VAR
END_FUNCTION
