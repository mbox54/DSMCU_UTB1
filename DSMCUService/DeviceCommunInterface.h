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

#define DEVICE_I2C_RTXBUF_SIZE			256 
#define I2C_DATA_SIZE					56  
#define I2C_TIMEOUT_LIMIT          		200		// 200[ms]

// Communicate Status
#define DEVICE_OP_SUCCESS				0x00
#define DEVICE_IS_EMPTY					0x01
#define DEVICE_OP_FAILURE				0x02
#define DEVICE_OP_MISMATCH				0x03

// OID Type
#define OID_TYPE_NULL					0x00
#define OID_TYPE_SHOW					0x01
#define OID_TYPE_SET					0x02
#define OID_TYPE_CMD					0x03	
#define OID_TYPE_I2C					0x04	

// Table Type
#define TABLE_BOARD_SERVICE				0x01
#define TABLE_BOARD_PORTS				0x02
#define TABLE_POWER_SOURCE				0x03

// Set
#define SET_BOARD_SERVICE				0x01
#define SET_BOARD_PORTS					0x02
#define SET_POWER_SOURCE				0x03

// i2c Frame Type
#define I2C_TYPE_PROC					0x00
#define I2C_TYPE_TRASM_GET				0x01
#define I2C_TYPE_TRASM_SET				0x02

// i2c init PROC Type
#define I2C_WRITE_REQUEST				0x00
#define I2C_WRITE_RESPONSE				0x00
#define I2C_READ_REQUEST				0x01
#define I2C_READ_RESPONSE				0x01

// i2c Mode
#define I2C_MODE_NORMAL					0x00
#define I2C_MODE_CUSTOM					0x01

// i2c OP Status
#define I2C_OP_SUCCESS            		0x00    
#define I2C_OP_ERROR		     		0x01
#define I2C_OP_TIMEOUT		     		0x02
#define I2C_OP_INVALID_PARAM     		0x03

// i2c ErrCode
#define I2C_ERRCODE_NO_ERROR       		0x00 


/////////////////////////////////////////////////////////////////////////////
// Project Data Types 
/////////////////////////////////////////////////////////////////////////////
typedef HANDLE hSerialCDC;
typedef hSerialCDC hUTBDevice;

typedef UCHAR channelFrame[FRAME_SIZE];


// ------ TABLE STRUCTS ------ // 
// struct Tables
struct st_TABLE_BOARD_SERVICE
{
	// 4 BYTE
	BYTE ucUSBConnectState;				// NOTE: 0 = NULL, 1 = Valid, 2 = Invalid
	BYTE ucDCMainState;					// NOTE: 0 = NULL, 1 = Active, 2 = Failure
	BYTE ucDCAllState;					// NOTE: 0 = NULL, 1 = Active, 2 = Failure
	BYTE ucMCUInitState;				// NOTE: 0 = NULL, 1 = Inited, 2 = Failure
										// ..

};

struct st_TABLE_BOARD_PORTS
{

	// 2 BYTE
	BYTE ucModAbsState;					// NOTE: 0 = Module Absent, 1 = Module Present
	BYTE ucModTypeNum;					// NOTE: 0 = NULL, X = Type: 
										// FORMAT: 1 = , 2 = , 3 = , 4 = , 5 = , 6 = , 7 = .
										// ..
};

struct st_TABLE_Power_Source
{
	// 7 BYTE
	BYTE bV_IN_Proh;
	BYTE b1V8_Proh;
	BYTE b3V3_Proh;
	BYTE b5V_State;
	BYTE b5V_Enable;
	BYTE b3V3_Enable;
	BYTE b1V8_Enable;
};


// SET Tables
struct st_SET_TABLE_Power_Source
{
	// 7 BYTE
	BYTE bV_IN_Proh;
	BYTE b1V8_Proh;
	BYTE b3V3_Proh;
	BYTE b5V_State;
	BYTE b5V_Enable;
	BYTE b3V3_Enable;
	BYTE b1V8_Enable;
};

struct st_SET_TABLE_RESPONSE
{
	// 1 BYTE
	BYTE ucOperation_status;

};


// // struct i2c
// [I2C]
struct st_I2C_TYPE_PROC_REQUEST
{
	// 4 BYTE
	BYTE ucReadWriteType;
	BYTE ucAddressSLA;
	BYTE ucCount;
	BYTE ucMode;
};

struct st_I2C_TYPE_PROC_RESPONSE
{
	// 2 BYTE
	BYTE ucStatus;
	BYTE ucErrCode;
};

struct st_I2C_TYPE_TRASM_REQUEST
{
	// 57 BYTE
	BYTE ucBlockNumber;
	BYTE v_Data[I2C_DATA_SIZE];
};

struct st_I2C_TYPE_TRASM_RESPONSE
{
	// 2 BYTE
	BYTE ucStatus;
	BYTE ucErrCode;
};

// ------ FRAME PROTORYTES ------ // 
// struct Frame Header
struct st_FrameHeader
{
	BYTE ucOID;							// Operation ID
	BYTE ucPAR;							// Parameter
	WORD usTRN;							// Transaction
};


