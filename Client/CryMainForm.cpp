
// ClientDlg.cpp : 实现文件
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

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CClientDlg 对话框

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

// CClientDlg 消息处理程序

BOOL action::CryMainForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//SetWindowText("欢迎使用唐门插件...");
	//EndDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void action::CryMainForm::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
			SetWindowText("密码错误");
		}
		if (resp.status() == 3)
		{
			SetWindowText("用户名不存在");
		}
		if (resp.status() == 4)
		{
			SetWindowText("您的帐号已被封停");
		}
		if (resp.status() == 6)
		{
			SetWindowText("您的帐号已在线，请稍后再试。");
		}
		if (resp.status() == 7)
		{
			SetWindowText("您的帐号已到期，请及时续费！");
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
				AfxMessageBox("用户名长度 不能小于4位");
				return;
			}

			if (GetDlgItemText(IDC_EDIT_PASSWORD, szPass, 33) < 4)
			{
				AfxMessageBox("密码长度 不能小于4位");
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
