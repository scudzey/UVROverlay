#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_remOverlayButton_clicked();

    void on_addOverlayButton_clicked();

    void on_overlayList_itemSelectionChanged();

    void on_keybindBox_cursorPositionChanged(int arg1, int arg2);

    void on_xRotationSlider_sliderMoved(int position);

    void on_yRotationSlider_sliderMoved(int position);

    void on_zRotationSlider_sliderMoved(int position);

    void on_scaleSlider_sliderMoved(int position);

    void on_trackingSelect_currentIndexChanged(int index);

    void on_xRotateText_editingFinished();
    void on_yRotateText_editingFinished();
    void on_zRotateText_editingFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
