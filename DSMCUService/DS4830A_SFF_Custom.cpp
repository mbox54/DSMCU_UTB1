// DS4830A_SFF_Custom.cpp : implementation file
//

#include "stdafx.h"
#include "DSMCUService.h"
#include "DS4830A_SFF_Custom.h"
#include "afxdialogex.h"

// ##########################################################################
// CDS4830A_SFF_Custom dialog
// ##########################################################################
IMPLEMENT_DYNAMIC(CDS4830A_SFF_Custom, CDialog)

CDS4830A_SFF_Custom::CDS4830A_SFF_Custom(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_CUSTOM, pParent)
	, m_SLA_ADDR(_T(""))
	, m_TABL_ADDR(_T(""))
	, m_TABL_NAME(_T(""))
	, m_sEdit_PasValue(_T(""))
	, m_sEdit_PasAddr(_T(""))
	, m_bCheck_PasEnable(FALSE)
	, m_bCheck_SelRange(FALSE)
	, m_SelAddr(_T(""))
	, m_SelCount(_T(""))
	, m_bCheck_TabSelect(FALSE)
	, m_iRadio_Mode(0)
	, m_strEdit_ByteMode(_T(""))
	, m_strEdit_PacketDelay(_T(""))
	, m_strEdit_SlaAddrPass(_T(""))
	, m_strEdit_SlaAddrTable(_T(""))
{

}

CDS4830A_SFF_Custom::CDS4830A_SFF_Custom(CUTBDevice* pUTBDevice, CProgressCtrl * p_cPB_OP, CEdit * p_EDIT_STATUS, st_serviceData * p_service, CWnd * pParent)
	: CDialog(IDD_PROPPAGE_DS4830A_SFPP_CUSTOM, pParent)
	, m_pUTBDevice(pUTBDevice)
	, p_EDIT_STATUS(p_EDIT_STATUS)
	, p_cPB_OP(p_cPB_OP)
	, p_service(p_service)
	, m_Grid(m_pHidSmbus, p_cPB_OP, p_EDIT_STATUS, p_service)

{
}

CDS4830A_SFF_Custom::~CDS4830A_SFF_Custom()
{
}

void CDS4830A_SFF_Custom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TRACE, m_TraceWnd);
	DDX_Text(pDX, IDC_EDIT_SLAADDR, m_SLA_ADDR);
	DDX_Text(pDX, IDC_EDIT_TABLADDR, m_TABL_ADDR);
	DDX_Text(pDX, IDC_EDIT_TABLNAME, m_TABL_NAME);
	//  DDX_Text(pDX, IDC_EDIT_PASADDR, m_Edit_PasAddr);
	//  DDX_Control(pDX, IDC_EDIT_PASVALUE, m_Edit_PasValue);
	DDX_Text(pDX, IDC_EDIT_PASVALUE, m_sEdit_PasValue);
	//  DDX_Control(pDX, IDC_EDIT_PASADDR, m_sEdit_PasAddr);
	DDX_Text(pDX, IDC_EDIT_PASADDR, m_sEdit_PasAddr);
	DDX_Check(pDX, IDC_CHECK_PASENABLE, m_bCheck_PasEnable);
	DDX_Control(pDX, IDC_EDIT_SLAADDR, m_Edit_SlaveAddr);
	DDX_Control(pDX, IDC_EDIT_TABLADDR, m_Edit_TableAddr);
	DDX_Control(pDX, IDC_EDIT_TABLNAME, m_Edit_TableName);
	DDX_Control(pDX, IDC_EDIT_PASADDR, m_Edit_PasAddr);
	DDX_Control(pDX, IDC_EDIT_PASVALUE, m_Edit_PasValue);
	DDX_Check(pDX, IDC_CHECK_SELENABLE, m_bCheck_SelRange);
	DDX_Text(pDX, IDC_EDIT_SELADDR, m_SelAddr);
	//  DDV_MaxChars(pDX, m_SelAddr, 2);
	//  DDX_Control(pDX, IDC_EDIT_SELCNT, m);
	DDX_Text(pDX, IDC_EDIT_SELCNT, m_SelCount);
	DDV_MaxChars(pDX, m_SelCount, 3);
	DDX_Check(pDX, IDC_CHECK_TABENABLE, m_bCheck_TabSelect);
	DDX_Radio(pDX, IDC_RADIO_MODE, m_iRadio_Mode);
	DDX_Text(pDX, IDC_EDIT_BYTEMODE, m_strEdit_ByteMode);
	DDX_Text(pDX, IDC_EDIT_PACKETDELAY, m_strEdit_PacketDelay);
	DDX_Text(pDX, IDC_EDIT_SLAADDRPASS, m_strEdit_SlaAddrPass);
	DDX_Text(pDX, IDC_EDIT_SLAADDRTABL, m_strEdit_SlaAddrTable);
}


