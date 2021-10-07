#include "bspline.h"

BSpline::BSpline(QObject *parent, int degree) : QObject(parent)
{
    setDegree(degree);
}

BSpline::~BSpline()
{

}

void BSpline::setDegree(int degree)
{
    _degree = degree;
    refreshSpline();
}

void BSpline::setControlPoints(std::vector<QVector3D> controlPoints)
{
    _controlPoints.clear();
    _controlPoints.insert(_controlPoints.begin(), controlPoints.begin(), controlPoints.end());
    refreshSpline();
}

void BSpline::addControlPoint(QVector3D controlPoint)
{
    _controlPoints.push_back(controlPoint);
    refreshSpline();
}

void BSpline::removeControlPoint(QVector3D controlPoint)
{
    _controlPoints.erase(std::remove(_controlPoints.begin(), _controlPoints.end(), controlPoint), _controlPoints.end());
    refreshSpline();
}

void BSpline::replaceControlPoint(QVector3D oldV, QVector3D newV)
{
    std::replace(_controlPoints.begin(), _controlPoints.end(), oldV, newV);
    refreshSpline();
}

void BSpline::setStartFromFirstPoint(bool newStartFromFirstPoint)
{
    _startFromFirstPoint = newStartFromFirstPoint;
    refreshSpline();
}

void BSpline::setFinishAtLastPoint(bool newFinishAtLastPoint)
{
    _finishAtLastPoint = newFinishAtLastPoint;
    refreshSpline();
}

void BSpline::refreshSpline()
{
    _computedPoints = computeBSpline();
    emit splineChanged();
}

const std::vector<QVector3D> &BSpline::controlPoints() const
{
    return _controlPoints;
}

std::vector<int> BSpline::getNodalVector(std::vector<QVector3D> controls)
{
    int k = _degree + 1;
    int nbPoints = controls.size();

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

std::vector<QVector3D> BSpline::computeBSpline(float step)
{
    if ((unsigned int) _degree >= _controlPoints.size())
    {
        return {};
    }

    std::vector<QVector3D> controls;
    controls.insert(controls.end(), _controlPoints.begin(), _controlPoints.end());

    if (_closeSpline)
    {
        for (int i = 0 ; i < _degree + 1 ; ++i)
        {
            controls.push_back(_controlPoints[i]);
        }
    }

    std::vector<int> nodal = getNodalVector(controls);
    std::vector<float> uValues = arange<float>(nodal[_degree], nodal[controls.size()], step);

    std::vector<QVector3D> spline(uValues.size());

    for (unsigned int i = 0 ; i < uValues.size() ; ++i)
    {
        spline[i] = computeSplineFor(controls, nodal, uValues[i]);
    }

    return spline;
}

const std::vector<QVector3D> &BSpline::computedPoints() const
{
    return _computedPoints;
}

void BSpline::setCloseSpline(bool newCloseSpline)
{
    _closeSpline = newCloseSpline;
    refreshSpline();
}

QVector3D BSpline::computeSplineFor(std::vector<QVector3D> controls, std::vector<int> nodal, float u)
{
    int k = _degree + 1;

    // Step 1 : Compute relevant control points

    std::vector<QVector3D> Ptf(k);

    int offset = 0;
    while (u > nodal[k + offset])
    {
        offset++;
    }

    for (int i = 0 ; i < k ; ++i)
    {
        Ptf[i] = controls[i + offset];
    }

    // Step 2 : Compute spline value for u
    for (int i = 0 ; i < _degree ; ++i)
    {
        for (int j = 0 ; j < k - 1 - i ; ++j)
        {
            auto a = nodal[offset + k + j];
            auto b = nodal[offset + 1 + j + i];

            Ptf[j] = (a - u) / (a - b) * Ptf[j] + (u - b) / (a - b) * Ptf[j + 1];
        }
    }

    return Ptf[0];

}
