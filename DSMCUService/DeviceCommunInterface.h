// DeviceCommunication.h : header file

// NOTE:
// implement MCU (Cypress) Device communication
// based on Module COMPort
// 

// interface


#pragma once


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "COMPort.h"


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define FRAME_SIZE						64

#define DEVICE_OP_SUCCESS				0x00
#define DEVICE_IS_EMPTY					0x01
#define DEVICE_OP_FAILURE				0x02
#define DEVICE_OP_MISMATCH				0x03

#define OID_TYPE_SHOW					0x00
#define OID_TYPE_SET					0x01
#define OID_TYPE_CMD					0x02


/////////////////////////////////////////////////////////////////////////////
// Project Data Types 
/////////////////////////////////////////////////////////////////////////////
typedef HANDLE hSerialCDC;
typedef hSerialCDC hUTBDevice;
typedef UCHAR channelFrame[FRAME_SIZE];


// ##########################################################################
// CDeviceCommunInterface
// ##########################################################################
class CDeviceCommunInterface
{

public:
	// Construction
	CDeviceCommunInterface(hSerialCDC * hSerialCDC);
	CDeviceCommunInterface();

	~CDeviceCommunInterface();


	// > Properties
	// config
	DWORD m_dwComNum = 0;					// expected COM Port Number
	WORD  m_TransactionCnt = 0;
	DWORD m_dwBufferSize;

	// main Device pointer
	hUTBDevice m_hUTBDevice;


	// > Events
	// init
	void Init();

	// connection
	int Connect(DWORD dwComNum);
	int Disconnect();
	int Reconnect();

	// cmd: simple
	int Send(channelFrame frInput);
	int SendRequest(UCHAR ucRequestType, UCHAR ucParameter);
	int Get(channelFrame * frOutput);

	// cmd: combined
	int ShowTable(UCHAR ucTableNum, channelFrame * frTableOutput);
	

};

