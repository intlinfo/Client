// CrySystemRecharge.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "CrySystemRechargeForm.h"
#include "afxdialogex.h"
#include <data\AccountRecharge.pb.h>

action::CrySystemRechargeForm::CrySystemRechargeForm(CWnd* pParent) : CDialogEx(action::CrySystemRechargeForm::IDD, pParent)
{

}

action::CrySystemRechargeForm::~CrySystemRechargeForm()
{
}

void action::CrySystemRechargeForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(action::CrySystemRechargeForm, CDialogEx)
	ON_MESSAGE(WM_POST_NOTIFY, OnPostNotify)
	ON_MESSAGE(WM_SEND_NOTIFY, OnSendNotify)
END_MESSAGE_MAP()

action::CrySystemRechargeForm* action::CrySystemRechargeForm::GetInstance()
{
	static action::CrySystemRechargeForm Init;
	return &Init;
}

void action::CrySystemRechargeForm::Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len)
{
	::SendMessage(client->GetWnd(), WM_SEND_NOTIFY, len, reinterpret_cast<LPARAM>(data));
}
LRESULT action::CrySystemRechargeForm::OnPostNotify(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
LRESULT action::CrySystemRechargeForm::OnSendNotify(WPARAM wParam, LPARAM lParam)
{
	data::AccountRechargeResponse resp;
	if (resp.ParsePartialFromArray(reinterpret_cast<const char *>(lParam), wParam))
	{
		if (resp.status() == 0)
		{
			SetWindowText("您输入的帐号不存在");
		}
		if (resp.status() == 1)
		{
			SetWindowText("您输入的密码不正确");
		}
		if (resp.status() == 2)
		{
			SetWindowText("您输入的充值卡号不存在");
		}
		if (resp.status() == 3)
		{
			SetWindowText("您输入的充值密码不正确");
		}
		if (resp.status() == 4)
		{
			SetWindowText("充值失败、请稍后再试");
		}
		if (resp.status() == 5)
		{
			SetWindowText("您的帐号充值成功");
		}
		if (resp.status() == 6)
		{
			SetWindowText("您输入的充值卡已被使用");
		}
		if (resp.status() == 7)
		{
			SetWindowText("您输入的充值卡已被锁定");
		}
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	return TRUE;
}
void action::CrySystemRechargeForm::OnOK()
{
	auto server = m_Client.GetServer();
	{
		if (server)
		{
			TCHAR szName[33] = { 0 }, szPass[33] = { 0 }, szCard[48] = { 0 }, szCardpwd[48] = { 0 };
			if (GetDlgItemText(IDC_EDIT_USERNAME, szName, 33) < 4)
			{
				AfxMessageBox("用户名长度 不能小于4位");
				return;
			}
			if (GetDlgItemText(IDC_EDIT_PASSWORD, szPass, 33) < 4)
			{
				AfxMessageBox("原始密码长度 不能小于4位");
				return;
			}
			if (GetDlgItemText(IDC_EDIT_CARD, szCard, 48) < 32)
			{
				AfxMessageBox("充值卡号长度不足16位");
				return;
			}
			if (GetDlgItemText(IDC_EDIT_CARDPWD, szCardpwd, 48) < 32)
			{
				AfxMessageBox("充值密码长度不足16位");
				return;
			}

			data::AccountRechargeRequest req;

			req.set_name(szName);
			req.set_passwd(szPass);
			req.set_card(szCard);
			req.set_cardpwd(szCardpwd);

			server->SetWnd(m_hWnd);
			server->Send(ActionID::AccounRecharge, req);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
	}
}