#ifndef SPLINESURFACE_H
#define SPLINESURFACE_H

#include <vector>

#include <QPointF>

#include <Spline/bspline.h>
#include <3D/Geometry/geometry.h>

class SplineSurface : public Geometry
{
public:
    explicit SplineSurface();

private:

    std::vector<std::vector<QPointF>> _controlPolygon;

};

#endif // SPLINESURFACE_H
