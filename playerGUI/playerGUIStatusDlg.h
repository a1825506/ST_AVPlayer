

#include "resource.h"
#pragma once

class CplayerGUIStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CplayerGUIStatusDlg)

public:
	CplayerGUIStatusDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CplayerGUIStatusDlg();

	// �Ի�������
	enum { IDD = IDD_STATUSDIALOG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	

	DECLARE_MESSAGE_MAP()
};

