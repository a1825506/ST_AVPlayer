/**
 *����libVLC�Ĳ�������
 *rtsp://192.168.11.123/1/h264major

 */

#include "stdafx.h"
#include <string>
#include "playerGUI.h"
#include "playerGUIDlg.h"
#include "afxdialogex.h"
#include "playerGUIStatusDlg.h"
#include <windows.h>
#include <wininet.h>
#include<time.h>
#include <stdlib.h>
#include <json/json.h>


using namespace std;
#define MAXSIZE 1024
#pragma comment(lib, "Wininet.lib") 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



// CplayerGUIDlg �Ի���




CplayerGUIDlg::CplayerGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CplayerGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CplayerGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURTIME, m_curtime);
	DDX_Control(pDX, IDC_DURATION, m_duration);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
   DDX_Control(pDX,IDC_SAVETIME,m_savetime);
   DDX_Control(pDX,IDC_SSIDName,m_SSIDName);

   DDX_Control(pDX,IDC_SSIDPwd,m_SSIDPwd);
	DDX_Control(pDX, IDC_URL, m_url);
	//  DDX_Control(pDX, IDC_VOLUME, m_volume);
}

BEGIN_MESSAGE_MAP(CplayerGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START, &CplayerGUIDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_PAUSE, &CplayerGUIDlg::OnBnClickedPause)
	ON_BN_CLICKED(ID_STOP, &CplayerGUIDlg::   OnBnClickedStop)
	ON_BN_CLICKED(ID_DOWNLOAD,&CplayerGUIDlg::OnBnClickedDownload)
	ON_BN_CLICKED(ID_SHOWInfo,&CplayerGUIDlg:: OnBnClickedShowInfo)
	ON_BN_CLICKED(ID_ABOUT, &CplayerGUIDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_FILEBROWSE, &CplayerGUIDlg::OnBnClickedFilebrowse)
	ON_BN_CLICKED(IDC_BUTTON_SAVETIEM,&CplayerGUIDlg::OnBnClickedSaveTime)
	ON_BN_CLICKED(IDC_CHANGESSID,&CplayerGUIDlg::OnBnClickedChanegSSID)
	ON_BN_CLICKED(IDC_RADIO_HIDE,&CplayerGUIDlg::OnBnClickedHideSSID)
	ON_BN_CLICKED(IDC_RADIO_SHOW,&CplayerGUIDlg::OnBnClickedShowSSID)

	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(ID_INFO, &CplayerGUIDlg::OnBnClickedInfo)
END_MESSAGE_MAP()


// CplayerGUIDlg ��Ϣ�������

BOOL CplayerGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//SetTimer(2,10000,NULL);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	CreateSubWindow();
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_progress.SetRange(0,100);
	SystemClear();
	startstate=STATE_STOP;
	const char * const vlc_args[] = {
		"--demux=h264",
		"--ipv4",
		"--no-prefer-system-codecs",
		"--rtsp-caching=300",
		"--network-caching=500",
		"--rtsp-tcp",

	};
	flag=false;
	m_savetime.SetWindowText("60~3600��֮��");
	/* Load the VLC engine */
	   libvlc_inst = libvlc_new (sizeof(vlc_args)/sizeof(vlc_args[0]), vlc_args);
		//libvlc_inst = libvlc_new (0, NULL);
	   return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CplayerGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CplayerGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CplayerGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CplayerGUIDlg::OnBnClickedStart()
{
if(startstate==STATE_STOP){
	CStringW cstr_url;

#ifdef _UNICODE
	m_url.GetWindowText(cstr_url);
#else
	USES_CONVERSION;
	CStringA cstr_urla;
	m_url.GetWindowText(cstr_urla);
	cstr_url.Format(L"%s",A2W(cstr_urla));
#endif

	std::string str_url;
	UNICODE_to_UTF8(cstr_url, str_url);
	const char *char_url=str_url.c_str();

	if(strcmp(char_url,"")==0){
		m_url.SetWindowText( "rtsp://192.168.11.123/1/h264major");
	}
	
	HWND screen_hwnd=NULL;

	screen_hwnd = this->GetDlgItem(IDC_SCREEN)->m_hWnd; 

		if(libvlc_m==NULL){
			//�������ļ���ַ
			//rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp
			//rtsp://192.168.11.123/1/h264major
			libvlc_m = libvlc_media_new_location (libvlc_inst, _T("rtsp://192.168.11.123/1/h264major"));
		}
		/* Create a media player playing environement */
		if(libvlc_mp==NULL){
			libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);
		}
		//on windows
		libvlc_media_player_set_hwnd (libvlc_mp,screen_hwnd);

		/* play the media_player */
		int x=libvlc_media_player_play (libvlc_mp);
		//ͬ������ʱ����豸
	   //  SetTime();
		if(flag){
			playerstate=STATE_PLAY;
			startstate=STATE_START;
			GetDlgItem(ID_START)->SetWindowText(_T("ֹͣ"));
			SetBtn(STATE_START);
			//��������ֵ���ǵ�һ������ֵ1����ʾ�˶�ʱ����ID�š� 
			//�ڶ���������ʾҪ�ȴ�100����ʱ�������´���һ�Ρ����������������ַ�����һ����NULL�� 
			//ע�⣺���õڶ�������ʱҪע�⣬������õĵȴ�ʱ��ȴ���ʱ��̣�����ͻ�������ˡ� 
			tss=0,tns=0, thh=0, tmm=0;
			SetTimer(1,1000,NULL);
			SetTimer(2,5000,NULL);
		}
	}else if(startstate==STATE_START){
		if(libvlc_mp!=NULL){
			libvlc_media_player_stop (libvlc_mp);
			libvlc_media_player_release (libvlc_mp);
			libvlc_mp=NULL;
			KillTimer(1);
			KillTimer(2);
		}
		SystemClear();
		startstate=STATE_STOP;
		SetBtn(STATE_STOP);
		GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
	}
	}
