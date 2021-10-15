#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <iostream>
#include <string>

#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

class Geometry : public QObject
{
    Q_OBJECT

public:
    Geometry(QObject *parent = nullptr);
    ~Geometry();

    virtual void setScene(std::string fileName);
    virtual std::vector<GLfloat> vertices();
    virtual std::vector<GLfloat> normals();
    virtual std::vector<GLuint> indices();

    void subdivide();

private:
    // OpenMesh geometry
    Mesh _mesh;
    std::vector<Mesh::VertexHandle> _vertexHandles;

    // Geometry cache
    bool _dirty = true;

    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

    void refreshGeometryCache();
};

#endif // GEOMETRY_H
