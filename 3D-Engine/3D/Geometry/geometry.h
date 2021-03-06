#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
#include <OpenMesh/Core/Utils/PropertyManager.hh>

class Geometry : QObject
{
    Q_OBJECT

public:
    Geometry(QObject *parent = nullptr);
    ~Geometry();

    void setScene(std::vector<GLfloat> vertices, std::vector<GLuint> indices);

    std::vector<GLfloat> vertices();
    std::vector<GLfloat> normals();
    std::vector<GLuint> indices();

signals:
    void geometryRefreshed();

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