// -------------------------------------------------------------------
// Supporting procedures
// -------------------------------------------------------------------
void CDS4830A_SFF_Custom::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	m_TraceWnd.GetWindowText(strWndText);
	strWndText += str;
	m_TraceWnd.SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	m_TraceWnd.LineScroll(-m_TraceWnd.GetLineCount());
	m_TraceWnd.LineScroll(m_TraceWnd.GetLineCount() - 4);
}


void CDS4830A_SFF_Custom::DeviceProc(BYTE ucOpType)
{
	UpdateData(TRUE);

	BYTE ucSlaAddr = (BYTE)_tcstoul(m_SLA_ADDR, NULL, 16);
	// complience for Cypress I2C header byte
	ucSlaAddr = ucSlaAddr >> 1;

	WORD usPacketDelay = (WORD)_tcstoul(m_strEdit_PacketDelay, NULL, 16);

	BYTE ucByteAddr = 0;
	WORD usByteCount = 0;

	// [MODE: SLAVE PAGE STANDARD]
	if (m_iRadio_Mode == 0)
	{
		// Set profile
		ucByteAddr = 0;
		usByteCount = 128;

		Trace(_T("–≈∆»Ã: ¬ÂıÌˇˇ ÒÚ‡ÌËˆ‡.\n"));
	}
	// [MODE: SLAVE PAGE ADDITION]
	else if (m_iRadio_Mode == 1)
	{
		// Set profile
		ucByteAddr = 128;
		usByteCount = 128;

		Trace(_T("–ÂÊËÏ: ÌËÊÌˇˇ ÒÚ‡ÌËˆ‡.\n"));
	}
	// [MODE: SLAVE CUSTOM RANGE]
	else if (m_iRadio_Mode == 2)
	{
		// Set profile from controls
		ucByteAddr = (BYTE)_tcstoul(m_SelAddr, NULL, 16);
		usByteCount = (WORD)_tcstoul(m_SelCount, NULL, 10);

		Trace(_T("–≈∆»Ã: ÔÓËÁ‚ÓÎ¸Ì‡ˇ Ó·Î‡ÒÚ¸.\n"));

		// SafeCheck
		if (ucByteAddr > 0xFF)
		{
			// [CORRECTION]

			ucByteAddr = 0xFF;

			m_SelAddr.Truncate(0);
			m_SelAddr.Append(_T("FF"));

			UpdateData(FALSE);

			Trace(_T("¬Õ»Ã¿Õ»≈! ¿‰ÂÒ ·‡ÈÚ‡ ‚ÌÂ ‰Ë‡Ô‡ÁÓÌ‡.\n"));
			Trace(_T("¿¬“Œ Œ––≈ ÷»ﬂ œ–»Ã≈Õ≈Õ¿.\n"));
		}

		if (ucByteAddr + usByteCount > 256)
		{
			// [CORRECTING]

			usByteCount = 256 - ucByteAddr;

			m_SelAddr.Truncate(0);
			m_SelAddr.AppendFormat(_T("%d"), usByteCount);
			UpdateData(FALSE);

			Trace(_T("¬Õ»Ã¿Õ»≈! –‡ÁÏÂ Ó·Î‡ÒÚË ‚ÌÂ ‰Ë‡Ô‡ÁÓÌ‡.\n"));
			Trace(_T("¿¬“Œ Œ––≈ ÷»ﬂ œ–»Ã≈Õ≈Õ¿.\n"));
		}
		
	}

	// check Page password Option
	if (this->m_bCheck_PasEnable)
	{
		Trace(_T("¬¬Œƒ œ¿–ŒÀﬂ.\n"));

		BYTE ucPassAddr = (BYTE)_tcstoul(m_sEdit_PasAddr, NULL, 16);
		BYTE ucPassValue = (BYTE)_tcstoul(m_SLA_ADDR, NULL, 16);
		
		BYTE ucSlaAddrPass = (BYTE)_tcstoul(m_strEdit_SlaAddrPass, NULL, 16);
		// complience for Cypress I2C header byte
		ucSlaAddrPass = ucSlaAddrPass >> 1;

		// get Password Value
		CString strHex;
		BYTE v_PassSymbols[4];
		for (BYTE k = 0; k < 4; k++)
		{
			char cPassLetter[2];
			cPassLetter[0] = m_sEdit_PasValue[k * 2];
			cPassLetter[1] = m_sEdit_PasValue[k * 2 + 1];

			strHex.AppendChar(cPassLetter[0]);
			strHex.AppendChar(cPassLetter[1]);

			// convert to Byte
			unsigned char byte_passLetter;
			byte_passLetter = (BYTE)_tcstoul(strHex, NULL, 16);

			v_PassSymbols[k] = byte_passLetter;

			strHex.Truncate(0);
		}

		// > Send password		
		BYTE ucErrCode = 0;
		BYTE iResult = 0;

		BYTE v_ValueBuf[5];
		v_ValueBuf[0] = ucPassAddr;
		memcpy(v_ValueBuf + 1, v_PassSymbols, 4);

		// write pass in Device
		iResult = m_pUTBDevice->I2C_Write(ucSlaAddrPass, 5, I2C_MODE_NORMAL, v_ValueBuf, &ucErrCode);

		// set Delay
		Sleep(usPacketDelay);
	}

	// check Page select Option
	if (m_bCheck_TabSelect)
	{
		Trace(_T("¬¬Œƒ “¿¡À»÷€.\n"));

		// get Page Value from controls
		BYTE uTablAddr = (BYTE)_tcstoul(m_TABL_ADDR, NULL, 16);
		BYTE uTablName = (BYTE)_tcstoul(m_TABL_NAME, NULL, 16);

		BYTE ucSlaAddrTable = (BYTE)_tcstoul(m_strEdit_SlaAddrTable, NULL, 16);
		// complience for Cypress I2C header byte
		ucSlaAddrTable = ucSlaAddrTable >> 1;

		// > Send Table
		BYTE ucErrCode = 0;
		BYTE iResult = 0;

		BYTE v_ValueBuf[2];
		v_ValueBuf[0] = uTablAddr;
		v_ValueBuf[1] = uTablName;

		// write tabl in Device
		iResult = m_pUTBDevice->I2C_Write(ucSlaAddrTable, 2, I2C_MODE_NORMAL, v_ValueBuf, &ucErrCode);

		// set Delay
		Sleep(usPacketDelay);
	}

	p_cPB_OP->SetPos(0);

	// Device Proc
	if (ucOpType == 0)
	{
		// [WRITE]

		Trace(_T("Œœ≈–¿÷»ﬂ: «¿œ»—‹.\n"));

		BYTE ucErrCode = 0;
		BYTE iResult = 0;

		// temp buffer for OP
		BYTE v_Values_Grid[256];
		BYTE v_Values[256];

		// Do Write Op
		Trace(_T("[ÓÚÔ‡‚Í‡ ·‡ÈÚÓ‚ %d].\n"), usByteCount);
		iResult = m_pUTBDevice->I2C_Write(ucSlaAddr, usByteCount + 1, I2C_MODE_NORMAL, v_Values, &ucErrCode);

		// # define number of packets, proc every packet
		WORD usPacketSize = (WORD)_tcstoul(m_strEdit_ByteMode, NULL, 10);
		WORD usRoundCount = usByteCount / usPacketSize;
		WORD usLastPacketSize = usByteCount - usPacketSize * usRoundCount;
		WORD usRoundAddr = ucByteAddr;
		BYTE v_RoundBuf[256 + 1];

		// rounds
		for (WORD k = 0; k < usRoundCount; k++)
		{			
			Trace(_T("[‚‚Ó‰ ‡‰ÂÒ‡ ·‡ÈÚ‡ %02d].\n"), usRoundAddr);

			// Prepare Values to write
			m_Grid.GridSFF_Read(v_Values, usRoundAddr, usPacketSize);
			memcpy(v_RoundBuf + 1, v_Values, usPacketSize);
			
			// Set Byte Addr
			v_RoundBuf[0] = usRoundAddr;

			// Do Write Op
			Trace(_T("[ÓÚÔ‡‚Í‡ ·‡ÈÚÓ‚ %d].\n"), usPacketSize);

			iResult = m_pUTBDevice->I2C_Write(ucSlaAddr, usPacketSize + 1, I2C_MODE_NORMAL, v_RoundBuf, &ucErrCode);

			if (iResult != I2C_OP_SUCCESS)
			{
				// [ERROR]

				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), iResult);
				return;
			}

			// set next start byte addr
			usRoundAddr += usPacketSize;

			// set Delay
			Sleep(usPacketDelay);

			// update output controls
			p_cPB_OP->SetPos(96 / usRoundCount * k);
		}//rounds

		// last packet
		if (usLastPacketSize > 0)
		{
			Trace(_T("[‚‚Ó‰ ‡‰ÂÒ‡ ·‡ÈÚ‡ %02d].\n"), usRoundAddr);

			// Prepare Values to write
			m_Grid.GridSFF_Read(v_Values, usRoundAddr, usLastPacketSize);
			memcpy(v_RoundBuf + 1, v_Values, usLastPacketSize);

			// Set Byte Addr
			v_RoundBuf[0] = usRoundAddr;

			// Do Write Op
			Trace(_T("[ÓÚÔ‡‚Í‡ ·‡ÈÚÓ‚ %d].\n"), usLastPacketSize);

			iResult = m_pUTBDevice->I2C_Write(ucSlaAddr, usLastPacketSize + 1, I2C_MODE_NORMAL, v_RoundBuf, &ucErrCode);

			if (iResult != I2C_OP_SUCCESS)
			{
				// [ERROR]

				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), iResult);
				return;
			}

			// set Delay
			Sleep(usPacketDelay);
		}
	}
	else if (ucOpType == 1)
	{
		// [READ]

		Trace(_T("Œœ≈–¿÷»ﬂ: ◊“≈Õ»≈.\n"));

		BYTE ucErrCode = 0;
		BYTE iResult = 0;

		// temp buffer for OP
		BYTE v_Values[256];


		// # define number of packets, proc every packet
		WORD usPacketSize = (WORD)_tcstoul(m_strEdit_ByteMode, NULL, 10); 
		WORD usRoundCount = usByteCount / usPacketSize;
		WORD usLastPacketSize = usByteCount - usPacketSize * usRoundCount;
		WORD usRoundAddr = ucByteAddr;
		BYTE v_RoundBuf[256];
		WORD usDataShift = 0;

		// rounds
		for (WORD k = 0; k < usRoundCount; k++)
		{
			// Set Byte Addr
			Trace(_T("[‚‚Ó‰ ‡‰ÂÒ‡ ·‡ÈÚ‡ %02d].\n"), usRoundAddr);

			v_RoundBuf[0] = usRoundAddr;
			iResult = m_pUTBDevice->I2C_Write(ucSlaAddr, 1, I2C_MODE_NORMAL, v_RoundBuf, &ucErrCode);

			if (iResult != I2C_OP_SUCCESS)
			{
				// [ERROR]

				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), iResult);
				return;
			}

			// Do Read Op
			Trace(_T("[ÔÓÎÛ˜ÂÌËÂ ·‡ÈÚÓ‚ %d].\n"), usPacketSize);

			iResult = m_pUTBDevice->I2C_Read(ucSlaAddr, usPacketSize, I2C_MODE_NORMAL, v_RoundBuf, &ucErrCode);

			if (iResult != I2C_OP_SUCCESS)
			{
				// [ERROR]

				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), iResult);
				return;
			}

			// place to memory
			memcpy(v_Values + usDataShift, v_RoundBuf, usPacketSize);

			// set next start byte addr
			usRoundAddr += usPacketSize;
			usDataShift += usPacketSize;

			// set Delay
			Sleep(usPacketDelay);

			// update output controls
			p_cPB_OP->SetPos(96 / usRoundCount * k);
		}//rounds

		// last packet
		if (usLastPacketSize > 0)
		{
			// Set Byte Addr
			Trace(_T("[‚‚Ó‰ ‡‰ÂÒ‡ ·‡ÈÚ‡ %02d].\n"), usRoundAddr);

			v_RoundBuf[0] = usRoundAddr;
			iResult = m_pUTBDevice->I2C_Write(ucSlaAddr, 1, I2C_MODE_NORMAL, v_Values, &ucErrCode);

			if (iResult != I2C_OP_SUCCESS)
			{
				// [ERROR]

				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), iResult);
				return;
			}

			// Do Read Op
			Trace(_T("[ÔÓÎÛ˜ÂÌËÂ ·‡ÈÚÓ‚ %d].\n"), usLastPacketSize);

			iResult = m_pUTBDevice->I2C_Read(ucSlaAddr, usLastPacketSize, I2C_MODE_NORMAL, v_RoundBuf, &ucErrCode);

			if (iResult != I2C_OP_SUCCESS)
			{
				// [ERROR]

				Trace(_T("Œÿ»¡ ¿. [ÍÓ‰: %02d] \n"), iResult);
				return;
			}

			// place to memory
			memcpy(v_Values + usDataShift, v_RoundBuf, usLastPacketSize);

			// set Delay
			Sleep(usPacketDelay);
		}
		
		// > Output to controls
		// clear output Table
		m_Grid.ClearTable();

		// set output Table
		m_Grid.GridSFF_Write(v_Values, ucByteAddr, usByteCount);

	}//if (ucOpType == 1)

	p_cPB_OP->SetPos(100);

	UpdateData(FALSE);

	Trace(_T("”—œ≈ÿÕŒ.\n\n"));	
}