// specific, body type
// [TABLES]
struct st_FRAME_TABLE_BOARD_SERVICE
{
	// 4 BYTE
	struct st_TABLE_BOARD_SERVICE structTableVal;

	// 56 BYTE
	BYTE v_Reserved[56];

};

struct st_FRAME_TABLE_BOARD_PORTS
{
	// 2 BYTE
	struct st_TABLE_BOARD_PORTS structTableVal;

	// 58 BYTE
	BYTE v_Reserved[58];
};

struct st_FRAME_TABLE_Power_Source
{
	// 7 BYTE
	struct st_TABLE_Power_Source structTableVal;

	// 53 BYTE
	BYTE v_Reserved[53];
};


// [SET TABLES]
struct st_FRAME_SET_TABLE_Power_Source
{
	// 7 BYTE
	struct st_SET_TABLE_Power_Source structTableVal;

	// 53 BYTE
	BYTE v_Reserved[53];
};

struct st_FRAME_SET_TABLE_RESPONSE
{
	// 1 BYTE
	struct st_SET_TABLE_RESPONSE structTableVal;

	// 59 BYTE
	BYTE v_Reserved[59];
};


// [I2C]
struct st_FRAME_I2C_TYPE_PROC_REQUEST
{
	// 4 BYTES
	struct st_I2C_TYPE_PROC_REQUEST structTableVal;

	// 56 BYTES
	BYTE v_Reserved[56];
};

struct st_FRAME_I2C_TYPE_PROC_RESPONSE
{
	// 2 BYTES
	struct st_I2C_TYPE_PROC_RESPONSE structTableVal;

	// 58 BYTES
	BYTE v_Reserved[58];
};

struct st_FRAME_I2C_TYPE_TRASM_REQUEST
{
	// 57 BYTES
	struct st_I2C_TYPE_TRASM_REQUEST structTableVal;

	// 3 BYTES
	BYTE v_Reserved[3];
};

struct st_FRAME_I2C_TYPE_TRASM_RESPONSE
{
	// 2 BYTES
	struct st_I2C_TYPE_TRASM_RESPONSE structTableVal;

	// 58 BYTES
	BYTE v_Reserved[58];
};


// union 
// specific, body type
typedef union
{
	// 60 BYTE
	BYTE v_CommonData[60];

	// Tables
	struct st_FRAME_TABLE_BOARD_SERVICE structVal_TABLE_BOARD_SERVICE;
	struct st_FRAME_TABLE_BOARD_PORTS structVal_TABLE_BOARD_PORTS;
	struct st_FRAME_TABLE_Power_Source structVal_TABLE_Power_Source;

	// Set Tables
	struct st_FRAME_SET_TABLE_Power_Source structVal_SET_TABLE_Power_Source;
	struct st_FRAME_SET_TABLE_RESPONSE structVal_SET_TABLE_RESPONSE;

	// i2c
	struct st_FRAME_I2C_TYPE_PROC_REQUEST structVal_I2C_TYPE_PROC_REQUEST;
	struct st_FRAME_I2C_TYPE_PROC_RESPONSE structVal_I2C_TYPE_PROC_RESPONSE;
	struct st_FRAME_I2C_TYPE_TRASM_REQUEST structVal_I2C_TYPE_TRASM_REQUEST;
	struct st_FRAME_I2C_TYPE_TRASM_RESPONSE structVal_I2C_TYPE_TRASM_RESPONSE;

} un_FRAME_BODY;


// struct common type
struct st_FRAME_COMMON
{
	// 4 BYTE
	struct st_FrameHeader structHeaderVal;

	// 60 BYTE
	un_FRAME_BODY unionBodyVal;
};

// union common type
typedef union
{
	channelFrame frameVal;

	struct st_FRAME_COMMON structVal;

} un_FRAME_COMMON;



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
	BYTE Connect(DWORD dwComNum);
	BYTE Disconnect();
	BYTE Reconnect();

	// cmd: simple
	BYTE Send(channelFrame frInput);
	BYTE SendRequest(UCHAR ucRequestType, UCHAR ucParameter);
	BYTE Get(channelFrame * frOutput);

	// cmd: combined
	BYTE ShowTable(UCHAR ucTableNum, channelFrame * frTableOutput);
	BYTE SetTable(BYTE ucTableNum, un_FRAME_COMMON frTableInput, BYTE * ucOp_status);

	// I2C
	BYTE I2C_Read_Init_Request(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * ucErrCode);
	BYTE I2C_Read_Init_Response(BYTE * ucStatus, BYTE * ucErrCode);
	BYTE I2C_Read_Trasm_Request(BYTE ucBlockNumber);
	BYTE I2C_Read_Trasm_Response(BYTE * v_Data, BYTE * ucBlockNumber);

	BYTE I2C_Write_Init_Request(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * ucErrCode);
	BYTE I2C_Write_Init_Response(BYTE * ucStatus, BYTE * ucErrCode);
	BYTE I2C_Write_Trasm_Request(BYTE * v_Data, BYTE ucBlockNumber);
	BYTE I2C_Write_Trasm_Response(BYTE * ucStatus, BYTE * ucErrCode);

	BYTE I2C_Read_Proc(BYTE * v_Data);

};

