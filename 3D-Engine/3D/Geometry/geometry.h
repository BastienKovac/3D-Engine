#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>

#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Tools/Utils/MeshCheckerT.hh>

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

    void updateExistingVertex(const Mesh::VertexHandle& v_h);
    void addMidPoint(const Mesh::EdgeHandle& e_h);
    void splitEdge(const Mesh::EdgeHandle& e_h);
    void splitFace(const Mesh::FaceHandle& f_h);
    void cutCorner(const Mesh::HalfedgeHandle& h_e);

    void refreshGeometryCache();

    OpenMesh::VPropHandleT<Mesh::Point> _vp_pos;
    OpenMesh::EPropHandleT<Mesh::Point> _ep_pos;
};

#endif // GEOMETRY_H
