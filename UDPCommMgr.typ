(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: UDPComm
 * File: UDPCommMgr.typ
 * Author: davidblackburn
 * Created: January 28, 2014
 ********************************************************************
 * Data types of library UDPComm
 ********************************************************************)

TYPE
	UDPCOMM_ERR_enum : 
		(
		UDPCOMM_ERR_INVALIDINPUT := 50000,
		UDPCOMM_ERR_RECVDATATOOLARGE,
		UDPCOMM_ERR_SENDDATATOOLARGE,
		UDPCOMM_ERR_
		);
	UDPCOMM_ST_enum : 
		(
		UDPCOMM_ST_CLOSED,
		UDPCOMM_ST_OPENING,
		UDPCOMM_ST_RUNNING,
		UDPCOMM_ST_CLOSING,
		UDPCOMM_ST_ERROR
		);
	UDPCOMM_SEND_ST_enum : 
		(
		UDPCOMM_SEND_ST_IDLE,
		UDPCOMM_SEND_ST_SEND
		);
	UDPCommMgr_Int_FUB_typ : 	STRUCT 
		Open : UdpOpen;
		Receive : UdpRecv;
		Send : UdpSend;
		Close : UdpClose;
	END_STRUCT;
	UDPCommMgr_Internal_typ : 	STRUCT 
		CommState : UDPCOMM_ST_enum;
		ReceivedFromIPAddress : STRING[UDPCOMM_STRLEN_IPADDRESS];
		FUB : UDPCommMgr_Int_FUB_typ;
	END_STRUCT;
	UDPCommMgr_OUT_STAT_typ : 	STRUCT 
		CommunicationActive : BOOL;
		Sending : BOOL;
		DataSent : BOOL;
		SentDataLength : UDINT;
		NewDataAvailable : BOOL;
		ReceivedDataLength : UDINT;
		ReceivedFromInterface : UDPCommMgr_Interface_typ;
		Ident : UDINT;
		Error : BOOL;
		ErrorID : UINT;
		ErrorString : STRING[UDPCOMM_STRLEN_ERRORSTRING];
	END_STRUCT;
	UDPCommMgr_OUT_typ : 	STRUCT 
		STAT : UDPCommMgr_OUT_STAT_typ;
	END_STRUCT;
	UDPCommMgr_Interface_typ : 	STRUCT 
		IPAddress : STRING[UDPCOMM_STRLEN_IPADDRESS] := 'IF1';
		Port : UINT;
	END_STRUCT;
	UDPCommMgr_IN_CFG_typ : 	STRUCT 
		LocalInterface : UDPCommMgr_Interface_typ;
		RemoteInterface : UDPCommMgr_Interface_typ;
	END_STRUCT;
	UDPCommMgr_IN_PAR_typ : 	STRUCT 
		AllowContinuousSend : BOOL;
		SendFlags : UINT;
		pSendData : UDINT;
		SendLength : UDINT;
		AllowContinuousReceive : BOOL;
		ReceiveFlags : UINT;
		pReceiveData : UDINT;
		MaxReceiveLength : UDINT;
		DisableReceiving : BOOL;
	END_STRUCT;
	UDPCommMgr_IN_CMD_typ : 	STRUCT 
		Enable : BOOL;
		Send : BOOL;
		AcknowledgeData : BOOL;
		AcknowledgeError : BOOL;
	END_STRUCT;
	UDPCommMgr_IN_typ : 	STRUCT 
		CMD : UDPCommMgr_IN_CMD_typ;
		PAR : UDPCommMgr_IN_PAR_typ;
		CFG : UDPCommMgr_IN_CFG_typ;
	END_STRUCT;
	UDPCommMgr_typ : 	STRUCT 
		IN : UDPCommMgr_IN_typ;
		OUT : UDPCommMgr_OUT_typ;
		Internal : UDPCommMgr_Internal_typ;
	END_STRUCT;
END_TYPE
