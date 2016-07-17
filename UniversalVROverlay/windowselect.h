#ifndef WINDOWSELECT_H
#define WINDOWSELECT_H

#include <QDialog>

namespace Ui {
class WindowSelect;
}

class WindowSelect : public QDialog
{
    Q_OBJECT

public:
    explicit WindowSelect(QWidget *parent = 0);
    ~WindowSelect();

private:
    Ui::WindowSelect *ui;
};

#endif // WINDOWSELECT_H
