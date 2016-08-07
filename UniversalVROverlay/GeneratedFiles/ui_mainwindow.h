/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QListWidget *overlayList;
    QPushButton *addOverlayButton;
    QPushButton *remOverlayButton;
    QGroupBox *groupBox;
    QLineEdit *titleBox;
    QLineEdit *keybindBox;
    QLabel *label;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QPushButton *xTranslateLeftBig;
    QPushButton *xTranslateLeft;
    QPushButton *xTranslateRight;
    QPushButton *xTranslateRightBig;
    QPushButton *yTranslateDown;
    QPushButton *yTranslateDownBig;
    QPushButton *yTranslateUp;
    QPushButton *yTranslateUpBig;
    QLineEdit *xTranslationText;
    QLineEdit *yTranslationText;
    QLabel *label_3;
    QLabel *label_4;
    QGroupBox *groupBox_3;
    QLabel *label_6;
    QPushButton *zTranslateBack;
    QPushButton *zTranslateBackBig;
    QPushButton *zTranslateForwardBig;
    QLineEdit *zTranslationText;
    QPushButton *zTranslateForward;
    QGroupBox *groupBox_4;
    QSlider *xRotationSlider;
    QSlider *yRotationSlider;
    QSlider *zRotationSlizer;
    QLineEdit *xRotateText;
    QLineEdit *yRotateText;
    QLineEdit *zRotateText;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_8;
    QGroupBox *groupBox_5;
    QSlider *scaleSlider;
    QLineEdit *scaleText;
    QGroupBox *groupBox_6;
    QComboBox *trackingSelect;
    QGroupBox *groupBox_7;
    QSlider *transparancySlider;
    QLineEdit *transparancyText;
    QGroupBox *groupBox_8;
    QComboBox *rateSelect;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(726, 548);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(726, 548));
        MainWindow->setMaximumSize(QSize(726, 548));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        overlayList = new QListWidget(centralWidget);
        overlayList->setObjectName(QStringLiteral("overlayList"));
        overlayList->setGeometry(QRect(0, 30, 291, 491));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(overlayList->sizePolicy().hasHeightForWidth());
        overlayList->setSizePolicy(sizePolicy1);
        addOverlayButton = new QPushButton(centralWidget);
        addOverlayButton->setObjectName(QStringLiteral("addOverlayButton"));
        addOverlayButton->setGeometry(QRect(260, 0, 31, 31));
        remOverlayButton = new QPushButton(centralWidget);
        remOverlayButton->setObjectName(QStringLiteral("remOverlayButton"));
        remOverlayButton->setGeometry(QRect(230, 0, 31, 31));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(300, 24, 421, 498));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        titleBox = new QLineEdit(groupBox);
        titleBox->setObjectName(QStringLiteral("titleBox"));
        titleBox->setGeometry(QRect(80, 30, 201, 20));
        keybindBox = new QLineEdit(groupBox);
        keybindBox->setObjectName(QStringLiteral("keybindBox"));
        keybindBox->setGeometry(QRect(80, 60, 201, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 47, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 47, 20));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 100, 241, 151));
        xTranslateLeftBig = new QPushButton(groupBox_2);
        xTranslateLeftBig->setObjectName(QStringLiteral("xTranslateLeftBig"));
        xTranslateLeftBig->setGeometry(QRect(10, 50, 31, 51));
        xTranslateLeft = new QPushButton(groupBox_2);
        xTranslateLeft->setObjectName(QStringLiteral("xTranslateLeft"));
        xTranslateLeft->setGeometry(QRect(40, 50, 31, 51));
        xTranslateRight = new QPushButton(groupBox_2);
        xTranslateRight->setObjectName(QStringLiteral("xTranslateRight"));
        xTranslateRight->setGeometry(QRect(170, 50, 31, 51));
        xTranslateRightBig = new QPushButton(groupBox_2);
        xTranslateRightBig->setObjectName(QStringLiteral("xTranslateRightBig"));
        xTranslateRightBig->setGeometry(QRect(200, 50, 31, 51));
        yTranslateDown = new QPushButton(groupBox_2);
        yTranslateDown->setObjectName(QStringLiteral("yTranslateDown"));
        yTranslateDown->setGeometry(QRect(81, 100, 80, 23));
        yTranslateDownBig = new QPushButton(groupBox_2);
        yTranslateDownBig->setObjectName(QStringLiteral("yTranslateDownBig"));
        yTranslateDownBig->setGeometry(QRect(81, 120, 80, 23));
        yTranslateUp = new QPushButton(groupBox_2);
        yTranslateUp->setObjectName(QStringLiteral("yTranslateUp"));
        yTranslateUp->setGeometry(QRect(81, 30, 80, 23));
        yTranslateUpBig = new QPushButton(groupBox_2);
        yTranslateUpBig->setObjectName(QStringLiteral("yTranslateUpBig"));
        yTranslateUpBig->setGeometry(QRect(81, 10, 80, 23));
        xTranslationText = new QLineEdit(groupBox_2);
        xTranslationText->setObjectName(QStringLiteral("xTranslationText"));
        xTranslationText->setGeometry(QRect(78, 72, 41, 20));
        yTranslationText = new QLineEdit(groupBox_2);
        yTranslationText->setObjectName(QStringLiteral("yTranslationText"));
        yTranslationText->setGeometry(QRect(122, 72, 41, 20));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(98, 54, 16, 16));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(140, 54, 16, 16));
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(260, 100, 151, 151));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(71, 57, 16, 16));
        zTranslateBack = new QPushButton(groupBox_3);
        zTranslateBack->setObjectName(QStringLiteral("zTranslateBack"));
        zTranslateBack->setGeometry(QRect(34, 33, 80, 23));
        zTranslateBackBig = new QPushButton(groupBox_3);
        zTranslateBackBig->setObjectName(QStringLiteral("zTranslateBackBig"));
        zTranslateBackBig->setGeometry(QRect(34, 13, 80, 23));
        zTranslateForwardBig = new QPushButton(groupBox_3);
        zTranslateForwardBig->setObjectName(QStringLiteral("zTranslateForwardBig"));
        zTranslateForwardBig->setGeometry(QRect(34, 123, 80, 23));
        zTranslationText = new QLineEdit(groupBox_3);
        zTranslationText->setObjectName(QStringLiteral("zTranslationText"));
        zTranslationText->setGeometry(QRect(53, 75, 41, 20));
        zTranslateForward = new QPushButton(groupBox_3);
        zTranslateForward->setObjectName(QStringLiteral("zTranslateForward"));
        zTranslateForward->setGeometry(QRect(34, 103, 80, 23));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(25, 250, 371, 141));
        xRotationSlider = new QSlider(groupBox_4);
        xRotationSlider->setObjectName(QStringLiteral("xRotationSlider"));
        xRotationSlider->setGeometry(QRect(30, 30, 281, 22));
        xRotationSlider->setMinimum(-180);
        xRotationSlider->setMaximum(180);
        xRotationSlider->setOrientation(Qt::Horizontal);
        xRotationSlider->setTickPosition(QSlider::NoTicks);
        yRotationSlider = new QSlider(groupBox_4);
        yRotationSlider->setObjectName(QStringLiteral("yRotationSlider"));
        yRotationSlider->setGeometry(QRect(30, 70, 281, 22));
        yRotationSlider->setMinimum(-180);
        yRotationSlider->setMaximum(180);
        yRotationSlider->setOrientation(Qt::Horizontal);
        zRotationSlizer = new QSlider(groupBox_4);
        zRotationSlizer->setObjectName(QStringLiteral("zRotationSlizer"));
        zRotationSlizer->setGeometry(QRect(30, 110, 281, 22));
        zRotationSlizer->setMinimum(-180);
        zRotationSlizer->setMaximum(180);
        zRotationSlizer->setOrientation(Qt::Horizontal);
        xRotateText = new QLineEdit(groupBox_4);
        xRotateText->setObjectName(QStringLiteral("xRotateText"));
        xRotateText->setGeometry(QRect(320, 30, 41, 20));
        yRotateText = new QLineEdit(groupBox_4);
        yRotateText->setObjectName(QStringLiteral("yRotateText"));
        yRotateText->setGeometry(QRect(320, 70, 41, 20));
        zRotateText = new QLineEdit(groupBox_4);
        zRotateText->setObjectName(QStringLiteral("zRotateText"));
        zRotateText->setGeometry(QRect(320, 110, 41, 20));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 28, 16, 20));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 68, 16, 20));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 108, 16, 20));
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 390, 241, 51));
        scaleSlider = new QSlider(groupBox_5);
        scaleSlider->setObjectName(QStringLiteral("scaleSlider"));
        scaleSlider->setGeometry(QRect(10, 20, 171, 22));
        scaleSlider->setMaximum(101);
        scaleSlider->setOrientation(Qt::Horizontal);
        scaleText = new QLineEdit(groupBox_5);
        scaleText->setObjectName(QStringLiteral("scaleText"));
        scaleText->setGeometry(QRect(190, 20, 41, 20));
        groupBox_6 = new QGroupBox(groupBox);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(260, 390, 151, 51));
        trackingSelect = new QComboBox(groupBox_6);
        trackingSelect->setObjectName(QStringLiteral("trackingSelect"));
        trackingSelect->setGeometry(QRect(10, 20, 131, 22));
        groupBox_7 = new QGroupBox(groupBox);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 440, 241, 51));
        transparancySlider = new QSlider(groupBox_7);
        transparancySlider->setObjectName(QStringLiteral("transparancySlider"));
        transparancySlider->setGeometry(QRect(10, 20, 171, 22));
        transparancySlider->setMaximum(101);
        transparancySlider->setOrientation(Qt::Horizontal);
        transparancyText = new QLineEdit(groupBox_7);
        transparancyText->setObjectName(QStringLiteral("transparancyText"));
        transparancyText->setGeometry(QRect(190, 20, 41, 20));
        groupBox_8 = new QGroupBox(groupBox);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(260, 440, 151, 51));
        rateSelect = new QComboBox(groupBox_8);
        rateSelect->setObjectName(QStringLiteral("rateSelect"));
        rateSelect->setGeometry(QRect(10, 20, 131, 22));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 726, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "VROverlay", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        addOverlayButton->setText(QApplication::translate("MainWindow", "+", 0));
        remOverlayButton->setText(QApplication::translate("MainWindow", "-", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        label->setText(QApplication::translate("MainWindow", "Title", 0));
        label_2->setText(QApplication::translate("MainWindow", "Keybind", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "X/Y Tranlation", 0));
        xTranslateLeftBig->setText(QApplication::translate("MainWindow", "<<", 0));
        xTranslateLeft->setText(QApplication::translate("MainWindow", "<", 0));
        xTranslateRight->setText(QApplication::translate("MainWindow", ">", 0));
        xTranslateRightBig->setText(QApplication::translate("MainWindow", ">>", 0));
        yTranslateDown->setText(QApplication::translate("MainWindow", "V", 0));
        yTranslateDownBig->setText(QApplication::translate("MainWindow", "VV", 0));
        yTranslateUp->setText(QApplication::translate("MainWindow", "^", 0));
        yTranslateUpBig->setText(QApplication::translate("MainWindow", "^^", 0));
        label_3->setText(QApplication::translate("MainWindow", "X", 0));
        label_4->setText(QApplication::translate("MainWindow", "Y", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Z Translation", 0));
        label_6->setText(QApplication::translate("MainWindow", "Z", 0));
        zTranslateBack->setText(QApplication::translate("MainWindow", "^", 0));
        zTranslateBackBig->setText(QApplication::translate("MainWindow", "^^", 0));
        zTranslateForwardBig->setText(QApplication::translate("MainWindow", "VV", 0));
        zTranslateForward->setText(QApplication::translate("MainWindow", "V", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Rotation", 0));
        label_5->setText(QApplication::translate("MainWindow", "X:", 0));
        label_7->setText(QApplication::translate("MainWindow", "Y:", 0));
        label_8->setText(QApplication::translate("MainWindow", "Z:", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Scale", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Overlay Attachment", 0));
        trackingSelect->clear();
        trackingSelect->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Spacial", 0)
         << QApplication::translate("MainWindow", "HMD", 0)
         << QApplication::translate("MainWindow", "Controller 1", 0)
         << QApplication::translate("MainWindow", "Controller 2", 0)
        );
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Tansparancy", 0));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Refresh Rate", 0));
        rateSelect->clear();
        rateSelect->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1 FPS", 0)
         << QApplication::translate("MainWindow", "5 FPS", 0)
         << QApplication::translate("MainWindow", "10 FPS", 0)
         << QApplication::translate("MainWindow", "24 FPS", 0)
         << QApplication::translate("MainWindow", "30 FPS", 0)
         << QApplication::translate("MainWindow", "50 FPS", 0)
         << QApplication::translate("MainWindow", "60 FPS", 0)
        );
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
