// UTBDevice.cpp : implementation file

// implementation


/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "UTBDevice.h"



// ##########################################################################
// CUTBDevice
// ##########################################################################
CUTBDevice::CUTBDevice()
{
	this->InitDeviceQueue();

	EnableMonitoring();
}

// active Constructor
CUTBDevice::CUTBDevice(CDeviceCommunInterface * pUTBDeviceCommun)
	: m_pUTBDevice(pUTBDeviceCommun)
{
	this->InitDeviceQueue();

	EnableMonitoring();
}


CUTBDevice::~CUTBDevice()
{
}

// Init starting Request Table Queue
void CUTBDevice::InitDeviceQueue()
{
	// > Init Basic Monitoring Options

	//m_frame1[10] = 10;

	// > Set Mon Default Tables
	BYTE ucCount = 0;

	// TABLE 1:1 / Board_Service
	m_DeviceQueue.v_OID_SHOW[ucCount].ucTableNum = 0x01;
	m_DeviceQueue.v_OID_SHOW[ucCount].ucCounterDivider = 1;
	m_DeviceQueue.v_OID_SHOW[ucCount].ucCounterValue = m_DeviceQueue.v_OID_SHOW[ucCount].ucCounterDivider - 1;

	ucCount++;
	m_DeviceQueue.ucMsgCount = ucCount;

	// TABLE 1:2 / Board_Ports

}

void CUTBDevice::DisableMonitoring()
{
	this->m_bMonitoringEnable = 0;
}

void CUTBDevice::EnableMonitoring()
{
	this->m_bMonitoringEnable = 1;
}

BYTE CUTBDevice::ShowTable(BYTE ucTableNum, channelFrame * frTableOutput)
{
	BYTE ucResult = m_pUTBDevice->ShowTable(ucTableNum, frTableOutput);


	return ucResult;
}

BYTE CUTBDevice::SetTable(BYTE ucTableNum, un_FRAME_COMMON frTableInput)
{
	BYTE ucOp_status;
	BYTE ucResult = m_pUTBDevice->SetTable(ucTableNum, frTableInput, &ucOp_status);

	if (ucOp_status == 1)
	{
		// [STATUS: OK]
	}

	return ucResult;
}

// Monitor and Update Device Data 
int CUTBDevice::UpdateTables()
{
	// NOTE:
	// FORMAT:
	// Send 1 SHOW Request   --->
	// Get  1 SHOW Responce  <---

	// > check Enable monitoring
	if (!this->m_bMonitoringEnable)
	{
		// skipped
		return 2;
	}

	for (UCHAR k = 0; k < m_DeviceQueue.ucMsgCount; k++)
	{
		if (m_DeviceQueue.v_OID_SHOW[k].ucCounterValue == 0)
		{
			// [PROCEED]

			// Monitor and Update Device Data 
			channelFrame frFrameTmp;
			m_pUTBDevice->ShowTable(TABLE_BOARD_SERVICE, &frFrameTmp);


			// restore Counter
			m_DeviceQueue.v_OID_SHOW[k].ucCounterValue = m_DeviceQueue.v_OID_SHOW[k].ucCounterDivider - 1;
		}
		else
		{
			// [WAIT]

			m_DeviceQueue.v_OID_SHOW[k].ucCounterValue--;
		}

	}


	return 0;
}


// -------------------------------------------------------------------
// I2C FUNTIONALITY
// -------------------------------------------------------------------

