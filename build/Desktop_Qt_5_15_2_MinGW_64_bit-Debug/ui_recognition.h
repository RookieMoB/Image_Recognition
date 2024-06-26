/********************************************************************************
** Form generated from reading UI file 'recognition.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECOGNITION_H
#define UI_RECOGNITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Recognition
{
public:
    QLabel *label;
    QPushButton *pushButton;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *Recognition)
    {
        if (Recognition->objectName().isEmpty())
            Recognition->setObjectName(QString::fromUtf8("Recognition"));
        Recognition->resize(800, 600);
        label = new QLabel(Recognition);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 10, 421, 311));
        label->setScaledContents(true);
        pushButton = new QPushButton(Recognition);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(80, 400, 371, 101));
        textBrowser = new QTextBrowser(Recognition);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(510, 260, 250, 300));
        textBrowser->setMinimumSize(QSize(250, 300));
        textBrowser->setMaximumSize(QSize(250, 300));
        QFont font;
        font.setPointSize(20);
        textBrowser->setFont(font);

        retranslateUi(Recognition);

        QMetaObject::connectSlotsByName(Recognition);
    } // setupUi

    void retranslateUi(QWidget *Recognition)
    {
        Recognition->setWindowTitle(QCoreApplication::translate("Recognition", "Recognition", nullptr));
        label->setText(QCoreApplication::translate("Recognition", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("Recognition", "\346\213\215\347\205\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Recognition: public Ui_Recognition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECOGNITION_H
