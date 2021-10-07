#ifndef BSPLINE_H
#define BSPLINE_H

#include <QObject>

#include <vector>
#include <algorithm>
#include <utils.h>
#include <optional>

#include <QVector3D>

class BSpline : public QObject
{
    Q_OBJECT

public:
    BSpline(QObject *parent = nullptr, int degree = 3);
    ~BSpline();

    void setDegree(int degree);

    void setControlPoints(std::vector<QVector3D> controlPoints);

    void addControlPoint(QVector3D controlPoint);

    void removeControlPoint(QVector3D controlPoint);

    void replaceControlPoint(QVector3D oldV, QVector3D newV);

    void setStartFromFirstPoint(bool newStartFromFirstPoint);

    void setFinishAtLastPoint(bool newFinishAtLastPoint);

    const std::vector<QVector3D> &controlPoints() const;

    const std::vector<QVector3D> &computedPoints() const;

    void setCloseSpline(bool newCloseSpline);

signals:
    void splineChanged();

private:

    int _degree;
    std::vector<QVector3D> _controlPoints {};
    std::vector<QVector3D> _computedPoints {};

    bool _startFromFirstPoint = false;
    bool _finishAtLastPoint = false;
    bool _closeSpline = false;

    std::vector<int> getNodalVector(std::vector<QVector3D> controls);
    QVector3D computeSplineFor(std::vector<QVector3D> controls, std::vector<int> nodal, float u);
    std::vector<QVector3D> computeBSpline(float step = 0.01);

    std::vector<QVector3D> getControlPoints();
    float getMinU();
    float getMaxU();

    void refreshSpline();
};

#endif // BSPLINE_H
