#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include <GL/gl.h>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Tools/Utils/MeshCheckerT.hh>

#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>

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
    virtual std::vector<GLfloat> texCoords();

    void subdivide();
    void simplify(long triangleTarget);

    unsigned int textureId() const;
    void setTextureId(unsigned int textureId);

    bool hasTexture();

    long getNumberOfTriangles();

    void setVertices(const std::vector<GLfloat>& vertices);
    void setWeights(const std::vector<GLfloat>& weights);
    inline const std::vector<GLfloat>& weights() const { return _bonesWeights; }
    inline const std::vector<GLfloat>& initialPose() const { return _initialPose; }

private:
    // OpenMesh geometry
    Mesh _mesh;
    std::vector<Mesh::VertexHandle> _vertexHandles;

    // Geometry cache
    bool _dirty = true;

    unsigned int _textureId;

    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;
    std::vector<GLfloat> _texCoords;
    std::vector<GLfloat> _initialPose;
    std::vector<GLfloat> _bonesWeights;

    void updateExistingVertex(const Mesh::VertexHandle& v_h);
    void addMidPoint(const Mesh::EdgeHandle& e_h);
    void splitEdge(const Mesh::EdgeHandle& e_h);
    void splitFace(const Mesh::FaceHandle& f_h);
    void cutCorner(const Mesh::HalfedgeHandle& h_e);

    void refreshGeometryCache();

    // Subdivision

    OpenMesh::VPropHandleT<Mesh::Point> _vp_pos;
    OpenMesh::EPropHandleT<Mesh::Point> _ep_pos;

    // Simplification
    struct Quadric
    {
        GLfloat q00 = 0.0;
        GLfloat q01 = 0.0;
        GLfloat q02 = 0.0;
        GLfloat q03 = 0.0;

        GLfloat q11 = 0.0;
        GLfloat q12 = 0.0;
        GLfloat q13 = 0.0;

        GLfloat q22 = 0.0;
        GLfloat q23 = 0.0;

        GLfloat q33 = 0.0;
    };

    float norm2(glm::mat3 matrix)
    {
        float res = 0.0;
        for (int i = 0 ; i < 2 ; ++i)
        {
            for (int j = 0 ; j < 2 ; ++j)
            {
                res += matrix[i][j] * matrix[i][j];
            }
        }

        return sqrt(res);
    };

    OpenMesh::FPropHandleT<glm::vec4> _planeEquations;
    OpenMesh::VPropHandleT<Geometry::Quadric*> _vertexErrorProp;
    OpenMesh::EPropHandleT<GLfloat> _edgeEstimatedErrorProp;

    std::vector<Mesh::EdgeHandle> _sortedEdges;

    void initializeSimplification();
    void computeVertexErrors();
    void computeErrorForVertex(Mesh::VertexHandle vertex);
    void computeEdge(OpenMesh::SmartEdgeHandle edge);
};

#endif // GEOMETRY_H
