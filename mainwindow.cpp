#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , state(Line)
    , primitive(nullptr)
    , pen(Qt::black, 2)
    , isBezierInProgress(false)
    , isPolygonInProgress(false)
    , isReferencePointSet(false)
    , isBSplineInProgress(false)
{
    ui->setupUi(this);
    image = QImage(this->size(), QImage::Format_RGBA64);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);
    painter.begin(&image);
    image.fill(Qt::white);
    foreach (Primitive *p, primitives)
    {
        painter.setPen(p->pen());
        painter.drawPoints(p->points());
        if (state == BezierControl && p->type() == Primitive::BezierCurve)
        {
            QPen controlPointPen(Qt::red, 5);
            painter.setPen(controlPointPen);
            painter.drawPoints(p->args());
        }
        if(p->type() == Primitive::Fill){
            painter.setPen(p->pen());
            p->setPoints(p->fill(image));
            painter.drawPoints(p->points());
        }

    }
    if (isReferencePointSet)
    {
        QPen refPointPen(Qt::blue, 5);
        painter.setPen(refPointPen);
        painter.drawPoint(referencePoint);
    }
    if (state == ClipLine && primitive)
    {
        painter.setPen(primitive->pen());
        painter.drawPoints(primitive->points());
    }
    if (state == ClipLine2 && primitive)
    {
        painter.setPen(primitive->pen());
        painter.drawPoints(primitive->points());
    }
    if (state == Polygon)
    {
        painter.setBrush(QBrush(Qt::black));
        foreach (QPoint p, points)
            painter.drawEllipse(p, 3, 3);
    }
    painter.end();
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= 10;
    pos.ry() -= 20;
    points.append(pos);
    switch (state)
    {
    case DashedLine:
        primitive = new Primitive(pen, Primitive::DashedLine,{pos, pos});
        primitives.append(primitive);
        break;
    case Line:
        primitive = new Primitive(pen, Primitive::Line,{pos, pos});
        primitives.append(primitive);
        break;
    case Line2:
        primitive = new Primitive(pen, Primitive::Line2,{pos, pos});
        primitives.append(primitive);
        break;
    case Triangle:
        primitive = new Primitive(pen, Primitive::Polygon,{pos, pos, pos});
        primitives.append(primitive);
        break;
    case Circle:
        primitive = new Primitive(pen, Primitive::Circle,{pos, QPoint(0, 0)});
        primitives.append(primitive);
        break;
    case Ellipse:
        primitive = new Primitive(pen, Primitive::Ellipse,{pos, QPoint(0, 0)});
        primitives.append(primitive);
        break;
    case Polygon:
        if (!isPolygonInProgress)
        {
            primitive = new Primitive(pen, Primitive::Polygon, {pos});
            primitives.append(primitive);
            isPolygonInProgress = true;
        }
        else
        {
            primitive->addPoint(pos);
        }
        break;
    case Rectangle:
        primitive = new Primitive(pen, Primitive::Polygon,{pos, pos, pos, pos});
        primitives.append(primitive);
        break;
    case BezierCurve:
        if (!isBezierInProgress)
        {
            primitive = new Primitive(pen, Primitive::BezierCurve, {pos});
            primitives.append(primitive);
            isBezierInProgress = true;
        }
        else
        {
            primitive->addPoint(pos);
        }
        break;
    case BezierControl:
        foreach (Primitive *p, primitives)
        {
            if (p->type() == Primitive::BezierCurve && p->containArgs(points[0]))
            {
                const QVector<QPoint>& args = p->args();
                for (int i = 0; i < args.size(); ++i)
                {
                    if (qAbs(pos.x() - args[i].x()) < 5 &&
                        qAbs(pos.y() - args[i].y()) < 5)
                    {
                        selectedControlPoint = args[i];
                        primitive = p;
                        break;
                    }
                }
            }
        }
        break;
    case BSplineCurve:
        if (!isBezierInProgress)
        {
            primitive = new Primitive(pen, Primitive::BSplineCurve, {pos});
            primitives.append(primitive);
            isBSplineInProgress = true;
        }
        else
        {
            primitive->addPoint(pos);
        }
        break;
    case ARC:
        primitive = new Primitive(pen, Primitive::ARC, {pos, pos, {ui->radiusARC->value(),0}});
        primitives.append(primitive);
        break;
    case Translate:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case ClipLine:
        primitive = new Primitive(QPen(Qt::black, 1), Primitive::Polygon,{pos, pos, pos, pos});
        break;
    case ClipLine2:
        primitive = new Primitive(QPen(Qt::black, 1), Primitive::Polygon,{pos, pos, pos, pos});
        break;
    case Fill:
        primitive = new Primitive(QPen(Qt::green),Primitive::Fill);
        primitives.append(primitive);
        primitive->setImage(image);
        primitive->setArgs({pos});
        break;
    case ZoomIn:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case ZoomOut:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case Delete:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    case SetRP:
        isReferencePointSet = true;
        referencePoint = pos;
        break;
    case DeleteRP:
        break;
    case Rotate:
        primitive = nullptr;
        foreach (Primitive *p, primitives)
            if (p->contain(points[0]))
            {
                primitive = p;
                break;
            }
        break;
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= 10;
    pos.ry() -= 20;
    QVector<QPoint> args;
    switch (state)
    {
    case DashedLine:
        primitive->setArgs({points[0], pos});
        break;
    case Line:
        primitive->setArgs({points[0], pos});
        break;
    case Line2:
        primitive->setArgs({points[0], pos});
        break;
    case Triangle:
        primitive->setArgs({pos,{(points[0].x() + pos.x()) / 2, points[0].y()},{points[0].x(), pos.y()}});
        break;
    case Circle:
        if (qAbs(pos.x() - points[0].x()) < qAbs(pos.y() - points[0].y()))
            if (pos.ry() > points[0].y())
                pos.ry() = points[0].y() + qAbs(pos.rx() - points[0].x());
            else
                pos.ry() = points[0].y() - qAbs(pos.rx() - points[0].x());
        else
            if (pos.rx() > points[0].x())
                pos.rx() = points[0].x() + qAbs(pos.ry() - points[0].y());
            else
                pos.rx() = points[0].x() - qAbs(pos.ry() - points[0].y());
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Ellipse:
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Polygon:
        break;
    case Rectangle:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        break;
    case BezierCurve:
        break;
    case BezierControl:
        if (!primitive)
            break;
        args = primitive->translateArg(selectedControlPoint,pos - points[0]);
        primitive->setPoints(args);
        break;
    case BSplineCurve:
        break;
    case ARC:
        primitive->setArgs({points[0], pos,{ui->radiusARC->value(),0}});
        break;
    case Translate:
        if (!primitive)
            break;
        args = primitive->translate(pos - points[0]);
        primitive->setPoints(args);
        break;
    case ClipLine:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        foreach (Primitive *p, primitives)
        {
            args = primitive->args();
            args = p->CohenSutherlandClip(args[0], args[2]);
            p->setPoints(args);
        }
        break;
    case ClipLine2:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        break;
    case Fill:
        break;
    case ZoomIn:
        break;
    case ZoomOut:
        break;
    case Delete:
        break;
    case SetRP:
        break;
    case DeleteRP:
        break;
    case Rotate:
        if (!primitive)
            break;
        if(isReferencePointSet){
            QPoint a = points[0] - referencePoint, b = pos - referencePoint;
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(referencePoint,qAsin(product / aNorm / bNorm));
        }
        else{
            QPoint a = points[0] - primitive->center(), b = pos - primitive->center();
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(primitive->center(),qAsin(product / aNorm / bNorm));
        }
        primitive->setPoints(args);
        break;
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= 10;
    pos.ry() -= 20;
    QVector<QPoint> args;
    switch (state)
    {
    case DashedLine:
        primitive->setArgs({points[0], pos});
        break;
    case Line:
        primitive->setArgs({points[0], pos});
        break;
    case Line2:
        primitive->setArgs({points[0], pos});
        break;
    case Triangle:
        primitive->setArgs({pos,{(points[0].x() + pos.x()) / 2, points[0].y()},{points[0].x(), pos.y()}});
        break;
    case Circle:
        if (qAbs(pos.x() - points[0].x()) < qAbs(pos.y() - points[0].y()))
            if (pos.ry() > points[0].y())
                pos.ry() = points[0].y() + qAbs(pos.rx() - points[0].x());
            else
                pos.ry() = points[0].y() - qAbs(pos.rx() - points[0].x());
        else
            if (pos.rx() > points[0].x())
                pos.rx() = points[0].x() + qAbs(pos.ry() - points[0].y());
            else
                pos.rx() = points[0].x() - qAbs(pos.ry() - points[0].y());
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Ellipse:
        primitive->setArgs({(pos + points[0]) / 2,(pos - points[0]) / 2});
        break;
    case Polygon:
        primitive->setArgs(points);
        break;
    case Rectangle:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        break;
    case BezierCurve:
        primitive->setArgs(points);
        break;
    case BezierControl:
        if (!primitive)
            break;
        args = primitive->translateArg(selectedControlPoint,pos - points[0]);
        primitive->setArgs(args);
        break;
    case BSplineCurve:
        primitive->setArgs(points);
        break;
    case ARC:
        primitive->setArgs({points[0], pos,{ui->radiusARC->value(),0}});
        break;
    case Translate:
        if (!primitive)
            break;
        args = primitive->translate(pos - points[0]);
        primitive->setArgs(args);
        break;
    case ClipLine:
        primitive->setArgs({points[0],{points[0].x(), pos.y()},pos,{pos.x(), points[0].y()}});
        foreach (Primitive *p, primitives)
        {
            args = primitive->args();
            args = p->CohenSutherlandClip(args[0], args[2]);
            p->setArgs(args);
        }
        delete primitive;
        primitive = nullptr;
        break;
    case Fill:
        break;
    case ClipLine2:
        points.append(pos);
        for (int i = 0; i < primitives.size(); ) {
            Primitive *p = primitives[i];
            if (p->type() == Primitive::Line || p->type() == Primitive::Line2) {
                QVector<QPoint> clippedPoints = p->MidpointSubdivisionClip(points.first(), points.back());
                if (!clippedPoints.empty()) {
                    p->setPoints(clippedPoints);
                    ++i;
                }
                else {
                    delete p;
                    primitives.removeAt(i);
                }
            } else {
                ++i;
            }
        }
        delete primitive;
        primitive = nullptr;
        break;
    case ZoomIn:
        if (!primitive) break;
        if(isReferencePointSet)
        {
            args = primitive->scale(referencePoint,1.2);
        }
        else
        {
            args = primitive->scale(primitive->center(),1.2);
        }
        primitive->setArgs(args);
        break;
    case ZoomOut:
        if (!primitive) break;
        if(isReferencePointSet)
        {
            args = primitive->scale(referencePoint,0.9);
        }
        else
        {
            args = primitive->scale(primitive->center(),0.9);
        }
        primitive->setArgs(args);
        break;
    case Delete:
        primitives.removeAll(primitive);
        delete primitive;
        break;
    case SetRP:
        break;
    case DeleteRP:
        break;
    case Rotate:
        if (!primitive)
            break;
        if(isReferencePointSet){
            QPoint a = points[0] - referencePoint, b = pos - referencePoint;
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(referencePoint,qAsin(product / aNorm / bNorm));
        }
        else{
            QPoint a = points[0] - primitive->center(), b = pos - primitive->center();
            qreal product = a.x() * b.y() - a.y() * b.x();
            qreal aNorm = qSqrt(qreal(a.x() * a.x() + a.y() * a.y()));
            qreal bNorm = qSqrt(qreal(b.x() * b.x() + b.y() * b.y()));
            args = primitive->rotate(primitive->center(),qAsin(product / aNorm / bNorm));
        }
        primitive->setArgs(args);
        break;
    }
    update();
    if (state != BezierCurve && state != Polygon)
        points.clear();
}

void MainWindow::on_Circle_clicked()
{
    state=Circle;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Line_clicked()
{
    state=Line;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Line2_clicked()
{
    state=Line2;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_BezierCurve_clicked()
{
    state=BezierCurve;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Translate_clicked()
{
    state=Translate;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Rotate_clicked()
{
    state=Rotate;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Clip_clicked()
{
    state=ClipLine;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Clip2_clicked()
{
    state=ClipLine2;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Polygon_clicked()
{
    state=Polygon;
    isBezierInProgress=false;
    points.clear();
}


void MainWindow::on_Ellipse_clicked()
{
    state=Ellipse;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_ARC_clicked()
{
    state=ARC;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_ZoomIn_clicked()
{
    state=ZoomIn;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_ZoomOut_clicked()
{
    state=ZoomOut;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Triangle_clicked()
{
    state=Triangle;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_Rectangle_clicked()
{
    state=Rectangle;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Delete_clicked()
{
    state = Delete;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_BezierControl_clicked()
{
    state = BezierControl;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_SetRP_clicked()
{
    state = SetRP;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


void MainWindow::on_DeleteRP_clicked()
{
    state = DeleteRP;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    isReferencePointSet = false;
    points.clear();
}


void MainWindow::on_BSplineCurve_clicked()
{
    state = BSplineCurve;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_Fill_clicked()
{
    state = Fill;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    pen.setWidth(arg1);
}


void MainWindow::on_DashLine_clicked()
{
    state = DashedLine;
    isBezierInProgress=false;
    isPolygonInProgress =false;
    points.clear();
}