void CplayerGUIDlg::OnBnClickedPause()
{
	if(playerstate==STATE_PLAY){
		libvlc_media_player_set_pause(libvlc_mp,1);
		playerstate=STATE_PAUSE;
		GetDlgItem(ID_PAUSE)->SetWindowText(_T("�������"));
		KillTimer(1);
	}else if(playerstate==STATE_PAUSE){
		libvlc_media_player_set_pause(libvlc_mp,0);
		playerstate=STATE_PLAY;
		GetDlgItem(ID_PAUSE)->SetWindowText(_T("�����ͣ"));
		
		SetTimer(1,1000,NULL);
	}
}
/************************************************************************/
/*             
�޸�SSID���ƻ�����                                                      */
/************************************************************************/
void CplayerGUIDlg::OnBnClickedChanegSSID()
{
	string reboot_url= "http://192.168.11.123/api/reboot";
	CString cSSIDname,cSSIDpwd;
	m_SSIDName.GetWindowText(cSSIDname);
	m_SSIDPwd.GetWindowText(cSSIDpwd);
	if(strcmp(cSSIDname,"")==0&&strcmp(cSSIDpwd,"")==0){
		AfxMessageBox("����ͬʱΪ��");
	}else{
		if(strcmp(cSSIDname,"")!=0){
			string url = "http://192.168.11.123/api/setssid?ssid="+cSSIDname;
			Record(url);
		m_SSIDName.SetWindowText("");
		if(strcmp(cSSIDpwd,"")==0){
			AfxMessageBox("�������·����豸��������");
			{
				if(libvlc_mp!=NULL){
					libvlc_media_player_stop (libvlc_mp);
					libvlc_media_player_release (libvlc_mp);
					libvlc_mp=NULL;
					KillTimer(1);
					KillTimer(2);
				}
				SystemClear();
				startstate=STATE_STOP;
				SetBtn(STATE_STOP);
				GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
			}
			Record(reboot_url);
		}else{
			AfxMessageBox("�������·�");
		}
		}if(strcmp(cSSIDpwd,"")!=0){
			string url = "http://192.168.11.123/api/setpasswd?pswd="+cSSIDname;
			Record(url);
			AfxMessageBox("�������·�,�豸��������");
			{
				if(libvlc_mp!=NULL){
					libvlc_media_player_stop (libvlc_mp);
					libvlc_media_player_release (libvlc_mp);
					libvlc_mp=NULL;
					KillTimer(1);
					KillTimer(2);
				}
				SystemClear();
				startstate=STATE_STOP;
				SetBtn(STATE_STOP);
				GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
			}
			m_SSIDPwd.SetWindowText("");
			Record(reboot_url);
		}
	}
}
void CplayerGUIDlg::OnBnClickedStop()
{
	if(libvlc_mp!=NULL){
		libvlc_media_player_stop (libvlc_mp);
		libvlc_media_player_release (libvlc_mp);
		libvlc_mp=NULL;
		KillTimer(1);
		KillTimer(2);
	}
	SystemClear();
}

