// DeviceCommunInterface.cpp : implementation file

// implementation


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DeviceCommunInterface.h"


// -------------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------------
CDeviceCommunInterface::CDeviceCommunInterface()
{

}

// Active constructor
CDeviceCommunInterface::CDeviceCommunInterface(hSerialCDC * hSerialCDC)
	: m_hUTBDevice(*hSerialCDC)
{

}


CDeviceCommunInterface::~CDeviceCommunInterface()
{

}


// init 
void CDeviceCommunInterface::Init()
{


}


// -------------------------------------------------------------------
// CORE COMMUNICATE FUNTIONALITY
// -------------------------------------------------------------------
BYTE CDeviceCommunInterface::Connect(DWORD dwComNum)
{
	int iResult = COMPort_Open(&this->m_hUTBDevice, dwComNum);

	if (iResult == DEVICE_OP_SUCCESS)
	{
		// [VALID PORT ADDR]

		m_dwComNum = dwComNum;
	}

	return iResult;
}


BYTE CDeviceCommunInterface::Send(channelFrame frInput)
{
	// NOTE:
	// Universal Send/Transceive procedure

	DWORD dwBytesWritten = 0;
	int iResult = COMPort_Write(&m_hUTBDevice, frInput, &dwBytesWritten);


	return DEVICE_OP_SUCCESS;
}


BYTE CDeviceCommunInterface::SendRequest(UCHAR ucRequestType, UCHAR ucParameter)
{
	// NOTE:
	// Custom purposs Requests
	// PROC Next Requests:
	// OID = BYTE01
	// PAR = BYTE02


	return 0;
}


BYTE CDeviceCommunInterface::Get(channelFrame * frOutput)
{
	// NOTE:
	// Universal Get/Receive procedure

	DWORD dwBytesRead = 0;
	int iResult = COMPort_Read(&m_hUTBDevice, *frOutput, &dwBytesRead);

	return DEVICE_OP_SUCCESS;
}


BYTE CDeviceCommunInterface::Disconnect()
{


	return DEVICE_OP_SUCCESS;
}


BYTE CDeviceCommunInterface::Reconnect()
{


	return DEVICE_OP_SUCCESS;
}


// -------------------------------------------------------------------
// TABLE MONITOR FUNTIONALITY
// -------------------------------------------------------------------
// Send Request to Show Table, Get response with referred Table 
BYTE CDeviceCommunInterface::ShowTable(UCHAR ucTableNum, channelFrame * frTableOutput)
{
	// *** Send Request: SHOW ***

	// new Transaction
	m_TransactionCnt++;

	// > Define frame
	un_FRAME_COMMON frame_instance;

	// Set Header
	frame_instance.structVal.structHeaderVal.ucOID = OID_TYPE_SHOW;
	frame_instance.structVal.structHeaderVal.ucPAR = ucTableNum;
	frame_instance.structVal.structHeaderVal.usTRN = m_TransactionCnt;

	// Reset Body
	frame_instance.structVal.unionBodyVal.structVal_TABLE_BOARD_SERVICE.structTableVal.ucDCAllState = 0;
	frame_instance.structVal.unionBodyVal.structVal_TABLE_BOARD_SERVICE.structTableVal.ucDCMainState = 0;
	frame_instance.structVal.unionBodyVal.structVal_TABLE_BOARD_SERVICE.structTableVal.ucMCUInitState = 0;
	frame_instance.structVal.unionBodyVal.structVal_TABLE_BOARD_SERVICE.structTableVal.ucUSBConnectState = 0;

	
	//channelFrame frRequest;
	
	//frRequest[0] = OID_TYPE_SHOW;
	//frRequest[1] = ucTableNum;
	//frRequest[2] = m_TransactionCnt;


	// > Send Request Frame
	int iResult = Send(frame_instance.frameVal);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}

	// *** Get Response: SHOW ***

	// > Get Response Frame
	iResult = Get(frTableOutput);


	return iResult;
}


// -------------------------------------------------------------------
// I2C FUNTIONALITY
// -------------------------------------------------------------------

