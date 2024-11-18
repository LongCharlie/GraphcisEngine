#include "MathModule.h"

int qBinomial(int n, int k)
{
    if (k < 0 || k > n)
        return 0;

    if (k == 0 || k == n)
        return 1;

    k = std::min(k, n - k);
    int result = 1;
    for (int i = 1; i <= k; ++i)
    {
        result *= n - (k - i);
        result /= i;
    }
    return result;
}

float computeBasisFunction(int degree, const QVector<float>& knots, int j, float t)
{
    if (degree == 0)
    {
        return (knots[j] <= t && t < knots[j + 1]) ? 1.0f : 0.0f;
    }

    float result = 0.0f;
    float denom1 = knots[j + degree] - knots[j];
    if (denom1 > 0.0f)
    {
        result += (t - knots[j]) / denom1 * computeBasisFunction(degree - 1, knots, j, t);
    }
    float denom2 = knots[j + degree + 1] - knots[j + 1];
    if (denom2 > 0.0f)
    {
        result += (knots[j + degree + 1] - t) / denom2 * computeBasisFunction(degree - 1, knots, j + 1, t);
    }

    return result;
}

OutCode computeOutCode(QPoint p, QPoint lt, QPoint rb) {

    OutCode code = INSIDE;

    if (p.x() < lt.x()) { // 点在窗口左侧
        code = static_cast<OutCode>(code | LEFT);
    } else if (p.x() > rb.x()) { // 点在窗口右侧
        code = static_cast<OutCode>(code | RIGHT);
    }
    if (p.y() < lt.y()) { // 点在窗口上方
        code = static_cast<OutCode>(code | TOP);
    } else if (p.y() > rb.y()) { // 点在窗口下方
        code = static_cast<OutCode>(code | BOTTOM);
    }
    return code;
}