void  CplayerGUIDlg::OnBnClickedShowInfo()
{
	ShowMessageBox();
}

void CplayerGUIDlg::CreateSubWindow()
{
	statusdlg=new CplayerGUIStatusDlg;
	statusdlg->Create(IDD_STATUSDIALOG);
}

void CplayerGUIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}
/************************************************************************/
/* ����¼�񱣴�ʱ��
   http://192.168.11.123/api/trio_seg?val=
*/
/************************************************************************/
void CplayerGUIDlg::OnBnClickedSaveTime()
{

   CString mtime;

   m_savetime.GetWindowText(mtime);

   string m_time = (string)mtime;

   string settime_url="http://192.168.11.123/api/trio_seg?val="+m_time;

	 AfxMessageBox("�������·�������¼�����Ч");
	 {
		 if(libvlc_mp!=NULL){
			 libvlc_media_player_stop (libvlc_mp);
			 libvlc_media_player_release (libvlc_mp);
			 libvlc_mp=NULL;
			 KillTimer(1);
			 KillTimer(2);
		 }
		 SystemClear();
		 startstate=STATE_STOP;
		 SetBtn(STATE_STOP);
		 GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
	 }

	 Record(settime_url);
}
void CplayerGUIDlg::OnBnClickedFilebrowse()
{
	CString FilePathName;
	//Filter String
	CString strfilter;
	strfilter.Append(_T("Common media formats|*.avi;*.wmv;*.wmp;*.wm;*.asf;*.rm;*.ram;*.rmvb;*.ra;*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;"));
	strfilter.Append(_T("*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.vob;*.ac3;*.dts;*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2;*.swf;*.ogg;*.wma;*.wav;"));
	strfilter.Append(_T("*.mid;*.midi;*.mpa;*.mp2;*.mp3;*.m1a;*.m2a;*.m4a;*.aac;*.mkv;*.ogm;*.m4b;*.tp;*.ts;*.tpr;*.pva;*.pss;*.wv;*.m2ts;*.evo;"));
	strfilter.Append(_T("*.rpm;*.realpix;*.rt;*.smi;*.smil;*.scm;*.aif;*.aiff;*.aifc;*.amr;*.amv;*.au;*.acc;*.dsa;*.dsm;*.dsv;*.dss;*.pmp;*.smk;*.flic|"));
	strfilter.Append(_T("Windows Media Video(*.avi;*wmv;*wmp;*wm;*asf)|*.avi;*.wmv;*.wmp;*.wm;*.asf|"));
	strfilter.Append(_T("Windows Media Audio(*.wma;*wav;*aif;*aifc;*aiff;*mid;*midi;*rmi)|*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi|"));
	strfilter.Append(_T("Real(*.rm;*ram;*rmvb;*rpm;*ra;*rt;*rp;*smi;*smil;*.scm)|*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.rt;*.rp;*.smi;*.smil;*.scm|"));
	strfilter.Append(_T("MPEG Video(*.mpg;*mpeg;*mpe;*m1v;*m2v;*mpv2;*mp2v;*dat;*mp4;*m4v;*m4p;*m4b;*ts;*tp;*tpr;*pva;*pss;*.wv;)|"));
	strfilter.Append(_T("*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.m4b;*.ts;*.tp;*.tpr;*.pva;*.pss;*.wv;|"));
	strfilter.Append(_T("MPEG Audio(*.mpa;*mp2;*m1a;*m2a;*m4a;*aac;*.m2ts;*.evo)|*.mpa;*.mp2;*.m1a;*.m2a;*.m4a;*.aac;*.m2ts;*.evo|"));
	strfilter.Append(_T("DVD(*.vob;*ifo;*ac3;*dts)|*.vob;*.ifo;*.ac3;*.dts|MP3(*.mp3)|*.mp3|CD Tracks(*.cda)|*.cda|"));
	strfilter.Append(_T("Quicktime(*.mov;*qt;*mr;*3gp;*3gpp;*3g2;*3gp2)|*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2|"));
	strfilter.Append(_T("Flash Files(*.flv;*swf;*.f4v)|*.flv;*.swf;*.f4v|Playlist(*.smpl;*.asx;*m3u;*pls;*wvx;*wax;*wmx;*mpcpl)|*.smpl;*.asx;*.m3u;*.pls;*.wvx;*.wax;*.wmx;*.mpcpl|"));
	strfilter.Append(_T("Others(*.ivf;*au;*snd;*ogm;*ogg;*fli;*flc;*flic;*d2v;*mkv;*pmp;*mka;*smk;*bik;*ratdvd;*roq;*drc;*dsm;*dsv;*dsa;*dss;*mpc;*divx;*vp6;*.ape;*.flac;*.tta;*.csf)"));
	strfilter.Append(_T("|*.ivf;*.au;*.snd;*.ogm;*.ogg;*.fli;*.flc;*.flic;*.d2v;*.mkv;*.pmp;*.mka;*.smk;*.bik;*.ratdvd;*.roq;*.drc;*.dsm;*.dsv;*.dsa;*.dss;*.mpc;*.divx;*.vp6;*.ape;*.amr;*.flac;*.tta;*.csf|"));
	strfilter.Append(_T("All Files(*.*)|*.*||"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,lpszfilter);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName();
		m_url.SetWindowText(FilePathName);
	}
}


