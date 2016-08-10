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
#include "windowselect.h"
#include "ui_windowselect.h"

WindowSelect::WindowSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowSelect)
{
	wndVec.clear();
    ui->setupUi(this);
	EnumWindows(WindowSelectEnum, (LPARAM)&wndVec);
	for (std::vector<WindowDescriptor>::iterator it = wndVec.begin(); it != wndVec.end(); ++it)
	{
		ui->windowCombo->addItem(QString::fromWCharArray(it->getTitle().c_str()));
	}
}

WindowSelect::~WindowSelect()
{
    delete ui;
}

void WindowSelect::on_buttonBox_accepted()
{
	emit WindowSelected(wndVec[ui->windowCombo->currentIndex()]);
    this->accept();
}

void WindowSelect::on_buttonBox_rejected()
{
    this->reject();
}

BOOL CALLBACK WindowSelectEnum(HWND hWnd, LPARAM lParam)
{
	TCHAR title[500] = L"";
	TCHAR wndTitle[500] = L"";
	std::vector<WindowDescriptor>* wndVec = (std::vector<WindowDescriptor>*) lParam;
	ZeroMemory(title, sizeof(title));

	if (IsWindowVisible(hWnd))
	{
		//GetWindowText(hWnd, title, sizeof(title) / sizeof(title[0]));
		DWORD PID = NULL;
		GetWindowThreadProcessId(hWnd, &PID);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
		GetModuleFileNameEx(hProcess, NULL, title, sizeof(title) / sizeof(title[0]));
		//GetModuleFileName()
		//GetWindowModuleFileName(hWnd, title, sizeof(title) / sizeof(title[0]));

		TCHAR *name = wcsrchr(title, L'\\');

		if (GetWindowText(hWnd, wndTitle, 500) > 0 && name != NULL)
		{
			//--------------------------------------------------
			wndVec->push_back(WindowDescriptor(hWnd, std::wstring(wndTitle), std::wstring(name)));
		}
		//--------------------------------------------------
		CloseHandle(hProcess);

	}


	return TRUE;
}
