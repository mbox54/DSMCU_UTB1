// interface

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "UTBDevice.h"


/////////////////////////////////////////////////////////////////////////////
// CBoardAdmin dialog
/////////////////////////////////////////////////////////////////////////////
class CBoardAdmin : public CDialog
{
	DECLARE_DYNAMIC(CBoardAdmin)

public:
	CBoardAdmin(CWnd* pParent = NULL);   // standard constructor
	CBoardAdmin(CUTBDevice * pUTBDevice, CWnd* pParent = NULL);

	virtual ~CBoardAdmin();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_BOARD_ADMIN };
#endif

protected:

	// USB_connection for device
	CUTBDevice * m_pUTBDevice;


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