// Set Initialisation Part of I2C Read routine
// NOTE:
// Address = 0 to 127
// Min Read Byte Count = 1
// Max Read Byte Count = 256
// Modes: 0 = Normal, 1 = ReStart, 2 = No NACK 
BYTE CDeviceCommunInterface::I2C_Read_Init_Request(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * ucErrCode)
{
	// > Check Validity of Params
	BYTE ucParamInvalid = 0;

	if (ucAddress > 127)
	{
		ucParamInvalid = 1;
	}

	if ( (ucCount < 1) || (ucCount > 256) )
	{
		ucParamInvalid = 1;
	}

	if (ucMode > 10)
	{
		ucParamInvalid = 1;
	}

	if (ucParamInvalid)
	{
		return I2C_OP_INVALID_PARAM;
	}


	// *** Send Request: I2C_READ INIT ***

	// new Transaction
	m_TransactionCnt++;

	// > Define frame
	un_FRAME_COMMON frame_instance;

	// Set Header
	frame_instance.structVal.structHeaderVal.ucOID = OID_TYPE_I2C;
	frame_instance.structVal.structHeaderVal.ucPAR = I2C_TYPE_PROC;
	frame_instance.structVal.structHeaderVal.usTRN = m_TransactionCnt;

	// Set I2C Request Line
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucAddressSLA = ucAddress;
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucCount = ucCount - 1;
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucReadWriteType = I2C_READ_REQUEST;
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucMode = I2C_MODE_NORMAL;

	// > Send Request Frame
	BYTE iResult = Send(frame_instance.frameVal);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}


	return iResult;

}


BYTE CDeviceCommunInterface::I2C_Read_Init_Response(BYTE * ucStatus, BYTE * ucErrCode)
{
	// *** Get Response: I2C ***

	// > Get Response Frame
	un_FRAME_COMMON frame_output;

	BYTE iResult = Get(&frame_output.frameVal);

	// Proceed Body section with Response Data
	*ucStatus = frame_output.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_RESPONSE.structTableVal.ucStatus;
	*ucErrCode = frame_output.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_RESPONSE.structTableVal.ucErrCode;

	return iResult;
}

// Set Transmittion Part of I2C Read routine
// NOTE:
// 1 FRAME can Transfer up to 56 Bytes MAXIMUM
// Max Byte Count to Receive: 256
BYTE CDeviceCommunInterface::I2C_Read_Trasm_Request(BYTE ucBlockNumber)
{	
	// *** Send Request: I2C_READ TRASM ***

	// new Transaction
	m_TransactionCnt++;

	// > Define frame
	un_FRAME_COMMON frame_instance;

	// Set Header
	frame_instance.structVal.structHeaderVal.ucOID = OID_TYPE_I2C;
	frame_instance.structVal.structHeaderVal.ucPAR = I2C_TYPE_TRASM_GET;
	frame_instance.structVal.structHeaderVal.usTRN = m_TransactionCnt;

	// Set I2C Request Line
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_TRASM_REQUEST.structTableVal.ucBlockNumber = ucBlockNumber;

	// > Send Request Frame
	BYTE iResult = Send(frame_instance.frameVal);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}


	return iResult;
}

// NOTE: in usual case Need 2Byte Var to send Read Bytes Count
//		 but here used a way, where Count_Parameter = Count_Source - 1,
//		 so Value = 256 can be transfered in 1Byte var as 256-1 = 255.
BYTE CDeviceCommunInterface::I2C_Read_Trasm_Response(BYTE * v_Data, BYTE * ucBlockNumber)
{
	// *** Get Response: I2C ***

	// > Get Response Frame
	un_FRAME_COMMON frame_instance;

	BYTE iResult = Get(&frame_instance.frameVal);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}

	// > Proceed Body section with Response Data
	*ucBlockNumber = frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_TRASM_REQUEST.structTableVal.ucBlockNumber;

	// Check Buffer Limit: 256 Bytes
	WORD ucDataSize = I2C_DATA_SIZE;

	if (*ucBlockNumber > 3)
	{
		ucDataSize = 256 - I2C_DATA_SIZE * 4;
	}

	// Copy Response Data
	for (BYTE k = 0; k < ucDataSize; k++)
	{
		v_Data[k] = frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_TRASM_REQUEST.structTableVal.v_Data[k];
	}	 


	return iResult;
}


