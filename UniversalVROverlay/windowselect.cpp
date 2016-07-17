#include "windowselect.h"
#include "ui_windowselect.h"

WindowSelect::WindowSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowSelect)
{
    ui->setupUi(this);
}

WindowSelect::~WindowSelect()
{
    delete ui;
}
