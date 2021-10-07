#include "splinesurface.h"

SplineSurface::SplineSurface() : Geometry(nullptr)
{

    _controlPolygon = {

        // 1st Spline
        {
            QVector3D(0., 0., 0.),
            QVector3D(2., 0., 0.),
            QVector3D(5., 0., 0.),
            QVector3D(7., 0., 0.),
            QVector3D(10., 0., 0.),
            QVector3D(12., 0., 0.)
        },

        // 2nd Spline
        {
            QVector3D(0., 2., 0.),
            QVector3D(2., 2., 1.),
            QVector3D(5., 2., 2.),
            QVector3D(7., 2., 3.),
            QVector3D(10., 2., 1.),
            QVector3D(12., 2., 0.)
        },

        // 3rd Spline
        {
            QVector3D(0., 5., 0.),
            QVector3D(2., 5., 1.),
            QVector3D(5., 5., 2.),
            QVector3D(7., 5., 3.),
            QVector3D(10., 5., 1.),
            QVector3D(12., 5., 0.)
        },

        // 4th Spline
        {
            QVector3D(0., 7., 0.),
            QVector3D(2., 7., -1.),
            QVector3D(5., 7., -2.),
            QVector3D(7., 7., -3.),
            QVector3D(10., 7., -1.),
            QVector3D(12., 7., 0.)
        },

        // 5th Spline
        {
            QVector3D(0., 10., 0.),
            QVector3D(2., 10., -1.),
            QVector3D(5., 10., -2.),
            QVector3D(7., 10., -3.),
            QVector3D(10., 10., -1.),
            QVector3D(12., 10., 0.)
        },

        // 6th Spline
        {
            QVector3D(0., 12., 0.),
            QVector3D(2., 12., 0.),
            QVector3D(5., 12., 0.),
            QVector3D(7., 12., 0.),
            QVector3D(10., 12., 0.),
            QVector3D(12., 12., 0.)
        }

    };

    computeSplineGeometry();
}

void SplineSurface::computeSplineGeometry()
{
    std::vector<GLfloat> vertices = {};
    std::vector<GLuint> indices = {};

    int surfaceDegree = 4;

    std::vector<BSpline *> directingSplines;
    for (unsigned int i = 0 ; i < _controlPolygon.size() ; ++i)
    {
        BSpline *spline = new BSpline(nullptr, surfaceDegree);
        spline->setControlPoints(_controlPolygon[i]);

        directingSplines.push_back(spline);
    }

    std::vector<BSpline *> generatingSplines;
    int nbPoints = directingSplines[0]->computedPoints().size();

    for (int i = 0 ; i < nbPoints  ; ++i)
    {
        std::vector<QVector3D> controls;

        for (unsigned int j = 0 ; j < directingSplines.size() ; ++j)
        {
            controls.push_back(directingSplines[j]->computedPoints()[i]);
        }

        BSpline *spline = new BSpline(nullptr, surfaceDegree);
        spline->setControlPoints(controls);

        generatingSplines.push_back(spline);
    }

    std::vector<std::vector<QVector3D>> points;

    for (BSpline *gen : generatingSplines)
    {
        points.push_back(gen->computedPoints());
    }

    for (unsigned int i = 0 ; i < points.size() ; ++i)
    {
        for (unsigned int j = 0 ; j < points[i].size() ; ++j)
        {
            vertices.push_back(points[i][j].x());
            vertices.push_back(points[i][j].y());
            vertices.push_back(points[i][j].z());
        }
    }

    for (unsigned int i = 0 ; i < points.size() - 2 ; ++i)
    {
        for (unsigned int j = 0 ; j < points[i].size() - 2 ; ++j)
        {
            indices.push_back(i * points[i].size() + 1 + j);
            indices.push_back(i * points[i].size() + 2 + j);
            indices.push_back((i + 1) * points[i].size() + 2 + j);

            indices.push_back(i * points[i].size() + 1 + j);
            indices.push_back((i + 1) * points[i].size() + 1 + j);
            indices.push_back((i + 1) * points[i].size() + 2 + j);
        }
    }

    setScene(vertices, indices);
}
