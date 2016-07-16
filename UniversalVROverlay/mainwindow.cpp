#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_remOverlayButton_clicked()
{
    //Remove Overlay from manager and delete overlay
}


//Add overlay button clicked
void MainWindow::on_addOverlayButton_clicked()
{
    //Popup Window Select Dialog
}


//Overlay Selection changed
void MainWindow::on_overlayList_itemSelectionChanged()
{
    //Fillout Overlay settings groupbox
}

void MainWindow::on_keybindBox_cursorPositionChanged(int arg1, int arg2)
{
    //Trigger keybind dialog
}

void MainWindow::on_xRotationSlider_sliderMoved(int position)
{
    //XRotation Update & textbox
}

void MainWindow::on_yRotationSlider_sliderMoved(int position)
{
    //YRotation Update
}

void MainWindow::on_zRotationSlider_sliderMoved(int position)
{
    //ZRotation Update
}

void MainWindow::on_scaleSlider_sliderMoved(int position)
{
    //Scale Update
}

void MainWindow::on_trackingSelect_currentIndexChanged(int index)
{
    //Update tracking device
}

void MainWindow::on_xRotateText_editingFinished()
{
    //Update X Translation & sliders
}

void MainWindow::on_yRotateText_editingFinished()
{
    //Update Y Translation & sliders
}

void MainWindow::on_zRotateText_editingFinished()
{
    //Update Z Translation & sliders
}
