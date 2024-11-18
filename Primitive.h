#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include<QStack>
#include <QPoint>
#include <QPen>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include <QPixmap>
#include "MathModule.h"

class Primitive {
public:
    enum Type { Line, Line2, DashedLine, Polygon, Circle, Ellipse, BezierCurve, ARC, BSplineCurve,Fill };
    Primitive();
    Primitive(QPen pen,Type type, QVector<QPoint> args);
    Primitive(QPen pen,Type type);
    QPoint center() const;
    Type type() const;
    QPen pen();
    QVector<QPoint> args() const;
    QVector<QPoint> points() const;
    bool contain(QPoint p);
    bool containArgs(QPoint p);
    void setArgs(QVector<QPoint> args);
    void setPoints(QVector<QPoint> points);
    void addPoint(const QPoint &point);
    void setPenWidth(int width);
    QVector<QPoint> BresenhamDrawDashedLine(QVector<QPoint> args,int dashLength);
    static QVector<QPoint> BresenhamDrawLine(QVector<QPoint> args);        // 绘制直线（Bresenham算法）
    static QVector<QPoint> MidpointDrawLine(QVector<QPoint> args);// 绘制直线（中点算法）
    static QVector<QPoint> drawPolygon(QVector<QPoint> args);     // 绘制多边形
    static QVector<QPoint> drawCircle(QVector<QPoint> args);	  // 绘制圆形
    static QVector<QPoint> drawEllipse(QVector<QPoint> args);	  // 绘制椭圆
    static QVector<QPoint> drawBezierCurve(QVector<QPoint> args); // 绘制Bezier曲线
    static QVector<QPoint> drawBSplineCurve(QVector<QPoint> args);// 绘制B样条曲线
    static QVector<QPoint> drawARC(QVector<QPoint> args);         // 绘制圆弧
    QVector<QPoint> translate(QPoint pos);		                   // 平移
    QVector<QPoint> translateArg(QPoint arg,QPoint pos);
    QVector<QPoint> rotate(QPoint center,qreal r);			       // 旋转
    QVector<QPoint> scale(QPoint center,float s);				   // 缩放
    QVector<QPoint> CohenSutherlandClip(QPoint lt, QPoint rb);	   // 裁剪（CohenSuther）
    QVector<QPoint> MidpointSubdivisionClip(QPoint lt, QPoint rb); // 裁剪（中点）
    void setImage(const QImage image);
    QVector<QPoint> fill(QImage& image);

private:
    Type _type;
    QPoint _center;
    QVector<QPoint> _args;
    QVector<QPoint> _points;
    QPen _pen;
    QImage _image;
};


#endif // PRIMITIVE_H
