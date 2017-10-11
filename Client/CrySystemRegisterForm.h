#pragma once
#include "action/IAction.h"
namespace action
{
	class CrySystemRegisterForm : public CDialogEx, public IAction
	{
	public:
		CrySystemRegisterForm(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~CrySystemRegisterForm();

		// �Ի�������
		enum { IDD = IDD_REG_FORM };
	public:
		static CrySystemRegisterForm* GetInstance();
		virtual void Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len) override;
		virtual LRESULT OnPostNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSendNotify(WPARAM wParam, LPARAM lParam);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
		virtual void OnOK();
		DECLARE_MESSAGE_MAP()
	};
}