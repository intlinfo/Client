
// Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

// CClientApp: 
// �йش����ʵ�֣������ Client.cpp
//

class CWindowsMain : public CWinApp
{
public:
	CWindowsMain();
private:
	CWinThread*		m_Thread;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};