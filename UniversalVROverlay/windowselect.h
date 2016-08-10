/*
**  VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**	This program is free software : you can redistribute it and / or modify
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
#ifndef WINDOWSELECT_H
#define WINDOWSELECT_H
#include "WindowDescriptor.h"
#include <QDialog>
#include <Psapi.h>

BOOL CALLBACK WindowSelectEnum(HWND hWnd, LPARAM lParam);

namespace Ui {
class WindowSelect;
}

class WindowSelect : public QDialog
{
    Q_OBJECT

public:
    explicit WindowSelect(QWidget *parent = 0);
    ~WindowSelect();

signals:
	void WindowSelected(WindowDescriptor& data);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::WindowSelect *ui;
	std::vector<WindowDescriptor> wndVec;
};

#endif // WINDOWSELECT_H
