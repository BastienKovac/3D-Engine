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

    void setDegree(int degree)
    {
        _degree = degree;
    }

    void setControlPoints(std::vector<QPointF> controlPoints)
    {
        _controlPoints.clear();
        _controlPoints.insert(_controlPoints.begin(), controlPoints.begin(), controlPoints.end());
    }

    void addControlPoint(QPointF controlPoint)
    {
        _controlPoints.push_back(controlPoint);
    }

    void removeControlPoint(QPointF controlPoint)
    {
        _controlPoints.erase(std::remove(_controlPoints.begin(), _controlPoints.end(), controlPoint), _controlPoints.end());
    }

    void replaceControlPoint(QPointF oldV, QPointF newV)
    {
        std::replace(_controlPoints.begin(), _controlPoints.end(), oldV, newV);
    }

    void setStartFromFirstPoint(bool newStartFromFirstPoint);

    void setFinishAtLastPoint(bool newFinishAtLastPoint);

    const std::vector<QPointF> &controlPoints() const;

    std::vector<QPointF> computeBSpline(float step = 0.01);

private:

    int _degree;
    std::vector<QPointF> _controlPoints {};

    bool _startFromFirstPoint = false;
    bool _finishAtLastPoint = false;

    std::vector<int> getNodalVector();
    QPointF computeSplineFor(float u);

};

#endif // BSPLINE_H
