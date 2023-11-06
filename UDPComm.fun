(*
 * File: UDPComm.fun
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of UDPComm, licensed under the MIT License.
 *
 *)

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
