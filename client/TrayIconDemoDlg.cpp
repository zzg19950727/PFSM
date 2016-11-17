
// TrayIconDemoDlg.cpp : implementation file
//
#include "afx.h"
#include "stdafx.h"

#include "driver/message_to_driver.h"

#include "TrayIconDemo.h"
#include "TrayIconDemoDlg.h"

#include "afxdialogex.h"

#include <windowsx.h>
#include  <afxpriv.h>
#include "WideMulti.h"

#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTrayIconDemoDlg dialog
LOGIN_STATE CTrayIconDemoDlg::loginState = {false,false,"","",};

CTrayIconDemoDlg::CTrayIconDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrayIconDemoDlg::IDD, pParent)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_TrayFlag = FALSE;

	m_username = _T("");
	m_password = _T("");

	ZeroMemory( &pi, sizeof(pi) );

	strCurrentFolderPath = QueryExePath();
}

CTrayIconDemoDlg::~CTrayIconDemoDlg()
{
	//通知驱动
	start_service();
	if( pi.hProcess )
	TerminateProcess( pi.hProcess,0 );
}

void CTrayIconDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UserName, m_username);
	DDX_Text(pDX, IDC_PassWord, m_password);
}

BEGIN_MESSAGE_MAP(CTrayIconDemoDlg, CDialogEx)
	ON_WM_PAINT()
    ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)

	//与菜单功能做映射
	ON_COMMAND(ID_TRAYMENU_OPENAPP, &CTrayIconDemoDlg::OnTraymenuOpenapp)//打开主界面才方便登录
	ON_COMMAND(ID_TRAYMENU_LOGOUT, &CTrayIconDemoDlg::OnBnClickedLogout)//注销
	ON_COMMAND(ID_TRAYMENU_SETTINGS, &CTrayIconDemoDlg::OnTraymenuSettings)//系统设置
	ON_COMMAND(ID_TRAYMENU_EXIT, &CTrayIconDemoDlg::OnTraymenuExit)//退出
	
	//与登录窗口上相关按钮做映射
	ON_BN_CLICKED(IDC_Login, &CTrayIconDemoDlg::OnBnClickedLogin)//登录
END_MESSAGE_MAP()


// CTrayIconDemoDlg message handlers

BOOL CTrayIconDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//=======================================显示状态栏=========================================
	UINT array[1];
	for (int i=0;i<1;i++)
	{
		array[i]=1001+i;
	}
	m_StausBar.Create(this);		//创建状态栏窗口
	m_StausBar.SetIndicators(array,sizeof(array)/sizeof(UINT));//添加面板
	for (int i= 0 ;i < 1 ;i++)
	{
		m_StausBar.SetPaneInfo(i,array[i],0,250);			//设置面板宽度
	}
	m_StausBar.SetPaneText(0,_T("请登录！")); //设置面板文本
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);//显示状态栏

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTrayIconDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CTrayIconDemoDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
    if (nID == SC_MINIMIZE||nID == SC_CLOSE)         //最小化窗口或关闭窗口,若未登录，则最小化，若已登录，则进入托盘程序
    {
		if(loginState.islogin)
		{
			ToTray();
		}
		else
		{
			CDialogEx::OnSysCommand(nID, lParam);
		}
    }
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTrayIconDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTrayIconDemoDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam)
{
    return m_TrayIcon.OnTrayNotification(wParam,lParam);
}


void CTrayIconDemoDlg::ToTray()
{
    if(!m_TrayFlag)
    {
        //m_TrayIcon.Create(this,WM_ICON_NOTIFY,L"TrayIconDemo",m_hIcon,IDR_MENU1); //ttttttttttttttttttttttttttttttttt
		m_TrayIcon.Create(this, WM_ICON_NOTIFY, _T("TrayIconDemo"), m_hIcon, IDR_MENU1);
        m_TrayFlag = TRUE;
    }
    
	ShowWindow(SW_HIDE);
}

void CTrayIconDemoDlg::RemoveTray()
{
	 if(m_TrayFlag)
    {
        //m_TrayIcon.Create(this,WM_ICON_NOTIFY,L"TrayIconDemo",m_hIcon,IDR_MENU1); //ttttttttttttttttttttttttttttttttt
		m_TrayIcon.RemoveIcon();
        m_TrayFlag = FALSE;
    }
    
	ShowWindow(SW_SHOW);
}

void CTrayIconDemoDlg::OnTraymenuOpenapp()
{
    ShowWindow(SW_SHOW);//打开主界面
}