// *** I2C WRITE ***

// Set Initialisation Part of I2C Write routine
// NOTE:
// Address = 0 to 127
// Min Write Byte Count = 1
// Max Write Byte Count = 256
// Modes: 0 = Normal, 1 = ReStart
BYTE CDeviceCommunInterface::I2C_Write_Init_Request(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * ucErrCode)
{
	// > Check Validity of Params
	BYTE ucParamInvalid = 0;

	if (ucAddress > 127)
	{
		ucParamInvalid = 1;
	}

	if ((ucCount < 1) || (ucCount > 256))
	{
		ucParamInvalid = 1;
	}

	if (ucMode > 10)
	{
		ucParamInvalid = 1;
	}

	if (ucParamInvalid)
	{
		return I2C_OP_INVALID_PARAM;
	}


	// *** Send Request: I2C_WRITE INIT ***

	// new Transaction
	m_TransactionCnt++;

	// > Define frame
	un_FRAME_COMMON frame_instance;

	// Set Header
	frame_instance.structVal.structHeaderVal.ucOID = OID_TYPE_I2C;
	frame_instance.structVal.structHeaderVal.ucPAR = I2C_TYPE_PROC;
	frame_instance.structVal.structHeaderVal.usTRN = m_TransactionCnt;

	// Set I2C Request Line
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucAddressSLA = ucAddress;
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucCount = ucCount - 1;		// '-1' to write 256 in Byte var.
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucReadWriteType = I2C_WRITE_REQUEST;
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_REQUEST.structTableVal.ucMode = I2C_MODE_NORMAL;

	// > Send Request Frame
	BYTE iResult = Send(frame_instance.frameVal);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}


	return iResult;
}


BYTE CDeviceCommunInterface::I2C_Write_Init_Response(BYTE * ucStatus, BYTE * ucErrCode)
{
	// *** Get Response: I2C ***

	// > Get Response Frame
	un_FRAME_COMMON frame_output;

	BYTE iResult = Get(&frame_output.frameVal);

	// Proceed Body section with Response Data
	*ucStatus = frame_output.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_RESPONSE.structTableVal.ucStatus;
	*ucErrCode = frame_output.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_RESPONSE.structTableVal.ucErrCode;

	return iResult;
}


BYTE CDeviceCommunInterface::I2C_Write_Trasm_Request(BYTE * v_Data, BYTE ucBlockNumber)
{
	// *** Set Trasm Request: I2C ***

	// new Transaction
	m_TransactionCnt++;

	// > Define frame
	un_FRAME_COMMON frame_instance;

	// Set Header
	frame_instance.structVal.structHeaderVal.ucOID = OID_TYPE_I2C;
	frame_instance.structVal.structHeaderVal.ucPAR = I2C_TYPE_TRASM_SET;
	frame_instance.structVal.structHeaderVal.usTRN = m_TransactionCnt;

	// > Proceed Body section with Request Data
	frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_TRASM_REQUEST.structTableVal.ucBlockNumber = ucBlockNumber;

	// > Set specific Buffer with Data block
	WORD ucDataSize = I2C_DATA_SIZE;

	// Copy Request Data
	for (BYTE k = 0; k < ucDataSize; k++)
	{
		frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_TRASM_REQUEST.structTableVal.v_Data[k] = v_Data[k];
	}

	// > Send Request Frame
	BYTE iResult = Send(frame_instance.frameVal);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}


	return iResult;
}


BYTE CDeviceCommunInterface::I2C_Write_Trasm_Response(BYTE * ucStatus, BYTE * ucErrCode)
{
	// *** Get Response: I2C ***

	// > Get Response Frame
	un_FRAME_COMMON frame_instance;

	BYTE iResult = Get(&frame_instance.frameVal);

	// > Proceed Body section with Response Data
	*ucStatus = frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_RESPONSE.structTableVal.ucStatus;
	*ucErrCode = frame_instance.structVal.unionBodyVal.structVal_I2C_TYPE_PROC_RESPONSE.structTableVal.ucErrCode;


	return iResult;
}


BYTE CDeviceCommunInterface::I2C_Read_Proc(BYTE * v_Data)
{

	return 0;
}
