#ifndef MULTIPLEGEOMETRY_H
#define MULTIPLEGEOMETRY_H

#include <QObject>

#include <3D/Geometry/geometry.h>

#include <vector>
#include <experimental/filesystem>
#include <iostream>

class MultipleGeometry : public Geometry
{
public:
    MultipleGeometry(Geometry *parent = nullptr);
    ~MultipleGeometry();

    virtual void setScene(std::string fileName) override;
    virtual std::vector<GLfloat> vertices() override;
    virtual std::vector<GLfloat> normals() override;
    virtual std::vector<GLuint> indices() override;

private:
    void addObject(std::string fileName);

    std::vector<Geometry *> _objects;

    void clearObjects();
};

#endif // MULTIPLEGEOMETRY_H
