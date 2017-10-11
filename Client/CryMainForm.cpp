
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CryMainForm.h"
#include "afxdialogex.h"
#include <Poco\DateTime.h>
#include "Client.h"
#include "WorkServer.h"
#include <data\AccountLogin.pb.h>
#include "action\ActionID.h"
#include "CrySystemRegisterForm.h"
#include "CrySystemRechargeForm.h"
#include "CrySystemFindWindowForm.h"

class CryHelpForm : public CDialogEx
{
public:
	CryHelpForm();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CryHelpForm::CryHelpForm() : CDialogEx(CryHelpForm::IDD)
{

}

void CryHelpForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CryHelpForm, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg �Ի���

namespace action
{

}
action::CryMainForm::CryMainForm(CWnd* pParent) : CDialogEx(action::CryMainForm::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void action::CryMainForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(action::CryMainForm, CDialogEx)
	ON_MESSAGE(WM_POST_NOTIFY, OnPostNotify)
	ON_MESSAGE(WM_SEND_NOTIFY, OnSendNotify)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CryMainForm::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_REGISTER, &CryMainForm::OnBnClickedButtonOpenRegister)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RECHARGE, &CryMainForm::OnBnClickedButtonOpenRecharge)
END_MESSAGE_MAP()

// CClientDlg ��Ϣ�������

BOOL action::CryMainForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	//SetWindowText("��ӭʹ�����Ų��...");
	//EndDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void action::CryMainForm::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CryHelpForm dlg;
		dlg.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void action::CryMainForm::OnPaint()
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
HCURSOR action::CryMainForm::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

action::CryMainForm* action::CryMainForm::GetInstance()
{
	static action::CryMainForm Init;
	return &Init;
}
void action::CryMainForm::Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len)
{
	::PostMessage(client->GetWnd(), WM_SEND_NOTIFY, len, reinterpret_cast<LPARAM>(data));
}
LRESULT action::CryMainForm::OnPostNotify(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
LRESULT action::CryMainForm::OnSendNotify(WPARAM wParam, LPARAM lParam)
{
	data::AccountLoginResponse resp;
	if (resp.ParsePartialFromArray(reinterpret_cast<const char *>(lParam), wParam))
	{
		if (!resp.status())
		{
			if (resp.time())
			{
				EndDialog(IDOK);
				CrySystemFindWindowForm dlg;
				dlg.DoModal();
			}
		}
		if (resp.status() == 2)
		{
			SetWindowText("�������");
		}
		if (resp.status() == 3)
		{
			SetWindowText("�û���������");
		}
		if (resp.status() == 4)
		{
			SetWindowText("�����ʺ��ѱ���ͣ");
		}
		if (resp.status() == 6)
		{
			SetWindowText("�����ʺ������ߣ����Ժ����ԡ�");
		}
		if (resp.status() == 7)
		{
			SetWindowText("�����ʺ��ѵ��ڣ��뼰ʱ���ѣ�");
		}
		GetDlgItem(IDC_BUTTON_OK)->EnableWindow(TRUE);
	}
	return TRUE;
}
void action::CryMainForm::OnBnClickedButtonOk()
{
	auto server = m_Client.GetServer();
	{
		if (server)
		{
			TCHAR szName[33] = { 0 }, szPass[33] = { 0 };
			data::AccountLoginRequest req;
			if (GetDlgItemText(IDC_EDIT_USERNAME, szName, 33) < 4)
			{
				AfxMessageBox("�û������� ����С��4λ");
				return;
			}

			if (GetDlgItemText(IDC_EDIT_PASSWORD, szPass, 33) < 4)
			{
				AfxMessageBox("���볤�� ����С��4λ");
				return;
			}

			req.set_name(szName);
			req.set_passwd(szPass);

			server->SetWnd(m_hWnd);

			server->Send(ActionID::AccountLogin, req);
			GetDlgItem(IDC_BUTTON_OK)->EnableWindow(FALSE);
		}
	}
}

void action::CryMainForm::OnBnClickedButtonOpenRegister()
{
	CrySystemRegisterForm dlg;
	dlg.DoModal();
}

void action::CryMainForm::OnBnClickedButtonOpenRecharge()
{
	CrySystemRechargeForm dlg;
	dlg.DoModal();
}
