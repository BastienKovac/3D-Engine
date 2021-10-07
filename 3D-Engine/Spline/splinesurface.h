#ifndef SPLINESURFACE_H
#define SPLINESURFACE_H

#include <vector>
#include <optional>
#include <algorithm>

#include <QVector3D>

#include <Spline/bspline.h>
#include <3D/Geometry/geometry.h>
#include <utils.h>

class SplineSurface : public Geometry
{
public:
    explicit SplineSurface();

private:

    std::vector<std::vector<QVector3D>> _controlPolygon;

    void computeSplineGeometry();

};

#endif // SPLINESURFACE_H
