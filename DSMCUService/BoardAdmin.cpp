// BoardAdmin.cpp : implementation file
//

#include "stdafx.h"
#include "BoardAdmin.h"
#include "afxdialogex.h"


// CBoardAdmin dialog

IMPLEMENT_DYNAMIC(CBoardAdmin, CDialog)

CBoardAdmin::CBoardAdmin(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPPAGE_BOARD_ADMIN, pParent)
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
}


BEGIN_MESSAGE_MAP(CBoardAdmin, CDialog)
END_MESSAGE_MAP()


// CBoardAdmin message handlers
