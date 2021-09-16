#include "bspline.h"

BSpline::BSpline(int degree)
{
    setDegree(degree);
}

void BSpline::setDegree(int degree)
{
    _degree = degree;

    _computedPoints = computeBSpline();
}

void BSpline::setControlPoints(std::vector<QPointF> controlPoints)
{
    _controlPoints.clear();
    _controlPoints.insert(_controlPoints.begin(), controlPoints.begin(), controlPoints.end());

    _computedPoints = computeBSpline();
}

void BSpline::addControlPoint(QPointF controlPoint)
{
    _controlPoints.push_back(controlPoint);

    _computedPoints = computeBSpline();
}

void BSpline::removeControlPoint(QPointF controlPoint)
{
    _controlPoints.erase(std::remove(_controlPoints.begin(), _controlPoints.end(), controlPoint), _controlPoints.end());

    _computedPoints = computeBSpline();
}

void BSpline::replaceControlPoint(QPointF oldV, QPointF newV)
{
    std::replace(_controlPoints.begin(), _controlPoints.end(), oldV, newV);

    _computedPoints = computeBSpline();
}

void BSpline::setStartFromFirstPoint(bool newStartFromFirstPoint)
{
    _startFromFirstPoint = newStartFromFirstPoint;

    _computedPoints = computeBSpline();
}

void BSpline::setFinishAtLastPoint(bool newFinishAtLastPoint)
{
    _finishAtLastPoint = newFinishAtLastPoint;

    _computedPoints = computeBSpline();
}

const std::vector<QPointF> &BSpline::controlPoints() const
{
    return _controlPoints;
}

std::vector<int> BSpline::getNodalVector()
{
    int k = _degree + 1;
    int nbPoints = _controlPoints.size();

    std::vector<int> nodal = arange<int>(1, k + nbPoints);

    if (_startFromFirstPoint)
    {
        for (int i = 0 ; i < _degree ; ++i)
        {
            nodal[i] = nodal[_degree];
        }
    }

    if (_finishAtLastPoint)
    {
        for (unsigned int i = nbPoints + 1 ; i < nodal.size() ; i++)
        {
            nodal[i] = nodal[nbPoints];
        }
    }

    return nodal;
}

std::vector<QPointF> BSpline::computeBSpline(float step)
{

    if ((unsigned int) _degree >= _controlPoints.size())
    {
        return {};
    }

    std::vector<int> nodal = getNodalVector();
    std::vector<float> uValues = arange<float>(nodal[_degree], nodal[_controlPoints.size()], step);

    std::vector<QPointF> spline(uValues.size());

    for (unsigned int i = 0 ; i < uValues.size() ; ++i)
    {
        spline[i] = computeSplineFor(uValues[i]);
    }

    return spline;
}

const std::vector<QPointF> &BSpline::computedPoints() const
{
    return _computedPoints;
}

QPointF BSpline::computeSplineFor(float u)
{
    int k = _degree + 1;
    std::vector<int> nodal = getNodalVector();

    // Step 1 : Compute relevant control points

    std::vector<QPointF> Ptf(k);

    int offset = 0;
    while (u > nodal[k + offset])
    {
        offset++;
    }

    for (int i = 0 ; i < k ; ++i)
    {
        Ptf[i] = _controlPoints[i + offset];
    }

    // Step 2 : Compute spline value for u
    for (int i = 0 ; i < _degree ; ++i)
    {
        for (int j = 0 ; j < k - 1 - i ; ++j)
        {
            Ptf[j] = (nodal[offset + k + j] - u) / (nodal[offset + k + j] - nodal[offset + 1 + j]) * Ptf[j] +
                     (u - nodal[offset + 1 + j]) / (nodal[offset + k + j] - nodal[offset + 1 + j]) * Ptf[j + 1];
        }
    }

    return Ptf[0];

}
