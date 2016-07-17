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
