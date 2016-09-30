/**
 */

#pragma once
#include <string>
#include "vlc/vlc.h"
#include "playerGUIStatusDlg.h"
using namespace std;
#define MAX_URL_LENGTH 500

enum PlayerState{
	STATE_PREPARE,
	STATE_PLAY,
	STATE_PAUSE,
	
};
enum StartState{
	STATE_START,
	STATE_STOP
};
// CplayerGUIDlg 对话框
class CplayerGUIDlg : public CDialogEx
{
// 构造
public:
	CplayerGUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLAYERGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	libvlc_instance_t * libvlc_inst;
	libvlc_media_player_t *libvlc_mp;
	libvlc_media_t *libvlc_m;
	PlayerState playerstate;
	StartState startstate;


	
	afx_msg void OnBnClickedSaveTime();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedDownload();
	afx_msg void OnBnClickedShowInfo();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedChanegSSID();
	afx_msg void OnBnClickedFilebrowse();
	afx_msg void OnBnClickedHideSSID();
	afx_msg void OnBnClickedShowSSID();
	CEdit m_curtime;
	CEdit m_duration;
	CSliderCtrl m_progress;
	CEdit m_url;
	CEdit m_savetime;
	CEdit m_SDCard;
	CEdit m_SSIDName;
	CEdit m_SSIDPwd;
	CButton btn_positive;
	CButton btn2_native;
	CplayerGUIStatusDlg *statusdlg;
	int sdcard;
	int sdcardcap;
	int sdcardfree ;
	int recording;
	CString str;CString str1;CString str2;
	CString infoStatus;
	unsigned video_w,video_h;
	float framerate;
	int channel;
	CString infostr;
	string s1;
	int tns, thh, tmm, tss;
	bool flag;

	void SystemClear();
	void SetBtn(StartState state);
	void CreateSubWindow();
	void ShowMessageBox();
	void urlopen(string url);
	void Record(string url);
	string getTime();
	void SetTime();
	int GetSystemBits();
	void SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo);

	void UNICODE_to_UTF8(CStringW& unicodeString, std::string& str);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedInfo();
};