void CDS4830A_SFF_Custom::EditHexControl(CEdit * pEdit)
{
	CString str;

	pEdit->GetWindowTextW(str);

	// valid symbol checking
	CString hexTextValid = str.SpanIncluding(_T("0123456789abcdefABCDEF"));

	// check for invalid hex symbol
	if (hexTextValid.Compare(str) != 0)
	{
		// clear control Text
		pEdit->SetWindowTextW(_T(""));
	}

}

void CDS4830A_SFF_Custom::EditInit()
{
	// init EditBoxes

	// set default text values
	m_SLA_ADDR = (CString)"A2";
	m_TABL_ADDR = (CString)"7F";
	m_TABL_NAME = (CString)"00";

	m_strEdit_ByteMode = (CString)"8";
	m_strEdit_PacketDelay = (CString)"0";

	m_strEdit_SlaAddrTable = (CString)"A2";

	m_sEdit_PasAddr = (CString)"7B";
	m_sEdit_PasValue = (CString)"4F505759";

	// set Edit control parameter
	m_Edit_SlaveAddr.SetLimitText(2);
	m_Edit_TableAddr.SetLimitText(2);
	m_Edit_TableName.SetLimitText(2);

	m_Edit_PasAddr.SetLimitText(2);

}

void CDS4830A_SFF_Custom::OnGridClick(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	Trace(_T("Text: %s\n"), m_Grid.GetItemText(pItem->iRow, pItem->iColumn));
	//Item.strText
}

