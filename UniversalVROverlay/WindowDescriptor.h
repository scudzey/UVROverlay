#pragma once
#include <string>
#include <Windows.h>

class WindowDescriptor
{
public:
	WindowDescriptor(const HWND hWnd, const std::wstring &wndTitle=NULL, const std::wstring &exeName=NULL);
	virtual ~WindowDescriptor();

	void setHwnd(const HWND srchWnd);
	HWND getHwnd() const;

	void setTitle(const std::wstring &title);
	std::wstring getTitle() const;

	void setExe(const std::wstring &exe);
	std::wstring getExe() const;

private:
	HWND m_hWnd;
	std::wstring m_windowTitle;
	std::wstring m_executableName;
};