// Set Initialisation Part of I2C Read routine
// NOTE:
// Address = 0 to 127
// Max Read Byte Count = 256
// Modes: 0 = Normal, 1 = ReStart, 2 = No NACK 
BYTE CUTBDevice::I2C_Read(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * v_Data, BYTE * ucErrCode)
{
	// *** PROC PART ***

	// > Send Request: Read, Define Read Params 
	m_pUTBDevice->I2C_Read_Init_Request(ucAddress, ucCount, ucMode, ucErrCode);

	// > Get Response: Status and ErrCode(if err'd)
	BYTE ucStatus = 0;
	BYTE iResult = m_pUTBDevice->I2C_Read_Init_Response(&ucStatus, ucErrCode);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}

	// Check Validity of PROC
	if (ucStatus != I2C_OP_SUCCESS)
	{
		return I2C_OP_ERROR;
	}


	// *** TRASM PART ***
	// > Send Request: Get DataBlock

	BYTE uRounds = ucCount / I2C_DATA_SIZE;
	BYTE uRemain = ucCount - uRounds * I2C_DATA_SIZE;
	BYTE iCount;
	BYTE ucBlockNumber = 0;

	BYTE ucBlockNumberGet;

	do
	{
		// > Define Read count
		if (uRounds > 0)
		{
			uRounds--;
			iCount = I2C_DATA_SIZE;
		}
		else
		{
			iCount = uRemain;
		}

		// > Get DataBlock
		// Set Request: DataBlock
		iResult = m_pUTBDevice->I2C_Read_Trasm_Request(ucBlockNumber);

		// check error
		if (iResult != DEVICE_OP_SUCCESS)
		{
			return iResult;
		}

		// Get Response: DataBlock and BlockNumber
		// NOTE: Placed to Output Buffer automatically
		iResult = m_pUTBDevice->I2C_Read_Trasm_Response(v_Data + ucBlockNumber * I2C_DATA_SIZE, &ucBlockNumberGet);

		// check error
		if (iResult != DEVICE_OP_SUCCESS)
		{
			return iResult;
		}	

		// > Increment BlockNumber
		ucBlockNumber ++;

	} while (ucBlockNumber * I2C_DATA_SIZE < ucCount);


	return I2C_OP_SUCCESS;

}


void CUTBDevice::I2C_Read_Proc(BYTE * v_Data)
{


}

// NOTE: in usual case Need 2Byte Var to send Read Bytes Count
//		 but here used a way, where Count_Parameter = Count_Source - 1,
//		 so Value = 256 can be transfered in 1Byte var as 256-1 = 255.
BYTE CUTBDevice::I2C_Write(BYTE ucAddress, WORD ucCount, BYTE ucMode, BYTE * v_Data, BYTE * ucErrCode)
{
	// > Send Request: Write, Define Write Params 
	m_pUTBDevice->I2C_Write_Init_Request(ucAddress, ucCount, ucMode, ucErrCode);

	// > Get Response: Status and ErrCode(if err'd)
	BYTE ucStatus = 0;
	BYTE iResult = m_pUTBDevice->I2C_Write_Init_Response(&ucStatus, ucErrCode);

	// check error
	if (iResult != DEVICE_OP_SUCCESS)
	{
		return iResult;
	}

	// Check Validity of PROC
	if (ucStatus != I2C_OP_SUCCESS)
	{
		return I2C_OP_ERROR;
	}

	// *** TRASM PART ***
	// > Send Request: Set DataBlock
	BYTE ucRounds = ucCount / I2C_DATA_SIZE;
	BYTE uRemain = ucCount - ucRounds * I2C_DATA_SIZE;

	BYTE v_DataBuf[I2C_DATA_SIZE];

	BYTE ucBlockNumber = 0;
	do
	{
		// > Define Write count
		if (ucRounds > 0)
		{
			ucRounds--;

			// Place data to buffer
			for (BYTE k = 0; k < I2C_DATA_SIZE; k++)
			{
				v_DataBuf[k] = v_Data[k];
			}

		}
		else
		{
			// Place data to buffer
			for (BYTE k = 0; k < uRemain; k++)
			{
				v_DataBuf[k] = v_Data[k];
			}

			// Place NULL to remain memory space
			for (BYTE k = uRemain; k < I2C_DATA_SIZE; k++)
			{
				v_DataBuf[k] = 0x00;
			}
		}

		// > Send DataBlock
		// Set Request: DataBlock
		iResult = m_pUTBDevice->I2C_Write_Trasm_Request(v_DataBuf, ucBlockNumber);

		// check error
		if (iResult != DEVICE_OP_SUCCESS)
		{
			return iResult;
		}

		// Get Response: DataBlock and BlockNumber
		// NOTE: Placed to Output Buffer automatically
		iResult = m_pUTBDevice->I2C_Write_Trasm_Response(&ucStatus, ucErrCode);

		// check error
		if (iResult != DEVICE_OP_SUCCESS)
		{
			return iResult;
		}

		// Check Validity of PROC
		if (ucStatus != I2C_OP_SUCCESS)
		{
			return I2C_OP_ERROR;
		}

		// > Increment BlockNumber
		ucBlockNumber++;

	} while (ucRounds > 0);


	return I2C_OP_SUCCESS;
}
