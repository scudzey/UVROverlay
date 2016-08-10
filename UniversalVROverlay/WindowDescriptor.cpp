/*
**		VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**		This program is free software : you can redistribute it and / or modify
**		it under the terms of the GNU General Public License as published by
**		the Free Software Foundation, either version 3 of the License, or
**		(at your option) any later version.
**
**		This program is distributed in the hope that it will be useful,
**		but WITHOUT ANY WARRANTY; without even the implied warranty of
**		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
**		GNU General Public License for more details.
**
**		You should have received a copy of the GNU General Public License
**		along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
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
