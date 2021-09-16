#ifndef BSPLINE_H
#define BSPLINE_H

#include <vector>
#include <QPoint>
#include <algorithm>
#include <utils.h>

class BSpline
{
public:
    BSpline(int degree = 3);

    void setDegree(int degree);

    void setControlPoints(std::vector<QPointF> controlPoints);

    void addControlPoint(QPointF controlPoint);

    void removeControlPoint(QPointF controlPoint);

    void replaceControlPoint(QPointF oldV, QPointF newV);

    void setStartFromFirstPoint(bool newStartFromFirstPoint);

    void setFinishAtLastPoint(bool newFinishAtLastPoint);

    const std::vector<QPointF> &controlPoints() const;

    const std::vector<QPointF> &computedPoints() const;

private:

    int _degree;
    std::vector<QPointF> _controlPoints {};
    std::vector<QPointF> _computedPoints {};

    bool _startFromFirstPoint = false;
    bool _finishAtLastPoint = false;

    std::vector<int> getNodalVector();
    QPointF computeSplineFor(float u);
    std::vector<QPointF> computeBSpline(float step = 0.01);

};

#endif // BSPLINE_H