BOOL CTrayIconDemoDlg::PreTranslateMessage(MSG* pMsg)//防止编辑框遇到Enter键和Esc键时退出程序
{
	// TODO:  在此添加专用代码和/或调用基类
	switch (pMsg->wParam)
	{
	case VK_RETURN:
		;

	case VK_ESCAPE:
		return true; break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//登录
void CTrayIconDemoDlg::OnBnClickedLogin()
{
	USES_CONVERSION;

	//检测是否处于登录状态
	if(loginState.islogin==true)
	{
		MessageBox(_T("您已登录，不能重复登录！\n"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return ;
	}

	UpdateData(true);

	if(m_username=="")
	{
		MessageBox(_T("请输入用户名！"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(m_password=="")
	{
		MessageBox(_T("请输入密码！"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return ;
	}

	if(m_username=="zzg" && m_password=="0727")
	{
		//设置登录状态
		loginState.islogin = true;
		loginState.s_username = W2A(m_username);
		loginState.s_password = W2A(m_password);

		//设置状态栏文本
		CString msg;
		msg = m_username+_T(",已登录");
		m_StausBar.SetPaneText(0,msg); 

		stop_service();//通知驱动开始用户登录成功

		ToTray();
	}
	else
	{
		MessageBox(_T("登录失败！\n用户名或密码错误"), _T("提示"), MB_OK|MB_ICONERROR);
	}
}

//注销
void CTrayIconDemoDlg::OnBnClickedLogout()
{
	//检测是否处于登录状态
	if(loginState.islogin==false&&loginState.isOffMode==false)
	{
		MessageBox(_T("您尚未登录且没有启用离线模式，不能注销！\n"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return ;
	}
	else
	{
		//改变登录状态
		loginState.islogin = false;
		loginState.isOffMode = false;
		loginState.s_username = "";
		loginState.s_password = "";

		//设置状态栏文本
		m_StausBar.SetPaneText(0,_T("请登录!")); 
		if( pi.hProcess )
		TerminateProcess( pi.hProcess,0 );

		//TODO：通知驱动
		start_service();

		MessageBox(_T("注销成功！"), _T("提示"), MB_OK|MB_ICONINFORMATION);

		RemoveTray();
	}
	
	return;
}

void char2wchar(wchar_t *pwstr, const char *str)
{
	if(!str)
		return;

	ZeroMemory(pwstr, _CMD_PATH*sizeof(WCHAR));

	size_t nu = strlen(str);

	size_t n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),NULL,0);

	::MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),pwstr,int(n));    
}

void read_file()
{
    ifstream fin("c:/change.txt");
	if( fin.fail() )
		return;
    string str;
    char Arr[1024];

    while (!fin.eof())
    {
		INPUT_BUFFER buffer;
        fin.getline(Arr,1024);
		str = string(Arr);
		if(str == "1")
			buffer.command = ADD_PROTECTED_PATH;
		else if(str=="0")
			buffer.command = DEL_PROTECTED_PATH;
		else
			break;

		fin.getline(Arr,1024);
		str = string(Arr);
		transform(str.begin(),str.end(),str.begin(),::tolower);
		char2wchar((wchar_t *)buffer.protectedPath, str.c_str());

		fin.getline(Arr,1024);
		str = string(Arr);
		transform(str.begin(),str.end(),str.begin(),::tolower);
		char2wchar((wchar_t *)buffer.fileType, str.c_str());

		switch(buffer.command)
		{
		case ADD_PROTECTED_PATH:
			MyAddProtectPath(buffer.protectedPath, buffer.fileType);
			break;
		case DEL_PROTECTED_PATH:
			MyDelProtectPath(buffer.protectedPath, buffer.fileType);
			break;
		}
    }
    fin.close();
}

void CTrayIconDemoDlg::OnTraymenuSettings()
{
	STARTUPINFO si={sizeof(si)};

	LPCWSTR pro= L"setting.exe";
	 ::CreateProcess(pro,NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);

	 while( WAIT_OBJECT_0 != WaitForSingleObject(pi.hProcess,INFINITE) )
	 {
		 ;
	 }
	 read_file();
}


//退出
void CTrayIconDemoDlg::OnTraymenuExit()
{
	//通知驱动
	start_service();
	if( pi.hProcess )
	TerminateProcess( pi.hProcess,0 );
    m_TrayIcon.RemoveIcon();
	OnCancel();
}

char* CTrayIconDemoDlg::str2char(CString str)
{
char *ptr;
#ifdef _UNICODE
LONG len;
len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
ptr = new char[len + 1];
memset(ptr, 0, len + 1);
WideCharToMultiByte(CP_ACP, 0, str, -1, ptr, len + 1, NULL, NULL);
#else
ptr = new char[str.GetAllocLength() + 1];
sprintf(ptr, _T("%s"), str);
#endif
return ptr;
}

void CTrayIconDemoDlg::DoSomethingBeforeMsgLoop()
{
	if(loginState.islogin)
	{
		ShowWindow(SW_HIDE);
	}
}

CString CTrayIconDemoDlg::QueryExePath()
{
    TCHAR path[MAX_PATH];
    ::GetModuleFileName(NULL,path,MAX_PATH);
    CString p(path);
    CString subp;
    int nPos = p.ReverseFind('\\');
    //ASSERT(-1!=nPos);
    return p.Left(nPos+1);
}
