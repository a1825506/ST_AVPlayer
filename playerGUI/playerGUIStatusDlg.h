

#include "resource.h"
#pragma once

class CplayerGUIStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CplayerGUIStatusDlg)

public:
	CplayerGUIStatusDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CplayerGUIStatusDlg();

	// 对话框数据
	enum { IDD = IDD_STATUSDIALOG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	

	DECLARE_MESSAGE_MAP()
};

