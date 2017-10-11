// CrySystemRecharge.cpp : ʵ���ļ�
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
			SetWindowText("��������ʺŲ�����");
		}
		if (resp.status() == 1)
		{
			SetWindowText("����������벻��ȷ");
		}
		if (resp.status() == 2)
		{
			SetWindowText("������ĳ�ֵ���Ų�����");
		}
		if (resp.status() == 3)
		{
			SetWindowText("������ĳ�ֵ���벻��ȷ");
		}
		if (resp.status() == 4)
		{
			SetWindowText("��ֵʧ�ܡ����Ժ�����");
		}
		if (resp.status() == 5)
		{
			SetWindowText("�����ʺų�ֵ�ɹ�");
		}
		if (resp.status() == 6)
		{
			SetWindowText("������ĳ�ֵ���ѱ�ʹ��");
		}
		if (resp.status() == 7)
		{
			SetWindowText("������ĳ�ֵ���ѱ�����");
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
				AfxMessageBox("�û������� ����С��4λ");
				return;
			}
			if (GetDlgItemText(IDC_EDIT_PASSWORD, szPass, 33) < 4)
			{
				AfxMessageBox("ԭʼ���볤�� ����С��4λ");
				return;
			}
			if (GetDlgItemText(IDC_EDIT_CARD, szCard, 48) < 32)
			{
				AfxMessageBox("��ֵ���ų��Ȳ���16λ");
				return;
			}
			if (GetDlgItemText(IDC_EDIT_CARDPWD, szCardpwd, 48) < 32)
			{
				AfxMessageBox("��ֵ���볤�Ȳ���16λ");
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