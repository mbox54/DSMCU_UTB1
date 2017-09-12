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
}

CUTBDevice::CUTBDevice(CDeviceCommunInterface * pUTBDevice)
	: m_pUTBDevice(pUTBDevice)
{
}


CUTBDevice::~CUTBDevice()
{
}

int CUTBDevice::UpdateTables()
{
	return 0;
}
