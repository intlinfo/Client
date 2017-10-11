
// ClientDlg.h : 头文件
//

#pragma once
#include "action/IAction.h"
namespace action
{
	class CryMainForm : public CDialogEx, public IAction
	{
		// 构造
	public:
		CryMainForm(CWnd* pParent = NULL);	// 标准构造函数

		// 对话框数据
		enum { IDD = IDD_MAIN_FORM };
	public:
		static CryMainForm* GetInstance();
		virtual void Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len) override;
		virtual LRESULT OnPostNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSendNotify(WPARAM wParam, LPARAM lParam);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	protected:
		HICON m_hIcon;

		// 生成的消息映射函数
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedButtonOk();
		afx_msg void OnBnClickedButtonOpenRegister();
		afx_msg void OnBnClickedButtonOpenRecharge();
	};
}