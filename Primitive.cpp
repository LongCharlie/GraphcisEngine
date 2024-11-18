#include "Primitive.h"

Primitive::Primitive()
{

}

Primitive::Primitive(QPen pen,Primitive::Type type, QVector<QPoint> args)
    : _type(type),_pen(pen)
{
    setArgs(args);
}

Primitive::Primitive(QPen pen,Primitive::Type type)
    : _type(type),_pen(pen)
{
}

QPoint Primitive::center() const
{
    return _center;
}

Primitive::Type Primitive::type() const
{
    return _type;
}

QPen Primitive::pen()
{
    return _pen;
}

QVector<QPoint> Primitive::args() const
{
    return _args;
}

QVector<QPoint> Primitive::points() const
{
    return _points;
}

bool Primitive::contain(QPoint pos){
    foreach (QPoint p, _points)
    {
        QPoint diff = p - pos;
        if (diff.x() * diff.x() + diff.y() * diff.y() < 25)
            return true;
    }
    return false;
}

bool Primitive::containArgs(QPoint pos){
    foreach (QPoint p, _args)
    {
        QPoint diff = p - pos;
        if (diff.x() * diff.x() + diff.y() * diff.y() < 25)
            return true;
    }
    return false;
}

void Primitive::setArgs(QVector<QPoint> args)
{
    _args = args;
    setPoints(args);
    int x = 0, y = 0;
    if(_type == ARC){
        QPoint p1 = args[0];
        QPoint p2 = args[1];

        int radius = args[2].x();

        QPoint midPoint((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);

        double dx = p2.x() - p1.x();
        double dy = p2.y() - p1.y();
        double dist = std::sqrt(dx * dx + dy * dy);

        double h = std::sqrt(radius * radius - (dist / 2) * (dist / 2));
        double cx = midPoint.x() - h * dy / dist;
        double cy = midPoint.y() + h * dx / dist;
        _center = QPoint(cx, cy);
        return;
    }

    for (QPoint p : args)
    {
        x += p.x();
        y += p.y();
    }
    int centerX = x / args.size();
    int centerY = y / args.size();
    _center = QPoint(centerX, centerY);
}

void Primitive::setPoints(QVector<QPoint> args) {
    switch (_type)
    {
    case Primitive::DashedLine:
        _points = BresenhamDrawDashedLine(args,5);
        break;
    case Primitive::Line:
        _points = BresenhamDrawLine(args);
        break;
    case Primitive::Line2:
        _points = MidpointDrawLine(args);
        break;
    case Primitive::Polygon:
        _points = drawPolygon(args);
        break;
    case Primitive::Circle:
        _points = drawCircle(args);
        break;
    case Primitive::Ellipse:
        _points = drawEllipse(args);
        break;
    case Primitive::BezierCurve:
        _points = drawBezierCurve(args);
        break;
    case Primitive::BSplineCurve:
        _points = drawBSplineCurve(args);
        break;
    case Primitive::ARC:
        _points=drawARC(args);
    default:
        break;
    }

}

void Primitive::addPoint(const QPoint &point)
{
    _args.append(point);
    setPoints(_args);
}

void Primitive::setPenWidth(int width)
{
    _pen.setWidth(width);
}

QVector<QPoint> Primitive::BresenhamDrawLine(QVector<QPoint> args)
{
    int x1 = args[0].x(), y1 = args[0].y(), x2 = args[1].x(), y2 = args[1].y();
    int dx = qAbs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = qAbs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e;
    QVector<QPoint> points;
    while (x1 != x2 || y1 != y2)
    {
        points.append(QPoint(x1, y1));
        e = err;
        if (e > -dx) { err -= dy; x1 += sx; }
        if (e < dy) { err += dx; y1 += sy; }
    }
    return points;
}

QVector<QPoint> Primitive::BresenhamDrawDashedLine(QVector<QPoint> args,int dashLength)
{
    int x1 = args[0].x(), y1 = args[0].y(), x2 = args[1].x(), y2 = args[1].y();
    int dx = qAbs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = qAbs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e;
    int dashCount = 0; // 用于控制虚线的实部和虚部的计数器

    QVector<QPoint> points;
    while (x1 != x2 || y1 != y2)
    {
        // 检查是否应该绘制实部
        if (dashCount < dashLength) {
            points.append(QPoint(x1, y1));
            dashCount++;
        } else {
            // 当实部长度达到 dashLength 时，跳过绘制，增加虚部长度
            dashCount++;
            if (dashCount >= dashLength * 2) {
                dashCount = 0; // 重置计数器以开始新的实部
            }
        }

        e = err;
        if (e > -dx) { err -= dy; x1 += sx; }
        if (e < dy) { err += dx; y1 += sy; }
    }
    // 确保最后一个点被绘制
    points.append(QPoint(x2, y2));

    return points;
}

QVector<QPoint> Primitive::MidpointDrawLine(QVector<QPoint> args)
{
    int x1 = args[0].x(), y1 = args[0].y(), x2 = args[1].x(), y2 = args[1].y();
    int dx = qAbs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = qAbs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx - dy, e2;
    QVector<QPoint> points;

    while (x1 != x2 || y1 != y2)
    {
        points.append(QPoint(x1, y1));
        e2 = 2 * err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }

    return points;
}

QVector<QPoint> Primitive::drawPolygon(QVector<QPoint> args)
{
    QVector<QPoint> points;
    int n = args.size();
    for (int i = 0; i < n; ++i)
    {
        if (i == n - 1)
            points.append(BresenhamDrawLine({args[n - 1], args[0]}));
        else
            points.append(BresenhamDrawLine({args[i], args[i + 1]}));
    }
    return points;
}

QVector<QPoint> Primitive::drawCircle(QVector<QPoint> args)
{
    QVector<QPoint> points;
    int cx = args[0].x(),cy = args[0].y(),r=qMin(qAbs(args[1].x()),qAbs(args[1].y()));
    auto Symmetrical=[&](int x,int y)
    {
        points.append(QPoint(cx+x,cy+y));
        points.append(QPoint(cx+x,cy-y));
        points.append(QPoint(cx-x,cy+y));
        points.append(QPoint(cx-x,cy-y));
        points.append(QPoint(cx+y,cy+x));
        points.append(QPoint(cx+y,cy-x));
        points.append(QPoint(cx-y,cy+x));
        points.append(QPoint(cx-y,cy-x));
    };
    int p=1-r,x=0,y=r;
    Symmetrical(x,y);
    while(x<y)
    {
        if(p<0)
        {
            p+=2*x+3;
        }
        else
        {
            y--;
            p+=2*(x-y)+5;
        }
        ++x;
        Symmetrical(x,y);
    }
    return points;
}

QVector<QPoint> Primitive::drawARC(QVector<QPoint> args)
{
    QVector<QPoint> points;

    if (args.size() != 3) {
        return points;
    }

    QPoint p1 = args[0];
    QPoint p2 = args[1];

    int radius = args[2].x();
    qDebug() << radius;

    QPoint midPoint((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);

    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 2 * radius) {
        return points;
    }

    double h = std::sqrt(radius * radius - (dist / 2) * (dist / 2));
    double cx = midPoint.x() - h * dy / dist;
    double cy = midPoint.y() + h * dx / dist;

    QPoint center(cx, cy);

    double startAngle = std::atan2(p1.y() - center.y(), p1.x() - center.x());
    double endAngle = std::atan2(p2.y() - center.y(), p2.x() - center.x());

    if (startAngle > endAngle) {
        endAngle += 2 * M_PI;
    }
    double angleDifference = endAngle - startAngle;
    if (angleDifference < 0) {
        angleDifference += 2 * M_PI;
    }


    int numSteps = static_cast<int>(radius * (endAngle - startAngle)); // 依据弧长和半径动态调整点数
    for (int i = 0; i <= numSteps; ++i) {
        double t = startAngle + (endAngle - startAngle) * i / numSteps;
        int x = center.x() + radius * std::cos(t);
        int y = center.y() + radius * std::sin(t);
        points.append(QPoint(x, y));
    }

    return points;

}

QVector<QPoint> Primitive::drawEllipse(QVector<QPoint> args)
{
    QVector<QPoint> points;
    int cx = args[0].x(), cy = args[0].y(), rx = qMax(qAbs(args[1].x()), 1), ry = qMax(qAbs(args[1].y()), 1);
    auto lambda = [&](int x, int y)
    {
        points.append(QPoint(cx + x, cy + y));
        points.append(QPoint(cx - x, cy + y));
        points.append(QPoint(cx - x, cy - y));
        points.append(QPoint(cx + x, cy - y));
    };
    if (rx >= ry)
    {
        int rx2 = rx * rx, ry2 = ry * ry;
        int p = ry2 - rx2 * ry + rx2, x = 0, y = ry;
        lambda(x, y);
        while (ry2 * x <= rx2 * y)
        {
            if (p < 0)
                p += ry2 * (3 + 2 * x);
            else
            {
                p += ry2 * (3 + 2 * x) + rx2 * (2 - 2 * y);
                --y;
            }
            ++x;
            lambda(x, y);
        }
        p = int(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1.0) * (y - 1.0) - 1.0 * rx2 * ry2);
        while (y >= 0)
        {
            if (p < 0)
            {
                p += ry2 * (2 + 2 * x) + rx2 * (3 - 2 * y);
                ++x;
            }
            else
            {
                p += rx2 * (3 - 2 * y);
            }
            --y;
            lambda(x, y);
        }
    }
    else
    {
        qSwap(args[0].rx(), args[0].ry());
        qSwap(args[1].rx(), args[1].ry());
        points = drawEllipse(args);
        for (auto& p : points)
            qSwap(p.rx(), p.ry());
    }
    return points;
}