void CDS4830A_SFF_Custom::OnGridEndEdit(NMHDR * pNotifyStruct, LRESULT * pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	// > check Valid Input
	bool bInvalid = m_Grid.CheckValidHex(pItem->iRow, pItem->iColumn);

	*pResult = (bInvalid || m_Grid.m_bRejectEditChanges) ? -1 : 0;

	Trace(_T("Res: %d\n"), bInvalid && m_Grid.m_bRejectEditChanges);
}

// -------------------------------------------------------------------
// MESSAGE_MAP
// -------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDS4830A_SFF_Custom, CDialog)
	// Grid event functions
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)

	ON_BN_CLICKED(IDC_BUTTON1, &CDS4830A_SFF_Custom::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDS4830A_SFF_Custom::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDS4830A_SFF_Custom::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDS4830A_SFF_Custom::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDS4830A_SFF_Custom::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT_SLAADDR, &CDS4830A_SFF_Custom::OnEnChangeEditSlaaddr)
	ON_EN_CHANGE(IDC_EDIT_TABLADDR, &CDS4830A_SFF_Custom::OnEnChangeEditTabladdr)
	ON_EN_CHANGE(IDC_EDIT_TABLNAME, &CDS4830A_SFF_Custom::OnEnChangeEditTablname)
	ON_EN_CHANGE(IDC_EDIT_PASADDR, &CDS4830A_SFF_Custom::OnEnChangeEditPasaddr)
	ON_EN_CHANGE(IDC_EDIT_PASVALUE, &CDS4830A_SFF_Custom::OnEnChangeEditPasvalue)
