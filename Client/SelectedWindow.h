#pragma once

class CSelectedWnd
{
public:
	virtual void SetSelectedWindow(HWND hWnd, DWORD ThreadId, DWORD dwProcessId) = 0;
protected:
	CSelectedWnd() = default;
	CSelectedWnd(const CSelectedWnd &) = default;
	CSelectedWnd &operator=(const CSelectedWnd &) = default;
};