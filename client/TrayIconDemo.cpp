
// TrayIconDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TrayIconDemo.h"
#include "TrayIconDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTrayIconDemoApp

BEGIN_MESSAGE_MAP(CTrayIconDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTrayIconDemoApp construction

CTrayIconDemoApp::CTrayIconDemoApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTrayIconDemoApp object

CTrayIconDemoApp theApp;


// CTrayIconDemoApp initialization

BOOL CTrayIconDemoApp::InitInstance()
{
	//让程序只能启动一次
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL,szPath,MAX_PATH);
	CString strFile(szPath);
	strFile = strFile.Right( strFile.GetLength()-1-strFile.ReverseFind(_T('\\')) );
	
	 HANDLE h =CreateMutex(NULL,FALSE,strFile);
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{	
		MessageBox(NULL,_T("程序已启动，不能重复启动！"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	//加载皮肤 
	
	VERIFY( 1 == InitSkinMagicLib(AfxGetInstanceHandle(), NULL, NULL, NULL));//初始化皮肤库,关键在第一个要获取实例句柄。其他可为NULL。
	//VERIFY( 1 == LoadSkinFile(_TEXT("smf/xpgrean_change.smf")));//加载皮肤库文件
	//VERIFY( 1 == LoadSkinFile(_TEXT("smf//xpsteel.smf")));//加载皮肤库文件
	//VERIFY(0 != LoadSkinFromResource(NULL,"IDR_SKIN2","\"SKIN\""));
	VERIFY (1==LoadSkinFromResource ( AfxGetInstanceHandle() , (LPCSTR)IDR_SKIN2 ,"SKIN") );
	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CTrayIconDemoDlg dlg;
	m_pMainWnd = &dlg;

	//将皮肤应用到程序上
	
	VERIFY( 1 == SetWindowSkin( m_pMainWnd->m_hWnd , "Dialog" ));
    //VERIFY( 1 == SetDialogSkin( "Dialog" ) );
	VERIFY( 1 == SetDialogSkin( "Dialog" ) );
	//VERIFY( 1 ==SetShapeWindowSkin( m_pMainWnd->m_hWnd , "PopupMenu"));
	
	

	//INT_PTR nResponse = dlg.DoModal();
	dlg.Create(CTrayIconDemoDlg::IDD);
    dlg.DoSomethingBeforeMsgLoop(); // 可以在此函数隐藏窗口及执行其它操作
    INT_PTR nResponse =  dlg.RunModalLoop();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CTrayIconDemoApp::ExitInstance()
{
	ExitSkinMagicLib();
	return CWinApp::ExitInstance();
}