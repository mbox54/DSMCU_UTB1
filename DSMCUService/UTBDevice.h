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
#define TABLE_BOARD_SERVICE				0x01
#define TABLE_BOARD_PORTS				0x02


// ##########################################################################
// CUTBDevice
// ##########################################################################
class CUTBDevice
{
public:
	// Construction
	CUTBDevice();
	CUTBDevice(CDeviceCommunInterface * pUTBDevice);

	~CUTBDevice();

	// > Properties
	// config

	// communucation
	CDeviceCommunInterface * m_pUTBDevice;


	// > Events
	// init

	int UpdateTables();


};