QVector<QPoint> Primitive::drawBezierCurve(QVector<QPoint> args)
{
    int n = args.size() - 1; // Bézier 曲线的阶数
    QVector<QPoint> points;

    points.append(args[0]);

    int steps = 100;
    float dt = 1.0f / steps;

    for (int i = 1; i <= steps; ++i)
    {
        float t = i * dt;

        float x = 0.0f, y = 0.0f;
        for (int j = 0; j <= n; ++j)
        {
            int binomial = qBinomial(n, j);
            float bernstein = pow(1.0f - t, n - j) * pow(t, j);
            x += binomial * bernstein * args[j].x();
            y += binomial * bernstein * args[j].y();
        }

        int newX = static_cast<int>(x + 0.5);
        int newY = static_cast<int>(y + 0.5);

        if (!points.isEmpty())
        {
            QPoint lastPoint = points.last();
            int dx = newX - lastPoint.x();
            int dy = newY - lastPoint.y();
            int maxSteps = std::max(std::abs(dx), std::abs(dy));

            if (maxSteps > 0)
            {
                for (int step = 1; step <= maxSteps; ++step)
                {
                    int newXStep = lastPoint.x() + dx * step / maxSteps;
                    int newYStep = lastPoint.y() + dy * step / maxSteps;
                    points.append(QPoint(newXStep, newYStep));
                }
            }
        }

        // 添加当前点
        points.append(QPoint(newX, newY));
    }

    return points;
}

