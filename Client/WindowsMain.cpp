
// Client.cpp : 定义应用程序的类行为。
//
#include "stdafx.h"
#include "WindowsMain.h"
#include "CryMainForm.h"

#include "InitEnv.h"
CWindowsMain theApp;

BEGIN_MESSAGE_MAP(CWindowsMain, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CWindowsMain::CWindowsMain()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL CWindowsMain::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	InitEnv		init;
	action::CryMainForm dlg;
	m_Client.SetMainForm(&dlg);
	if (m_Client.Start())
	{
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{

		}
		else if (nResponse == IDCANCEL)
		{
			//(_T("程序更新这里干什么?"));
		}
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}
		m_Client.Stop();
	}
	return FALSE;
}