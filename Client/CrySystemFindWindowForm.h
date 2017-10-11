#pragma once
#include "action/IAction.h"
namespace action
{
	class CrySystemFindWindowForm : public CDialogEx, public IAction
	{
	public:
		CrySystemFindWindowForm(CWnd* pParent = NULL);   // 标准构造函数
		virtual ~CrySystemFindWindowForm();

		// 对话框数据
		enum { IDD = IDD_SPY_FORM };
	public:
		static CrySystemFindWindowForm* GetInstance();
		virtual void Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len) override;
		virtual LRESULT OnPostNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSendNotify(WPARAM wParam, LPARAM lParam);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
		// 生成的消息映射函数
		virtual BOOL OnInitDialog();
		virtual void OnTimer(UINT_PTR nIDEvent);
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnClose();
	};
}
