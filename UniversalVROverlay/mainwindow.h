#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../inc/WindowOverlay.h"
#include "../inc/OverlayManager.h"
#include "../inc/OverlayTexture.h"
#include "../inc/RenderEnvironment.h"
#include <QMainWindow>
#include <QtDebug> 
#include <windows.h>
extern "C"
{
#include <hidsdi.h>
}


#include <openvr.h>


#include "windowselect.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, OverlayManager* mgr, vr::IVRSystem* vrSystem);
    ~MainWindow();

	void show();
	

private slots:
    void on_remOverlayButton_clicked();

    void on_addOverlayButton_clicked();

    void on_overlayList_itemSelectionChanged();

    void on_keybindBox_cursorPositionChanged(int arg1, int arg2);

    void on_xRotationSlider_sliderMoved(int position);

    void on_yRotationSlider_sliderMoved(int position);

    void on_zRotationSlizer_sliderMoved(int position);

    void on_scaleSlider_sliderMoved(int position);

    void on_trackingSelect_currentIndexChanged(int index);

    void on_xRotateText_editingFinished();
    void on_yRotateText_editingFinished();
    void on_zRotateText_editingFinished();

	void on_xTranslationText_editingFinished();
	void on_yTranslationText_editingFinished();
	void on_zTranslationText_editingFinished();

	void on_scaleText_editingFinished();

	void on_AddOverlay(WindowDescriptor& data);



    void on_zTranslateBackBig_clicked();

    void on_zTranslateBack_clicked();

    void on_zTranslateForward_clicked();

    void on_zTranslateForwardBig_clicked();

    void on_xTranslateLeftBig_clicked();

    void on_xTranslateLeft_clicked();

    void on_xTranslateRight_clicked();

    void on_xTranslateRightBig_clicked();

    void on_yTranslateUpBig_clicked();

    void on_yTranslateUp_clicked();

    void on_yTranslateDown_clicked();

    void on_yTranslateDownBig_clicked();

    void on_rateSelect_currentIndexChanged(int index);

    void on_transparancyText_editingFinished();

    void on_transparancySlider_sliderMoved(int position);

	void on_textureUpdate(int index);

private:
	bool nativeEvent(const QByteArray& eventType, void* message, long *result);
    Ui::MainWindow *ui;
	OverlayManager* m_mgr;
	RenderEnvironment* m_env;
	vr::IVRSystem* m_vrSystem;
};

#endif // MAINWINDOW_H
