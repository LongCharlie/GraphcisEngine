/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *Circle;
    QPushButton *Line;
    QPushButton *Rectangle;
    QPushButton *BezierCurve;
    QPushButton *Translate;
    QPushButton *Rotate;
    QPushButton *Clip;
    QPushButton *Polygon;
    QPushButton *Ellipse;
    QPushButton *ARC;
    QPushButton *ZoomIn;
    QPushButton *ZoomOut;
    QPushButton *Triangle;
    QPushButton *Delete;
    QPushButton *BezierControl;
    QPushButton *SetRP;
    QPushButton *DeleteRP;
    QPushButton *BSplineCurve;
    QPushButton *Clip2;
    QPushButton *Fill;
    QPushButton *Line2;
    QSpinBox *spinBox;
    QPushButton *DashLine;
    QSpinBox *radiusARC;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 40, 980, 540));
        Circle = new QPushButton(centralwidget);
        Circle->setObjectName("Circle");
        Circle->setGeometry(QRect(10, 20, 20, 20));
        Line = new QPushButton(centralwidget);
        Line->setObjectName("Line");
        Line->setGeometry(QRect(10, 0, 80, 20));
        Rectangle = new QPushButton(centralwidget);
        Rectangle->setObjectName("Rectangle");
        Rectangle->setGeometry(QRect(100, 20, 35, 20));
        BezierCurve = new QPushButton(centralwidget);
        BezierCurve->setObjectName("BezierCurve");
        BezierCurve->setGeometry(QRect(170, 0, 55, 20));
        Translate = new QPushButton(centralwidget);
        Translate->setObjectName("Translate");
        Translate->setGeometry(QRect(640, 0, 40, 20));
        Rotate = new QPushButton(centralwidget);
        Rotate->setObjectName("Rotate");
        Rotate->setGeometry(QRect(680, 0, 40, 20));
        Clip = new QPushButton(centralwidget);
        Clip->setObjectName("Clip");
        Clip->setGeometry(QRect(640, 20, 80, 20));
        Polygon = new QPushButton(centralwidget);
        Polygon->setObjectName("Polygon");
        Polygon->setGeometry(QRect(135, 20, 45, 20));
        Ellipse = new QPushButton(centralwidget);
        Ellipse->setObjectName("Ellipse");
        Ellipse->setGeometry(QRect(60, 20, 35, 20));
        ARC = new QPushButton(centralwidget);
        ARC->setObjectName("ARC");
        ARC->setGeometry(QRect(30, 20, 35, 20));
        ZoomIn = new QPushButton(centralwidget);
        ZoomIn->setObjectName("ZoomIn");
        ZoomIn->setGeometry(QRect(720, 0, 40, 20));
        ZoomOut = new QPushButton(centralwidget);
        ZoomOut->setObjectName("ZoomOut");
        ZoomOut->setGeometry(QRect(760, 0, 40, 20));
        Triangle = new QPushButton(centralwidget);
        Triangle->setObjectName("Triangle");
        Triangle->setGeometry(QRect(180, 20, 45, 20));
        Delete = new QPushButton(centralwidget);
        Delete->setObjectName("Delete");
        Delete->setGeometry(QRect(760, 20, 40, 20));
        BezierControl = new QPushButton(centralwidget);
        BezierControl->setObjectName("BezierControl");
        BezierControl->setGeometry(QRect(560, 0, 80, 20));
        SetRP = new QPushButton(centralwidget);
        SetRP->setObjectName("SetRP");
        SetRP->setGeometry(QRect(800, 0, 80, 20));
        DeleteRP = new QPushButton(centralwidget);
        DeleteRP->setObjectName("DeleteRP");
        DeleteRP->setGeometry(QRect(800, 20, 80, 20));
        BSplineCurve = new QPushButton(centralwidget);
        BSplineCurve->setObjectName("BSplineCurve");
        BSplineCurve->setGeometry(QRect(225, 0, 45, 20));
        Clip2 = new QPushButton(centralwidget);
        Clip2->setObjectName("Clip2");
        Clip2->setGeometry(QRect(560, 20, 80, 20));
        Fill = new QPushButton(centralwidget);
        Fill->setObjectName("Fill");
        Fill->setGeometry(QRect(720, 20, 40, 20));
        Line2 = new QPushButton(centralwidget);
        Line2->setObjectName("Line2");
        Line2->setGeometry(QRect(90, 0, 80, 20));
        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(300, 0, 40, 20));
        DashLine = new QPushButton(centralwidget);
        DashLine->setObjectName("DashLine");
        DashLine->setGeometry(QRect(225, 20, 40, 20));
        radiusARC = new QSpinBox(centralwidget);
        radiusARC->setObjectName("radiusARC");
        radiusARC->setGeometry(QRect(310, 20, 42, 22));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QString());
        Circle->setText(QCoreApplication::translate("MainWindow", "\345\234\206", nullptr));
        Line->setText(QCoreApplication::translate("MainWindow", "\347\272\277(Bresenham)", nullptr));
        Rectangle->setText(QCoreApplication::translate("MainWindow", "\347\237\251\345\275\242", nullptr));
        BezierCurve->setText(QCoreApplication::translate("MainWindow", "Bezier", nullptr));
        Translate->setText(QCoreApplication::translate("MainWindow", "\345\271\263\347\247\273", nullptr));
        Rotate->setText(QCoreApplication::translate("MainWindow", "\346\227\213\350\275\254", nullptr));
        Clip->setText(QCoreApplication::translate("MainWindow", "\350\243\201\345\211\252\347\233\264\347\272\2772", nullptr));
        Polygon->setText(QCoreApplication::translate("MainWindow", "\345\244\232\350\276\271\345\275\242", nullptr));
        Ellipse->setText(QCoreApplication::translate("MainWindow", "\346\244\255\345\234\206", nullptr));
        ARC->setText(QCoreApplication::translate("MainWindow", "\345\234\206\345\274\247", nullptr));
        ZoomIn->setText(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
        ZoomOut->setText(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
        Triangle->setText(QCoreApplication::translate("MainWindow", "\344\270\211\350\247\222\345\275\242", nullptr));
        Delete->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        BezierControl->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\224\271Bezier", nullptr));
        SetRP->setText(QCoreApplication::translate("MainWindow", "\345\217\202\350\200\203\347\202\271\350\256\276\347\275\256", nullptr));
        DeleteRP->setText(QCoreApplication::translate("MainWindow", "\345\217\202\350\200\203\347\202\271\345\210\240\351\231\244", nullptr));
        BSplineCurve->setText(QCoreApplication::translate("MainWindow", "B\346\240\267\346\235\241", nullptr));
        Clip2->setText(QCoreApplication::translate("MainWindow", "\350\243\201\345\211\252\347\233\264\347\272\2771", nullptr));
        Fill->setText(QCoreApplication::translate("MainWindow", "\345\241\253\345\205\205", nullptr));
        Line2->setText(QCoreApplication::translate("MainWindow", "\347\272\277(\344\270\255\347\202\271)", nullptr));
        DashLine->setText(QCoreApplication::translate("MainWindow", "\350\231\232\347\272\277", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
