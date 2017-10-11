// CrySystemFindWindowForm.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "CrySystemFindWindowForm.h"
#include "afxdialogex.h"
#include <data\Keepalive.pb.h>
#include "action\ActionID.h"

action::CrySystemFindWindowForm::CrySystemFindWindowForm(CWnd* pParent) : CDialogEx(CrySystemFindWindowForm::IDD, pParent)
{

}

action::CrySystemFindWindowForm::~CrySystemFindWindowForm()
{

}

void action::CrySystemFindWindowForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(action::CrySystemFindWindowForm, CDialogEx)
	ON_MESSAGE(WM_POST_NOTIFY, OnPostNotify)
	ON_MESSAGE(WM_SEND_NOTIFY, OnSendNotify)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()
BOOL action::CrySystemFindWindowForm::OnInitDialog()
{
	SetTimer(1, 10000, NULL);
	return TRUE;
}
void action::CrySystemFindWindowForm::OnClose()
{
	CDialog::OnClose();
}
action::CrySystemFindWindowForm* action::CrySystemFindWindowForm::GetInstance()
{
	static action::CrySystemFindWindowForm Init;
	return &Init;
}

void action::CrySystemFindWindowForm::Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len)
{
	::SendMessage(client->GetWnd(), WM_SEND_NOTIFY, len, reinterpret_cast<LPARAM>(data));
}
LRESULT action::CrySystemFindWindowForm::OnPostNotify(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
LRESULT action::CrySystemFindWindowForm::OnSendNotify(WPARAM wParam, LPARAM lParam)
{
	data::KeepaliveResponse resp;

	if (resp.ParsePartialFromArray(reinterpret_cast<const char *>(lParam), wParam))
	{
		if (resp.status() == 1)
		{
			TRACE("心跳正常\n");
		}
	}
	return TRUE;
}
void action::CrySystemFindWindowForm::OnTimer(UINT_PTR nIDEvent)
{
	auto server = m_Client.GetServer();
	{
		if (server)
		{
			data::KeepaliveRequest req;
			server->SetWnd(m_hWnd);
			server->Send(ActionID::Keepalive, req);
		}
	}
}