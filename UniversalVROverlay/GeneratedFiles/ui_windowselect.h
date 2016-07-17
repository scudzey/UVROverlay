/********************************************************************************
** Form generated from reading UI file 'windowselect.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWSELECT_H
#define UI_WINDOWSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_WindowSelect
{
public:
    QGroupBox *groupBox;
    QComboBox *windowCombo;
    QPushButton *windowRefreshButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WindowSelect)
    {
        if (WindowSelect->objectName().isEmpty())
            WindowSelect->setObjectName(QStringLiteral("WindowSelect"));
        WindowSelect->resize(391, 131);
        groupBox = new QGroupBox(WindowSelect);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 371, 121));
        windowCombo = new QComboBox(groupBox);
        windowCombo->setObjectName(QStringLiteral("windowCombo"));
        windowCombo->setGeometry(QRect(20, 40, 251, 22));
        windowRefreshButton = new QPushButton(groupBox);
        windowRefreshButton->setObjectName(QStringLiteral("windowRefreshButton"));
        windowRefreshButton->setGeometry(QRect(280, 40, 75, 23));
        buttonBox = new QDialogButtonBox(groupBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(210, 90, 156, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(WindowSelect);

        QMetaObject::connectSlotsByName(WindowSelect);
    } // setupUi

    void retranslateUi(QDialog *WindowSelect)
    {
        WindowSelect->setWindowTitle(QApplication::translate("WindowSelect", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("WindowSelect", "Select Window", 0));
        windowRefreshButton->setText(QApplication::translate("WindowSelect", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowSelect: public Ui_WindowSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWSELECT_H
