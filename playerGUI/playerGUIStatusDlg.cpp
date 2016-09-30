#include "stdafx.h"
#include "afxdialogex.h"
#include "playerGUIStatusDlg.h"


IMPLEMENT_DYNAMIC(CplayerGUIStatusDlg, CDialogEx)

	CplayerGUIStatusDlg::CplayerGUIStatusDlg(CWnd* pParent /*=NULL*/)

	: CDialogEx(CplayerGUIStatusDlg::IDD, pParent)
{

}

CplayerGUIStatusDlg::~CplayerGUIStatusDlg()
{
}

void CplayerGUIStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CplayerGUIStatusDlg, CDialogEx)

END_MESSAGE_MAP()