QVector<QPoint> Primitive::drawBSplineCurve(QVector<QPoint> args)
{
    int n = args.size(); // 控制点数量
    int degree = n - 1; // B 样条曲线的阶数
    int numKnots = n + degree + 1; // 节点向量的长度
    QVector<float> knots(numKnots, 0.0f); // 初始化节点向量为均匀分布

    for (int i = 0; i <= degree; ++i)
        knots[i] = 0.0f;
    for (int i = degree + 1; i < numKnots - degree - 1; ++i)
        knots[i] = (float)(i - degree - 1) / (n - 1);
    for (int i = numKnots - degree - 1; i < numKnots; ++i)
        knots[i] = 1.0f;

    QVector<QPoint> points;

    int steps = 100;
    float dt = 1.0f / steps;

    for (int i = 0; i <= steps; ++i)
    {
        float t = i * dt;

        float x = 0.0f, y = 0.0f;
        for (int j = 0; j < n; ++j)
        {
            float basisValue = computeBasisFunction(degree, knots, j, t);
            x += basisValue * args[j].x();
            y += basisValue * args[j].y();
        }

        int newX = static_cast<int>(x + 0.5);
        int newY = static_cast<int>(y + 0.5);

        if (!points.isEmpty())
        {
            QPoint lastPoint = points.last();
            int dx = newX - lastPoint.x();
            int dy = newY - lastPoint.y();
            int maxSteps = std::max(std::abs(dx), std::abs(dy));

            if (maxSteps > 0)
            {
                for (int step = 1; step <= maxSteps; ++step)
                {
                    int newXStep = lastPoint.x() + dx * step / maxSteps;
                    int newYStep = lastPoint.y() + dy * step / maxSteps;
                    points.append(QPoint(newXStep, newYStep));
                }
            }
        }

        points.append(QPoint(newX, newY));
    }

    return points;
}

