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
