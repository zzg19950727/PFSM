
// TrayIconDemoDlg.h : header file
//
#include "TrayIcon.h"
#include "CamException.h"
#include <string>
#include <Dbt.h>
#pragma once

using namespace std;

typedef struct _loginState
{
	bool islogin;
	bool isOffMode;
	std::string s_username;
	std::string s_password;

}LOGIN_STATE;

// CTrayIconDemoDlg dialog
class CTrayIconDemoDlg : public CDialogEx
{
public:
	CTrayIconDemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CTrayIconDemoDlg();	// standard destroyor
    LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);// Dialog Data

	enum { IDD = IDD_TRAYICONDEMO_DIALOG };
	void DoSomethingBeforeMsgLoop();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);//防止ENTER,ESC导致退出程序

	DECLARE_MESSAGE_MAP()

private:
    void ToTray();
	void RemoveTray();

	CString QueryExePath();//获取当前程序所在文件夹路径

	char* str2char(CString str);

protected:
	HICON m_hIcon;
	
private:  

	CStatusBar  m_StausBar;//状态栏

	CamException exception;//异常记录日志

	CString strCurrentFolderPath;//目前程序路径

	PROCESS_INFORMATION pi;
public:
	BOOL m_TrayFlag;//if the tray icon has been displayed before,the value is true,and vice versa.
	CTrayIcon m_TrayIcon;

	CString m_username;
	CString m_password;

	static LOGIN_STATE loginState;

	afx_msg void OnTraymenuOpenapp();//打开主界面函数
    afx_msg void OnTraymenuExit();//退出程序函数
	afx_msg void OnBnClickedLogin();//登入函数
	afx_msg void OnBnClickedLogout();//登出函数
	afx_msg void OnTraymenuSettings();
};