QVector<QPoint> Primitive::translate(QPoint pos)
{
    QVector<QPoint> args = _args;
    if (_type == Circle || _type == Ellipse || _type == ARC)
        args[0] += pos;
    else if(_type == ARC){
        args[0] += pos;
        args[1] += pos;
    }
    else
        for (int i = 0; i < args.size(); ++i)
            args[i] += pos;
    return args;
}

QVector<QPoint> Primitive::translateArg(QPoint arg,QPoint pos)
{
    QVector<QPoint> args = _args;
    for (int i = 0; i < args.size(); ++i)
    {
        if (args[i] == arg)
        {
            args[i] += pos;
        }
    }
    return args;
}

QVector<QPoint> Primitive::rotate(QPoint center,qreal r)
{
    QVector<QPoint> args = _args;
    qreal cosr = qCos(r), sinr = qSin(r);
    int dx, dy;
    if (_type != Circle && _type != Ellipse){
        if(_type == ARC){
            for(int i = 0; i < 2; i++){
                dx = args[i].x() - _center.x();
                dy = args[i].y() - _center.y();
                args[i].rx() = int(_center.x() + dx * cosr - dy * sinr);
                args[i].ry() = int(_center.y() + dx * sinr + dy * cosr);
            }
            return args;
        }

        for (auto& arg : args)
        {
            dx = arg.x() - center.x();
            dy = arg.y() - center.y();
            arg.rx() = int(center.x() + dx * cosr - dy * sinr);
            arg.ry() = int(center.y() + dx * sinr + dy * cosr);
        }
    }
    return args;
}

QVector<QPoint> Primitive::scale(QPoint center,float s)
{
    QVector<QPoint> args = _args;
    if (_type == Circle || _type == Ellipse|| _type == ARC)
    {
        args[1] *= s;
    }
    else if(_type == ARC){
        args[0] = _center + (args[0] - _center) * s;
        args[1] = _center + (args[1] - _center) * s;
        args[2] = args[2] * s;
    }
    else
        for (auto& arg : args)
            arg = center + (arg - center) * s;
    return args;
}

