
// Client.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

// CClientApp: 
// 有关此类的实现，请参阅 Client.cpp
//

class CWindowsMain : public CWinApp
{
public:
	CWindowsMain();
private:
	CWinThread*		m_Thread;
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};