// BoardAdmin.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "resource.h"

#include "BoardAdmin.h"


// CBoardAdmin dialog

IMPLEMENT_DYNAMIC(CBoardAdmin, CDialog)

CBoardAdmin::CBoardAdmin(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_BOARD_ADMIN, pParent)
	, m_bCheck_V_IN_Prohibit(FALSE)
	, m_bCheck_1V8_Prohibit(FALSE)
	, m_bCheck_3V3_Prohibit(FALSE)
	, m_bCheck_5V_Enable(FALSE)
	, m_bCheck_3V3_Enable(FALSE)
	, m_bCheck_1V8_Enable(FALSE)
	, m_bCheck_5V_State(FALSE)
{

}

CBoardAdmin::CBoardAdmin(CUTBDevice * pUTBDevice, CWnd * pParent)
	: CDialog(IDD_PROPPAGE_BOARD_ADMIN, pParent)
	, m_pUTBDevice(pUTBDevice)
{

}

CBoardAdmin::~CBoardAdmin()
{
}

void CBoardAdmin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PWR_V_IN_PROH, m_bCheck_V_IN_Prohibit);
	DDX_Check(pDX, IDC_CHECK_PWR_1V8_PROH, m_bCheck_1V8_Prohibit);
	DDX_Check(pDX, IDC_CHECK_PWR_3V3_PROH, m_bCheck_3V3_Prohibit);
	DDX_Check(pDX, IDC_CHECK_PWR_5V_ENABLE, m_bCheck_5V_Enable);
	DDX_Check(pDX, IDC_CHECK_PWR_ENABLE_3V3, m_bCheck_3V3_Enable);
	DDX_Check(pDX, IDC_CHECK_PWR_ENABLE_1V8, m_bCheck_1V8_Enable);
	DDX_Check(pDX, IDC_CHECK_PWR_ENABLE_5V_STATE, m_bCheck_5V_State);
}


BEGIN_MESSAGE_MAP(CBoardAdmin, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CBoardAdmin::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CBoardAdmin::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CBoardAdmin::OnBnClickedButton7)
END_MESSAGE_MAP()


// CBoardAdmin message handlers

// Get Values
void CBoardAdmin::OnBnClickedButton4()
{
	// > Disable Monitor
	m_pUTBDevice->DisableMonitoring();

	un_FRAME_COMMON frFrameTmp;	
	m_pUTBDevice->ShowTable(TABLE_POWER_SOURCE, &frFrameTmp.frameVal);

	// > Set Values to output controls
	
	m_bCheck_V_IN_Prohibit = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.bV_IN_Proh;

	m_bCheck_1V8_Prohibit = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b1V8_Proh;
	m_bCheck_3V3_Prohibit = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b3V3_Proh;

	m_bCheck_5V_State = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b5V_State;

	m_bCheck_5V_Enable = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b5V_Enable;
	m_bCheck_3V3_Enable = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b3V3_Enable;
	m_bCheck_1V8_Enable = frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b1V8_Enable;

	// update ddx
	UpdateData(FALSE);

	// > Enable Monitor
	m_pUTBDevice->EnableMonitoring();

}

// Set Values
void CBoardAdmin::OnBnClickedButton6()
{
	// > Disable Monitor
	m_pUTBDevice->DisableMonitoring();


	// > Get Valus from controls
	// update ddx
	UpdateData(TRUE);

	un_FRAME_COMMON frFrameTmp;
	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.bV_IN_Proh = m_bCheck_V_IN_Prohibit;

	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b1V8_Proh = m_bCheck_1V8_Prohibit;
	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b3V3_Proh = m_bCheck_3V3_Prohibit;

	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b5V_State = m_bCheck_5V_State;

	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b5V_Enable = m_bCheck_5V_Enable;
	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b3V3_Enable = m_bCheck_3V3_Enable;
	frFrameTmp.structVal.unionBodyVal.structVal_TABLE_Power_Source.structTableVal.b1V8_Enable = m_bCheck_1V8_Enable;

	// > Set Values
	m_pUTBDevice->SetTable(SET_POWER_SOURCE, frFrameTmp);

	// > Enable Monitor
	m_pUTBDevice->EnableMonitoring();

}


void CBoardAdmin::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_bCheck_V_IN_Prohibit = 1;
	UpdateData(FALSE);
}