void CplayerGUIDlg::SystemClear()
{
	//SetBtn(STATE_PREPARE);
	playerstate=STATE_PREPARE;
	m_curtime.SetWindowText(_T("00:00:00"));
	m_duration.SetWindowText(_T("00:00:00"));
	libvlc_mp=NULL;
	libvlc_m=NULL;
	m_progress.SetPos(0);
}

void CplayerGUIDlg::SetBtn(StartState state){
	if(state==STATE_STOP){
		GetDlgItem(ID_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(ID_INFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVETIEM)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANGESSID)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HIDE)->EnableWindow(FALSE);
		//GetDlgItem(ID_SHOWInfo)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_BUTTON_SAVETIEM)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHANGESSID)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HIDE)->EnableWindow(TRUE);
		GetDlgItem(ID_PAUSE)->EnableWindow(TRUE);
		GetDlgItem(ID_INFO)->EnableWindow(TRUE);
		//GetDlgItem(ID_SHOWInfo)->EnableWindow(TRUE);
	}
}

void CplayerGUIDlg::UNICODE_to_UTF8(CStringW& unicodeString, std::string& str)
{
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);

	char* buffer = new char[stringLength + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), buffer, stringLength, NULL, NULL);
	buffer[stringLength] = '\0';

	str = buffer;

	delete[] buffer;
}

void CplayerGUIDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnDropFiles(hDropInfo);

	LPTSTR pFilePathName =(LPTSTR)malloc(MAX_URL_LENGTH);
	::DragQueryFile(hDropInfo, 0, pFilePathName,MAX_URL_LENGTH);  // ��ȡ�Ϸ��ļ��������ļ�������ؼ���
	m_url.SetWindowText(pFilePathName);
	::DragFinish(hDropInfo);   // ע����������٣��������ͷ�Windows Ϊ�����ļ��ϷŶ�������ڴ�
	free(pFilePathName);

}


void CplayerGUIDlg::OnDestroy()
{
	/* Free the media_player */
	libvlc_release (libvlc_inst);

	CDialogEx::OnDestroy();
}
void CplayerGUIDlg::OnTimer(UINT_PTR nIDEvent)
{
		CString curtimestr;
	
	switch (nIDEvent)
	{
	case 1:
	   ++tss;
	   if(tss%60==0){
		  tmm++;
		   tss=0;
	   }
	   if(tmm+1%60==0){
		   thh++;
		   tmm=0;
	   }
		curtimestr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
		m_curtime.SetWindowText(curtimestr);
		break;
	case 2:
		urlopen("http://192.168.11.123/api/trio_sta");
		if(recording==0){
		GetDlgItem(ID_INFO)->SetWindowText(_T("�����ʼ"));
		}else if(recording==1){
		GetDlgItem(ID_INFO)->SetWindowText(_T("���ֹͣ"));
		}
		break;
	}
	//Stop in the end
	if(libvlc_media_player_get_state(libvlc_mp)==libvlc_Ended)
		OnBnClickedStop();


	CDialogEx::OnTimer(nIDEvent);
}


void CplayerGUIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_progress.GetSafeHwnd()){
		float posf=0.0;
		if(nSBCode==SB_THUMBPOSITION){
			posf=(float)nPos/100.0;
			libvlc_media_player_set_position(libvlc_mp,posf);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

/************************************************************************/
/*  ����SSID                                                                  */
/************************************************************************/
void CplayerGUIDlg::OnBnClickedHideSSID()
{
	string reboot_url= "http://192.168.11.123/api/reboot";
	string URL_Hide = "http://192.168.11.123/api/hidessid";
	Record(URL_Hide);
	Sleep(1000);
	AfxMessageBox("�������·����豸��������");
	{
		if(libvlc_mp!=NULL){
			libvlc_media_player_stop (libvlc_mp);
			libvlc_media_player_release (libvlc_mp);
			libvlc_mp=NULL;
			KillTimer(1);
			KillTimer(2);
		}
		SystemClear();
		startstate=STATE_STOP;
		SetBtn(STATE_STOP);
		GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
	}
	Record(reboot_url);
}
/************************************************************************/
/*      ��ʾSSID                                                         */
/************************************************************************/
void CplayerGUIDlg::OnBnClickedShowSSID()
{
	string reboot_url= "http://192.168.11.123/api/reboot";
	string URL_Show = "http://192.168.11.123/api/showssid";
	Record(URL_Show);
	AfxMessageBox("�������·����豸��������");
	{
		if(libvlc_mp!=NULL){
			libvlc_media_player_stop (libvlc_mp);
			libvlc_media_player_release (libvlc_mp);
			libvlc_mp=NULL;
			KillTimer(1);
			KillTimer(2);
		}
		SystemClear();
		startstate=STATE_STOP;
		SetBtn(STATE_STOP);
		GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
	}
	Record(reboot_url);

}
void CplayerGUIDlg::OnBnClickedInfo()
{
	//ShowWindow(SW_MINIMIZE);
	
	if(recording==0){
		//���Ϳ�ʼ¼������
		string fileName ="http://192.168.11.123/api/trio_rec?val="+getTime();
		Record(fileName);
	}else if(recording==1){
		//����ֹͣ¼������
		Record("http://192.168.11.123/api/trio_rec?val=video2016");
	}

}
void CplayerGUIDlg::OnBnClickedDownload()
{
	const int nBitSys = GetSystemBits();
	if(nBitSys==32){
			ShellExecute(NULL, _T("open"), _T(".\\filezilla_x86\\filezilla.exe"), _T(""), _T(""), SW_SHOW);
	}else if(nBitSys==64){
		  ShellExecute(NULL, _T("open"), _T(".\\filezilla\\filezilla.exe"), _T(""), _T(""), SW_SHOW);
	}
}

/**��ѯ�豸״̬*/
void  CplayerGUIDlg :: urlopen(string url)
{
	HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if(hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, url.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hHttp != NULL)
		{

			BYTE Temp[MAXSIZE];
			ULONG Number = 1;
			while (Number > 0)
			{
				s1="";
				InternetReadFile(hHttp, Temp, MAXSIZE-1, &Number);
				Temp[Number] = '\0';
				char *szTxt = (char *)Temp;
				string s(szTxt);
				if(s.size()>10){
					//��ȡ����ҳ��������
					/* "code": 0, "hidessid": 0, "volume": 0, "distanceint": 0, "distancedec": 0, "battery": 0, "magnification": 0, "airpressure": 0, "sdcard": 1, "sdcardcap": 7369, "sdcardfree": 7366, "recording": 1,
					"recordtm": 4, "recordname": "/mnt/sdcard/20160826/19/video20160826191413_%d.mov", "osd": "SaiTeng", "updatetime": 1 }*/
					size_t  statindex = s.find("<body>"); 
					size_t  endindex  = s.find("</body>");
					s1 =s.substr(statindex+6,endindex-statindex-6);

					Json::Reader reader;

					Json::Value root;

					if(reader.parse(s1,root))
					{
						sdcard = root["sdcard"].asInt();
						sdcardcap = root["sdcardcap"].asInt();
						sdcardfree = root["sdcardfree"].asInt();
						recording = root["recording"].asInt();
					}
				}
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}
	
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}
/**�豸¼���������*/
void CplayerGUIDlg::Record(string url){
	if(sdcard==0){
		AfxMessageBox("�����豸SD���Ƿ����");
	}else{
		HINTERNET hSession1 = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if(hSession1 != NULL)
		{
			HINTERNET hHttp = InternetOpenUrl(hSession1, url.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
			if (hHttp != NULL)
			{
			}
			InternetCloseHandle(hSession1);
			hSession1 = NULL;
		}
	}
}

void CplayerGUIDlg::ShowMessageBox()
{
	infoStatus="";CString str_con="";
	str.Format("%f",sdcardcap/1024.00);
	str1.Format("%f",sdcardfree/1024.00);
	if(sdcard==0){
		str2="�����豸SD���Ƿ����";
	}else
		str2 = str1+"G/"+str+"G";
	if(flag){
		 str_con="�豸������";
	}else
		 str_con="�豸δ����";

	infoStatus.AppendFormat(_T("========�豸��Ϣ========\r\n"));
	infoStatus.AppendFormat(_T("�豸����״̬:%s"),str_con+"\r\n");
	infoStatus.AppendFormat(_T("SD�����ÿռ�/�ܿռ�:%s"),str2+"\r\n");
	infoStatus.AppendFormat(_T("========����Ƶ��Ϣ========\r\n"));

	
	if(libvlc_mp!=NULL){
		libvlc_video_get_size(libvlc_mp,0,&video_w,&video_h);
		framerate=libvlc_media_player_get_fps(libvlc_mp);
		channel=libvlc_audio_get_channel(libvlc_mp);
	}
	infoStatus.AppendFormat(_T("��Ƶ��:%d\r\n��Ƶ��:%d\r\n֡��:%f\r\n")
		,video_w,video_h,framerate);
	
	infoStatus.AppendFormat(_T("��Ƶͨ��:%d\r\n"),channel);
	AfxMessageBox(infoStatus,MB_ICONINFORMATION);
}
/**���ϵͳʱ��*/
string CplayerGUIDlg::getTime()
{

	time_t t = time(0); 

	char tmp[64]; 

	strftime( tmp, sizeof(tmp), "%Y%m%d%X",localtime(&t) ); 

	string str(tmp);

	string::iterator it;

	for (it =str.begin(); it != str.end(); ++it)
	{
		if ( *it == '\:')
		{
			str.erase(it);
		}
	}

	string filename ="video"+str;

	return filename;
}

/*ͬ������ʱ�䵽�豸*/
void CplayerGUIDlg::SetTime(){

	time_t t = time(0); 

	char tmp[64]; 
	/************************************************************************/
	/*  ��ȡϵͳ��ǰ��ʱ�䡣���忴strftime���ܡ�                                                             */
	/************************************************************************/
	strftime( tmp, sizeof(tmp), "%Y-%m-%d %X",localtime(&t) ); 
	string str(tmp);
	string  url = "http://192.168.11.123/api/trio_tmr?val="+str;
	flag=false;
	HINTERNET hSession1 = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hSession1 != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession1, url.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hHttp != NULL)
		{
			flag=true;
		}
		if(!flag){
			AfxMessageBox("�����豸�Ƿ�����");
			if(libvlc_mp!=NULL){
				libvlc_media_player_stop (libvlc_mp);
				libvlc_media_player_release (libvlc_mp);
				libvlc_mp=NULL;
				KillTimer(1);
				KillTimer(2);
			}
			SystemClear();
			startstate=STATE_STOP;
			SetBtn(STATE_STOP);
			GetDlgItem(ID_START)->SetWindowText(_T("��ʼ"));
		}
		InternetCloseHandle(hSession1);
		hSession1 = NULL;
	}
}

int CplayerGUIDlg::GetSystemBits()
{
	SYSTEM_INFO si;
	SafeGetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		return 64;
	}
	return 32;
}
// ��ȫ��ȡ����ʵϵͳ��Ϣ
VOID CplayerGUIDlg::SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)
{
	if (NULL==lpSystemInfo)	return;
	typedef VOID (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress( GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");;
	if (NULL != fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo(lpSystemInfo);
	}
	else
	{
		GetSystemInfo(lpSystemInfo);
	}
}



