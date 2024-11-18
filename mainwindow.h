#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QString>
#include <QUrl>
#include "Primitive.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_Circle_clicked();
    void on_Line_clicked();
    void on_BezierCurve_clicked();
    void on_Translate_clicked();
    void on_Rotate_clicked();
    void on_Clip_clicked();
    void on_Polygon_clicked();
    void on_Ellipse_clicked();
    void on_ARC_clicked();
    void on_ZoomIn_clicked();
    void on_ZoomOut_clicked();
    void on_Triangle_clicked();
    void on_Rectangle_clicked();
    void on_Delete_clicked();
    void on_BezierControl_clicked();
    void on_SetRP_clicked();
    void on_DeleteRP_clicked();
    void on_BSplineCurve_clicked();
    void on_Clip2_clicked();
    void on_Fill_clicked();
    void on_Line2_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_DashLine_clicked();


private:
    Ui::MainWindow *ui;
    enum State {DashedLine, Line, Line2, Triangle, Circle, Ellipse, Polygon, Rectangle, BSplineCurve,
                 BezierCurve ,BezierControl, ARC ,Translate, ClipLine, ClipLine2, Fill, ZoomIn, ZoomOut, Delete, Rotate, SetRP, DeleteRP} state;//程序状态
    QVector<QPoint> points;          //鼠标点击的位置
    QList<Primitive *> primitives;   //已经绘制的图元列表
    Primitive *primitive;            //当前正在操作的图元
    QImage image;                    //画布
    QPainter painter;                //画笔
    QPen pen;                        //笔的颜色和大小
    bool isBezierInProgress;   //Bezier是否正在绘制
    bool isPolygonInProgress;  //Polygon是否正在绘制
    QPoint selectedControlPoint;     //存储被选中的控制点(用于Bezier曲线控制)
    QPoint referencePoint;           //旋转 平移参考点
    bool isReferencePointSet;
    bool isBSplineInProgress;
};


#endif // MAINWINDOW_H
