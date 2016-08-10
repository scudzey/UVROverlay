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