QVector<QPoint> Primitive::CohenSutherlandClip(QPoint lt, QPoint rb)
{
    int l = qMin(lt.x(), rb.x()), r = qMax(lt.x(), rb.x()), t = qMin(lt.y(), rb.y()), b = qMax(lt.y(), rb.y());
    QVector<QPoint> args = _args;
    auto lambda = [=](int x, int y)
    {
        uint8_t code = 0;
        if (y < t)
            code |= 8;
        if (y > b)
            code |= 4;
        if (x > r)
            code |= 2;
        if (x < l)
            code |= 1;
        return code;
    };
    if (_type == Line || _type == Line2)
    {
        int x = 0, y = 0, x1 = args[0].x(), y1 = args[0].y(), x2 = args[1].x(), y2 = args[1].y();
        uint8_t code1 = lambda(x1, y1), code2 = lambda(x2, y2), code;
        while (code1 || code2)
        {
            if (code1 & code2)
                break;
            code = code1 ? code1 : code2;
            if (1 & code)
            {
                x = l;
                y = y1 + (y2 - y1) * (l - x1) / (x2 - x1);
            }
            else if (2 & code)
            {
                x = r;
                y = y1 + (y2 - y1) * (r - x1) / (x2 - x1);
            }
            else if (4 & code)
            {
                y = b;
                x = x1 + (x2 - x1) * (b - y1) / (y2 - y1);
            }
            else if (8 & code)
            {
                y = t;
                x = x1 + (x2 - x1) * (t - y1) / (y2 - y1);
            }
            if (code == code1)
            {
                x1 = x;
                y1 = y;
                code1 = lambda(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = lambda(x2, y2);
            }
        }
        args = {{x1, y1}, {x2, y2}};
    }
    return args;
}

QVector<QPoint> Primitive::MidpointSubdivisionClip(QPoint lt, QPoint rb) {
    if (lt.x() > rb.x() || lt.y() > rb.y()) {
        // 交换 lt 和 rb，使 lt 始终是左上角，rb 是右下角
        std::swap(lt, rb);
    }
    QVector<QPoint> clippedPoints;
    QPoint p1 = _points.front();  // 获取直线的起点
    QPoint p2 = _points.back();   // 获取直线的终点

    // QPoint lt = QPoint(qMin(window1.x(), window2.x()), qMin(window1.y(), window2.y())); // 窗口左上角
    // QPoint rb = QPoint(qMax(window1.x(), window2.x()), qMax(window1.y(), window2.y())); // 窗口右下角

    OutCode code1 = computeOutCode(p1, lt, rb);
    OutCode code2 = computeOutCode(p2, lt, rb);

    // 如果两点都在窗口内
    if (code1 == INSIDE && code2 == INSIDE) {
        clippedPoints.push_back(p1);
        clippedPoints.push_back(p2);
        return clippedPoints;
    }

    // 如果两点在窗口外且在同一侧
    if ((code1 & code2) != 0) {
        return clippedPoints; // 返回空
    }

    // 使用递归进行裁剪
    while (true) {
        if (code1 == INSIDE && code2 == INSIDE) {
            clippedPoints.push_back(p1);
            clippedPoints.push_back(p2);
            break;
        }

        // 选择一个端点进行裁剪
        QPoint p;
        OutCode codeOut = (code1 != INSIDE) ? code1 : code2;

        // 根据外码决定裁剪
        if (codeOut & TOP) {
            p.setX(p1.x() + (p2.x() - p1.x()) * (lt.y() - p1.y()) / (p2.y() - p1.y()));
            p.setY(lt.y());
        } else if (codeOut & BOTTOM) {
            p.setX(p1.x() + (p2.x() - p1.x()) * (rb.y() - p1.y()) / (p2.y() - p1.y()));
            p.setY(rb.y());
        } else if (codeOut & RIGHT) {
            p.setY(p1.y() + (p2.y() - p1.y()) * (rb.x() - p1.x()) / (p2.x() - p1.x()));
            p.setX(rb.x());
        } else if (codeOut & LEFT) {
            p.setY(p1.y() + (p2.y() - p1.y()) * (lt.x() - p1.x()) / (p2.x() - p1.x()));
            p.setX(lt.x());
        }

        // 更新端点和外码
        if (codeOut == code1) {
            p1 = p;
            code1 = computeOutCode(p1, lt, rb);
        } else {
            p2 = p;
            code2 = computeOutCode(p2, lt, rb);
        }
    }

    return clippedPoints;

}

void Primitive::setImage(const QImage image){
    _image = image;
    qDebug() << _image.width() <<_image.height();
}

QVector<QPoint> Primitive::fill(QImage& image) {
    QVector<QPoint> pointsFilled;
    QRgb targetColor = image.pixel(_args[0]);

    if (targetColor == _pen.color().rgb()) {
        // 如果种子点的颜色已经是填充颜色，则直接返回空集合
        return pointsFilled;
    }

    QStack<QPoint> stack;
    stack.push(_args[0]);

    while (!stack.isEmpty()) {
        QPoint current = stack.pop();
        int x = current.x();
        int y = current.y();

        // 检查当前点是否在图像范围内并且颜色匹配
        if (x >= 0 && x < image.width() && y >= 0 && y < image.height() &&
            image.pixel(x, y) == targetColor) {

            // 填充当前点颜色
            image.setPixel(x, y, _pen.color().rgb());
            pointsFilled.append(current);

            // 将四个方向的相邻点入栈，即上下左右四个方向
            stack.push(QPoint(x + 1, y)); // 右
            stack.push(QPoint(x - 1, y)); // 左
            stack.push(QPoint(x, y + 1)); // 下
            stack.push(QPoint(x, y - 1)); // 上
        }
    }
    return pointsFilled;
}