END_MESSAGE_MAP()

// -------------------------------------------------------------------
// CDS4830A_SFF_Custom message handlers
// -------------------------------------------------------------------
BOOL CDS4830A_SFF_Custom::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Grid.Init();

	// init default Device options
	this->EditInit();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// -------------------------------------------------------------------
// Buttons Test
// -------------------------------------------------------------------
void CDS4830A_SFF_Custom::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	BYTE uValues[256];

	// fill with probe
	for (int k = 0; k < 256; k++)
	{
		uValues[k] = k;
	}

	//	UpdateData(TRUE);
	m_Grid.GridSFF_Write(uValues, 0, 256);

	UpdateData(FALSE);
}


void CDS4830A_SFF_Custom::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	m_Grid.GridSFF_Read(uValues2, 0, 256);
}


void CDS4830A_SFF_Custom::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_Grid.GridSFF_Write(uValues2, 0, 256);
}

// -------------------------------------------------------------------
// Table op call events
// -------------------------------------------------------------------

// Read Table Procedure
void CDS4830A_SFF_Custom::OnBnClickedButton4()
{
	// call Read Op
	DeviceProc(1);

}

// Write Table Procedure
void CDS4830A_SFF_Custom::OnBnClickedButton5()
{
	// call Write Op
	DeviceProc(0);

}





void CDS4830A_SFF_Custom::OnEnChangeEditSlaaddr()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	this->EditHexControl(&m_Edit_SlaveAddr);
}


void CDS4830A_SFF_Custom::OnEnChangeEditTabladdr()
{
	this->EditHexControl(&m_Edit_TableAddr);		
}


void CDS4830A_SFF_Custom::OnEnChangeEditTablname()
{
	this->EditHexControl(&m_Edit_TableName);
}


void CDS4830A_SFF_Custom::OnEnChangeEditPasaddr()
{
	this->EditHexControl(&m_Edit_PasAddr);
}


void CDS4830A_SFF_Custom::OnEnChangeEditPasvalue()
{
	this->EditHexControl(&m_Edit_PasValue);
}


void CDS4830A_SFF_Custom::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}
