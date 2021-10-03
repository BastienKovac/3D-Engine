#ifndef BSPLINE_H
#define BSPLINE_H

#include <QObject>

#include <vector>
#include <QPoint>
#include <algorithm>
#include <utils.h>

class BSpline : public QObject
{
    Q_OBJECT

public:
    BSpline(QObject *parent = nullptr, int degree = 3);
    ~BSpline();

    void setDegree(int degree);

    void setControlPoints(std::vector<QPointF> controlPoints);

    void addControlPoint(QPointF controlPoint);

    void removeControlPoint(QPointF controlPoint);

    void replaceControlPoint(QPointF oldV, QPointF newV);

    void setStartFromFirstPoint(bool newStartFromFirstPoint);

    void setFinishAtLastPoint(bool newFinishAtLastPoint);

    const std::vector<QPointF> &controlPoints() const;

    const std::vector<QPointF> &computedPoints() const;

    void setCloseSpline(bool newCloseSpline);

signals:
    void splineChanged();

private:

    int _degree;
    std::vector<QPointF> _controlPoints {};
    std::vector<QPointF> _computedPoints {};

    bool _startFromFirstPoint = false;
    bool _finishAtLastPoint = false;
    bool _closeSpline = false;

    std::vector<int> getNodalVector(std::vector<QPointF> controls);
    QPointF computeSplineFor(std::vector<QPointF> controls, std::vector<int> nodal, float u);
    std::vector<QPointF> computeBSpline(float step = 0.01);

    void refreshSpline();
};

#endif // BSPLINE_H
