// CryRegForm.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "CrySystemRegisterForm.h"
#include "afxdialogex.h"
#include "WorkServer.h"
#include <data\AccountRegister.pb.h>
#include "action\ActionID.h"

action::CrySystemRegisterForm::CrySystemRegisterForm(CWnd* pParent) : CDialogEx(action::CrySystemRegisterForm::IDD, pParent)
{

}

action::CrySystemRegisterForm::~CrySystemRegisterForm()
{

}

void action::CrySystemRegisterForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(action::CrySystemRegisterForm, CDialogEx)
	ON_MESSAGE(WM_POST_NOTIFY, OnPostNotify)
	ON_MESSAGE(WM_SEND_NOTIFY, OnSendNotify)
END_MESSAGE_MAP()


action::CrySystemRegisterForm* action::CrySystemRegisterForm::GetInstance()
{
	static action::CrySystemRegisterForm Init;
	return &Init;
}

void action::CrySystemRegisterForm::Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len)
{
	::SendMessage(client->GetWnd(), WM_SEND_NOTIFY, len, reinterpret_cast<LPARAM>(data));
}
LRESULT action::CrySystemRegisterForm::OnPostNotify(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
LRESULT action::CrySystemRegisterForm::OnSendNotify(WPARAM wParam, LPARAM lParam)
{
	data::AccountRegisterResponse resp;
	if (resp.ParsePartialFromArray(reinterpret_cast<const char *>(lParam), wParam))
	{
		if (!resp.status())
		{
			AfxMessageBox("注册成功\n");
		}

		if (resp.status())
		{
			AfxMessageBox("用户名已存在\n");
		}
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

	return TRUE;
}
void action::CrySystemRegisterForm::OnOK()
{
	auto server = m_Client.GetServer();
	{
		if (server)
		{
			TCHAR szName[33] = { 0 }, szPass[33] = { 0 }, szSPass[33] = { 0 };
			data::AccountRegisterRequest req;
			if (GetDlgItemText(IDC_EDIT_USERNAME, szName, 33) < 4)
			{
				AfxMessageBox("用户名长度 不能小于4位");
				return;
			}
			
			if (GetDlgItemText(IDC_EDIT_PASSWORD, szPass, 33) < 4)
			{
				AfxMessageBox("密码长度 不能小于4位");
				return;
			}

			if (GetDlgItemText(IDC_EDIT_SPASSWORD, szSPass, 33) < 4)
			{
				AfxMessageBox("超级密码长度 不能小于4位");
				return;
			}

			req.set_name(szName);
			req.set_passwd(szPass);
			req.set_super_passwd(szSPass);

			server->SetWnd(m_hWnd);
			server->Send(ActionID::AccountRegister, req);

			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
	}
}