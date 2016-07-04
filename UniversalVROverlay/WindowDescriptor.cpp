#include "WindowDescriptor.h"


WindowDescriptor::WindowDescriptor(const HWND hWnd, const std::wstring & wndTitle, const std::wstring & exeName)
	: m_hWnd(hWnd)
	, m_windowTitle(wndTitle)
	, m_executableName(exeName)
{
}

WindowDescriptor::~WindowDescriptor()
{
}

void WindowDescriptor::setHwnd(const HWND srchWnd)
{
	m_hWnd = srchWnd;
}

HWND WindowDescriptor::getHwnd() const
{
	return m_hWnd;
}

void WindowDescriptor::setTitle(const std::wstring & title)
{
	m_windowTitle = title;
}

std::wstring WindowDescriptor::getTitle() const
{
	return std::wstring(m_windowTitle);
}

void WindowDescriptor::setExe(const std::wstring & exe)
{
	m_executableName = exe;
}

std::wstring WindowDescriptor::getExe() const
{
	return std::wstring(m_executableName);
}
