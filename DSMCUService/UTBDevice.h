// UTBDevice.h : header file

// INFO:
// Class purpose is to implement UTBDevice memory and
// incorporate UTBDevice relate communication
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "DeviceCommunInterface.h" 


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
//#define TABLE_BOARD_SERVICE				0x01
//#define TABLE_BOARD_PORTS				0x02



/////////////////////////////////////////////////////////////////////////////
// Types
/////////////////////////////////////////////////////////////////////////////
// ------ REQUEST QUEUE ------ // 
struct st_OID_SHOW_PARAMS
{
	BYTE ucTableNum;
	BYTE ucCounterDivider;
	BYTE ucCounterValue;
};

struct st_COMPortMsgQue
{
	BYTE ucMsgCount;
	st_OID_SHOW_PARAMS v_OID_SHOW[32];

} ;


// ##########################################################################
// CUTBDevice
// ##########################################################################
class CUTBDevice
{
public:
	// Construction
	CUTBDevice();
	CUTBDevice(CDeviceCommunInterface * pUTBDeviceCommun);

	~CUTBDevice();

	// > Properties
	// config

	// communucation
	CDeviceCommunInterface * m_pUTBDevice;

	// define COMPortMsg Arbiter Queue
	st_COMPortMsgQue m_DeviceQueue;

	// Device Tables
	st_TABLE_BOARD_SERVICE	m_Table_Board_Service;
	st_TABLE_BOARD_PORTS	m_Table_Board_Ports;


	// > Events
	// init
	void InitDeviceQueue();

	void DisableMonitoring();
	void EnableMonitoring();

	// Board
	int UpdateTables();

	// parent fuctions
	BYTE ShowTable(BYTE ucTableNum, channelFrame * frTableOutput);
	BYTE SetTable(BYTE ucTableNum, un_FRAME_COMMON frTableInput);

	// I2C
	BYTE I2C_Read(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * v_Data, BYTE * ucErrCode);
	void I2C_Read_Proc(BYTE * v_Data);

	BYTE I2C_Write(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * v_Data, BYTE * ucErrCode);


private:
	BYTE m_bMonitoringEnable;

};

