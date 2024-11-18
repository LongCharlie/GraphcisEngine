#ifndef MATHMODULE_H
#define MATHMODULE_H

#include <algorithm>
#include <cmath>
#include <QVector>
#include <QPoint>

enum OutCode {
    INSIDE = 0,  // 0000
    LEFT = 1,    // 0001
    RIGHT = 2,   // 0010
    BOTTOM = 4,  // 0100
    TOP = 8      // 1000
};

int qBinomial(int n, int k);
float computeBasisFunction(int degree, const QVector<float>& knots, int j, float t);
QPointF computeIntersection(const QPointF &A, const QPointF &B, const QPointF &P1, const QPointF &P2);
OutCode computeOutCode(QPoint p, QPoint lt, QPoint rb);

#endif // MATHMODULE_H
