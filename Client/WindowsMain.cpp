
// Client.cpp : ����Ӧ�ó��������Ϊ��
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
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
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
			//(_T("������������ʲô?"));
		}
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}
		m_Client.Stop();
	}
	return FALSE;
}