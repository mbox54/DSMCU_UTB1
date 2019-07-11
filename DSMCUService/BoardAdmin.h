// interface

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "UTBDevice.h"


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define PROHIBIT__PWR_ON			0	
#define PROHIBIT__PWR_OFF			1	

#define ENABLE__PWR_ON				1	
#define ENABLE__PWR_OFF				0

#define STATE_5VSTATE_ON			0	
#define STATE_5VSTATE_OFF			1


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
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	BOOL m_bCheck_V_IN_Prohibit;
	BOOL m_bCheck_1V8_Prohibit;
	BOOL m_bCheck_3V3_Prohibit;
	BOOL m_bCheck_5V_Enable;
	BOOL m_bCheck_3V3_Enable;
	BOOL m_bCheck_1V8_Enable;
	BOOL m_bCheck_5V_State;
	afx_msg void OnBnClickedButton7();
};
