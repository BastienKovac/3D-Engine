#include "geometry.h"

Geometry::Geometry(QObject *parent) : QObject(parent)
{

}

Geometry::~Geometry()
{

}

void Geometry::setScene(std::string fileName)
{
    OpenMesh::IO::Options readOptions;
    readOptions += OpenMesh::IO::Options::VertexTexCoord;
    readOptions += OpenMesh::IO::Options::VertexNormal;

    if (!OpenMesh::IO::read_mesh(_mesh, fileName, readOptions))
    {
      std::cerr << "Couldn't read file " << fileName << std::endl;
      exit(1);
    }
    _dirty = true;
}

void Geometry::refreshGeometryCache()
{
    _vertices.clear();
    _indices.clear();
    _normals.clear();
    _texCoords.clear();

    // Start index on 1
    int v_index = 0;

    auto indexProperty = OpenMesh::makeTemporaryProperty<OpenMesh::VertexHandle, int>(_mesh, "v_index");

    // Refresh normals
    if (!_mesh.has_vertex_normals())
    {
        _mesh.request_vertex_normals();
        // we need face normals to update the vertex normals
        _mesh.request_face_normals();
        // let the mesh update the normals
        _mesh.update_normals();
    }

    for (const auto& vh : _mesh.vertices())
    {
        indexProperty[vh] = v_index++;

        // Fill vertex cache
        auto vertex = _mesh.point(vh);

        _vertices.push_back(vertex[0]);
        _vertices.push_back(vertex[1]);
        _vertices.push_back(vertex[2]);

        // Fill normals cache
        auto normal = _mesh.normal(vh);

        _normals.push_back(normal[0]);
        _normals.push_back(normal[1]);
        _normals.push_back(normal[2]);

        // Fill texCoords cache
        if (_mesh.has_vertex_texcoords2D())
        {
            auto texCoord = _mesh.texcoord2D(vh);

            _texCoords.push_back(texCoord[0]);
            _texCoords.push_back(texCoord[1]);
        }
    }

    for (Mesh::FaceIter f_it = _mesh.faces_begin() ; f_it != _mesh.faces_end() ; ++f_it)
    {
        for (Mesh::FaceVertexIter fv_it = _mesh.fv_iter(*f_it) ; fv_it.is_valid() ; ++fv_it)
        {
            // Fill indices cache
            _indices.push_back(indexProperty[*fv_it]);
        }
    }
    _mesh.release_vertex_normals();
    _mesh.garbage_collection();

    _initialPose = vertices();

    _dirty = false;
}

void Geometry::initializeSimplification()
{
    _sortedEdges.clear();

    // Init vertex errors
    computeVertexErrors();

    // Init edge errors and replacements
    for (auto edge : _mesh.edges())
    {
        computeEdge(edge);
        _sortedEdges.push_back(edge);
    }

    // Sort by ascending error
    std::sort(_sortedEdges.begin(), _sortedEdges.end(), [this](Mesh::EdgeHandle a, Mesh::EdgeHandle b) {
        return _mesh.property(_edgeEstimatedErrorProp, a) > _mesh.property(_edgeEstimatedErrorProp, b);
    });
}

void Geometry::setWeights(const std::vector<GLfloat> &weights)
{
    _bonesWeights = weights;
}

void Geometry::setVertices(const std::vector<GLfloat> &vertices)
{
    _vertices = vertices;
}

std::vector<GLfloat> Geometry::vertices()
{
    if (_dirty)
    {
        refreshGeometryCache();
    }

    return _vertices;
}

std::vector<GLfloat> Geometry::normals()
{
    if (_dirty)
    {
        refreshGeometryCache();
    }
    return _normals;
}

std::vector<GLuint> Geometry::indices()
{
    if (_dirty)
    {
        refreshGeometryCache();
    }
    return _indices;
}

std::vector<GLfloat> Geometry::texCoords()
{
    if (_dirty)
    {
        refreshGeometryCache();
    }
    return _texCoords;
}

void Geometry::subdivide()
{
    _mesh.add_property(_vp_pos);
    _mesh.add_property(_ep_pos);

    // Compute even vertex changes
    for (Mesh::VertexIter v_it = _mesh.vertices_begin() ; v_it != _mesh.vertices_end() ; ++v_it)
    {
        updateExistingVertex(*v_it);
    }

    // Compute new odd vertex
    for (Mesh::EdgeIter e_it = _mesh.edges_begin() ; e_it != _mesh.edges_end() ; ++e_it)
    {
        addMidPoint(*e_it);
    }

    // Split edges and faces
    for (auto eh : _mesh.edges())
    {
        splitEdge(eh);
    }

    for (auto fh : _mesh.faces())
    {
        splitFace(fh);
    }

    // Apply changes
    for (Mesh::VertexIter v_it = _mesh.vertices_begin() ; v_it != _mesh.vertices_end() ; ++v_it)
    {
        auto newPosition = _mesh.property(_vp_pos, *v_it);
        _mesh.set_point(*v_it, newPosition);
    }

    _mesh.remove_property(_vp_pos);
    _mesh.remove_property(_ep_pos);

    assert(OpenMesh::Utils::MeshCheckerT<Mesh>(_mesh).check());

    _dirty = true;
}

void Geometry::computeErrorForVertex(Mesh::VertexHandle vertex)
{
    Quadric* quadric = new Quadric();

    for (Mesh::VertexFaceIter vf_it = _mesh.vf_begin(vertex) ; vf_it.is_valid() ; ++vf_it)
    {
        glm::vec4 plane = _mesh.property(_planeEquations, *vf_it);

        quadric->q00 += plane[0] * plane[0];
        quadric->q01 += plane[0] * plane[1];
        quadric->q02 += plane[0] * plane[2];
        quadric->q03 += plane[0] * plane[3];

        quadric->q11 += plane[1] * plane[1];
        quadric->q12 += plane[1] * plane[2];
        quadric->q13 += plane[1] * plane[3];

        quadric->q22 += plane[2] * plane[2];
        quadric->q23 += plane[2] * plane[3];

        quadric->q33 += plane[3] * plane[3];
    }

    _mesh.property(_vertexErrorProp, vertex) = quadric;
}

void Geometry::computeVertexErrors()
{
    for (Mesh::FaceIter f_it = _mesh.faces_begin() ; f_it != _mesh.faces_end() ; ++f_it)
    {
        auto vertices = f_it->vertices();
        auto vectorVertices = vertices.to_vector();

        Mesh::Point a = _mesh.point(vectorVertices[0]);
        Mesh::Point b = _mesh.point(vectorVertices[1]);
        Mesh::Point c = _mesh.point(vectorVertices[2]);

        glm::vec3 AB = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
        glm::vec3 AC = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};

        auto cross = glm::cross(AB, AC);
        auto norm = cross / glm::l1Norm(cross);

        float d = - a[0] * norm[0] - a[1] * norm[1] - a[2] * norm[2];

        glm::vec4 equation = {norm[0], norm[1], norm[2], d};

        _mesh.property(_planeEquations, *f_it) = equation;
    }

    for (auto vertex : _mesh.vertices())
    {
        computeErrorForVertex(vertex);
    }
}

void Geometry::computeEdge(OpenMesh::SmartEdgeHandle edge)
{
    Quadric* q1 = _mesh.property(_vertexErrorProp, edge.v0());
    Quadric* q2 = _mesh.property(_vertexErrorProp, edge.v1());

    glm::mat4 mat1 = {
        q1->q00, q1->q01, q1->q02, q1->q03,
        q1->q01, q1->q11, q1->q12, q1->q13,
        q1->q02, q1->q12, q1->q22, q1->q23,
        q1->q03, q1->q13, q1->q23, q1->q33
    };

    glm::mat4 mat2 = {
        q2->q00, q2->q01, q2->q02, q2->q03,
        q2->q01, q2->q11, q2->q12, q2->q13,
        q2->q02, q2->q12, q2->q22, q2->q23,
        q2->q03, q2->q13, q2->q23, q2->q33
    };

    glm::mat4 matPrime = mat1 + mat2;

    glm::mat3 A = {
        matPrime[0][0], matPrime[0][1], matPrime[0][2],
        matPrime[0][1], matPrime[1][1], matPrime[1][2],
        matPrime[0][2], matPrime[1][2], matPrime[2][2]
    };

    glm::mat3 xMat = {
        -matPrime[0][3], matPrime[0][1], matPrime[0][2],
        -matPrime[1][3], matPrime[1][1], matPrime[1][2],
        -matPrime[2][3], matPrime[1][2], matPrime[2][2]
    };

    glm::mat3 yMat = {
        matPrime[0][0], -matPrime[0][3], matPrime[0][2],
        matPrime[0][1], -matPrime[1][3], matPrime[1][2],
        matPrime[0][2], -matPrime[2][3], matPrime[2][2]
    };

    glm::mat3 zMat = {
        matPrime[0][0], matPrime[0][1], -matPrime[0][3],
        matPrime[0][1], matPrime[1][1], -matPrime[1][3],
        matPrime[0][2], matPrime[1][2], -matPrime[2][3]
    };

    float x = norm2(xMat) / norm2(A);
    float y = norm2(yMat) / norm2(A);
    float z = norm2(zMat) / norm2(A);

    glm::vec4 v = {x, y, z, 1};

    float error = glm::dot(v, matPrime * v);

    _mesh.property(_edgeEstimatedErrorProp, edge) = error;
}

void Geometry::simplify(long triangleTarget)
{
    if (_mesh.n_faces() <= (size_t) triangleTarget)
    {
        // No work
        return;
    }

    // Code using OpenMesh built-in decimation (Custom code below doesn't work)

    typedef OpenMesh::Decimater::DecimaterT<Mesh> Decimater;
    typedef OpenMesh::Decimater::ModQuadricT<Mesh>::Handle HModQuadric;

    Decimater decimater(_mesh);
    HModQuadric hModQuadric;

    decimater.add(hModQuadric);

    decimater.module(hModQuadric).unset_max_err();
    decimater.initialize();
    decimater.decimate_to_faces(0, triangleTarget);

    _mesh.garbage_collection();

    /*

    _mesh.add_property(_planeEquations);
    _mesh.add_property(_vertexErrorProp);
    _mesh.add_property(_edgeEstimatedErrorProp);

    _mesh.request_face_status();
    _mesh.request_edge_status();
    _mesh.request_vertex_status();

    initializeSimplification();

    // Apply edge collapse while needed
    long i = 0;

    while (_mesh.n_faces() > (size_t) triangleTarget)
    {
        if (++i % 100 == 0)
        {
            std::cout << "Iteration " << i << ", current number of triangles: " << _mesh.n_faces() << " / " << triangleTarget << std::endl;
        }

        Mesh::EdgeHandle candidate = _sortedEdges[0];
        Mesh::HalfedgeHandle he1 = _mesh.halfedge_handle(candidate, 0);

        // Recompute affected vertices and faces
        Mesh::VertexHandle to = _mesh.to_vertex_handle(he1);

        computeErrorForVertex(to);
        for (Mesh::VertexFaceIter vf_it = _mesh.vf_iter(to) ; vf_it.is_valid() ; ++vf_it)
        {
            for (Mesh::FaceEdgeIter fe_it = _mesh.fe_iter(*vf_it) ; fe_it.is_valid() ; ++fe_it)
            {
                computeEdge(*fe_it);
            }
        }

        // Do collapse
        _mesh.collapse(he1);

        // Recompute errors for new edges
        for (Mesh::VertexEdgeIter ve_it = _mesh.ve_iter(to) ; ve_it.is_valid() ; ++ve_it)
        {
            computeEdge(*ve_it);
        }

        // Repopulate sorted list
        _sortedEdges.clear();
        for (auto edge : _mesh.edges())
        {
            _sortedEdges.push_back(edge);
        }

        // Sort by ascending error
        std::sort(_sortedEdges.begin(), _sortedEdges.end(), [this](Mesh::EdgeHandle a, Mesh::EdgeHandle b) {
            return _mesh.property(_edgeEstimatedErrorProp, a) > _mesh.property(_edgeEstimatedErrorProp, b);
        });

        // Clean-up
        _mesh.garbage_collection();
    }

    _mesh.remove_property(_planeEquations);
    _mesh.remove_property(_vertexErrorProp);
    _mesh.remove_property(_edgeEstimatedErrorProp);*/

    _dirty = true;
}

unsigned int Geometry::textureId() const
{
    return _textureId;
}

void Geometry::setTextureId(unsigned int textureId)
{
    _textureId = textureId;
}

bool Geometry::hasTexture()
{
    return _mesh.has_vertex_texcoords2D();
}

long Geometry::getNumberOfTriangles()
{
    return _mesh.n_faces();
}

void Geometry::updateExistingVertex(const Mesh::VertexHandle &v_h)
{
    Mesh::Point position(0.0, 0.0, 0.0);

    if (_mesh.is_boundary(v_h))
    {
        Mesh::HalfedgeHandle halfEdge, previousHalfEdge;

        halfEdge = _mesh.halfedge_handle(v_h);
        previousHalfEdge = _mesh.halfedge_handle(v_h);

        if (halfEdge.is_valid())
        {
            Mesh::VertexHandle to = _mesh.to_vertex_handle(halfEdge);
            Mesh::VertexHandle from = _mesh.from_vertex_handle(previousHalfEdge);

            position = _mesh.point(v_h);
            position *= 6;
            position += _mesh.point(to);
            position += _mesh.point(from);
            position /= 8;
        }
        else
        {
            return;
        }
    }
    else
    {
        unsigned int valence = 0;

        for (Mesh::VertexVertexIter vv_it = _mesh.vv_iter(v_h) ; vv_it.is_valid() ; ++vv_it)
        {
            ++valence;
            position += _mesh.point(*vv_it);
        }

        double inv_valence = 1 / (double) valence;
        double t = 3.0 + 2.0 * std::cos(2.0 * M_PI * inv_valence);
        double alpha = (40.0 - t * t) / 64.0;

        position *= (alpha * inv_valence);
        position += (1.0 - alpha) * _mesh.point(v_h);
    }

    // Store new position in property
    _mesh.property(_vp_pos, v_h) = position;
}

void Geometry::addMidPoint(const Mesh::EdgeHandle &e_h)
{
    Mesh::HalfedgeHandle halfOne = _mesh.halfedge_handle(e_h, 0);
    Mesh::HalfedgeHandle halfTwo = _mesh.halfedge_handle(e_h, 1);

    Mesh::Point position(_mesh.point(_mesh.to_vertex_handle(halfOne)));
    position += _mesh.point(_mesh.to_vertex_handle(halfTwo));

    if (_mesh.is_boundary(e_h))
    {
        position *= 0.5;
    }
    else
    {
        position *= 3.0;
        position += _mesh.point(_mesh.to_vertex_handle(_mesh.next_halfedge_handle(halfOne)));
        position += _mesh.point(_mesh.to_vertex_handle(_mesh.next_halfedge_handle(halfTwo)));
        position /= 8.0;
    }

    _mesh.property(_ep_pos, e_h) = position;
}

void Geometry::splitEdge(const Mesh::EdgeHandle &e_h)
{
    Mesh::HalfedgeHandle halfOne = _mesh.halfedge_handle(e_h, 0);
    Mesh::HalfedgeHandle halfTwo = _mesh.halfedge_handle(e_h, 1);

    Mesh::HalfedgeHandle newHalfEdge;
    Mesh::HalfedgeHandle newOpposingHalfEdge;
    Mesh::HalfedgeHandle tmpHalfEdge;

    Mesh::VertexHandle currentVertex(_mesh.to_vertex_handle(halfOne));
    Mesh::Point midPoint(_mesh.point(_mesh.to_vertex_handle(halfOne)));

    midPoint += _mesh.point(_mesh.to_vertex_handle(halfTwo));
    midPoint *= 0.5;

    // Create a temporary new vertex at the mid point, will be moved towards the computed position for given edge
    Mesh::VertexHandle newVertex = _mesh.new_vertex(midPoint);
    _mesh.property(_vp_pos, newVertex) = _mesh.property(_ep_pos, e_h);

    // Re-link mesh entities by iterating over the relevant half-edges
    if (_mesh.is_boundary(e_h))
    {
        tmpHalfEdge = halfOne;
        while (_mesh.next_halfedge_handle(tmpHalfEdge) != halfTwo)
        {
            tmpHalfEdge = _mesh.opposite_halfedge_handle(_mesh.next_halfedge_handle(tmpHalfEdge));
        }
    }
    else
    {
        tmpHalfEdge = _mesh.next_halfedge_handle(halfTwo);
        while(_mesh.next_halfedge_handle(tmpHalfEdge) != halfTwo)
        {
           tmpHalfEdge = _mesh.next_halfedge_handle(tmpHalfEdge);
        }
    }

    newHalfEdge = _mesh.new_edge(newVertex, currentVertex);
    newOpposingHalfEdge = _mesh.opposite_halfedge_handle(newHalfEdge);
    _mesh.set_vertex_handle(halfOne, newVertex);

    _mesh.set_next_halfedge_handle(tmpHalfEdge, newOpposingHalfEdge);
    _mesh.set_next_halfedge_handle(newHalfEdge, _mesh.next_halfedge_handle(halfOne));
    _mesh.set_next_halfedge_handle(halfOne, newHalfEdge);
    _mesh.set_next_halfedge_handle(newOpposingHalfEdge, halfTwo);

    if (_mesh.face_handle(halfTwo).is_valid())
    {
        _mesh.set_face_handle(newOpposingHalfEdge, _mesh.face_handle(halfTwo));
        _mesh.set_halfedge_handle(_mesh.face_handle(newOpposingHalfEdge), newOpposingHalfEdge);
    }

    _mesh.set_face_handle(newHalfEdge, _mesh.face_handle(halfOne));
    _mesh.set_halfedge_handle(newVertex, newHalfEdge);

    if (!_mesh.is_boundary(halfOne))
    {
        _mesh.set_halfedge_handle(_mesh.face_handle(halfOne), halfOne);
    }

    _mesh.set_halfedge_handle(currentVertex, newOpposingHalfEdge);

    _mesh.adjust_outgoing_halfedge(newVertex);
    _mesh.adjust_outgoing_halfedge(currentVertex);
}

void Geometry::splitFace(const Mesh::FaceHandle &f_h)
{
    Mesh::HalfedgeHandle halfEdgeOne(_mesh.halfedge_handle(f_h));
    Mesh::HalfedgeHandle halfEdgeTwo(_mesh.next_halfedge_handle(_mesh.next_halfedge_handle(halfEdgeOne)));
    Mesh::HalfedgeHandle halfEdgeThree(_mesh.next_halfedge_handle(_mesh.next_halfedge_handle(halfEdgeTwo)));

    cutCorner(halfEdgeOne);
    cutCorner(halfEdgeTwo);
    cutCorner(halfEdgeThree);
}

void Geometry::cutCorner(const Mesh::HalfedgeHandle &h_e)
{
    // Define Halfedge Handles
    Mesh::HalfedgeHandle halfEdgeOne(h_e);
    Mesh::HalfedgeHandle halfEdgeFive(halfEdgeOne);
    Mesh::HalfedgeHandle halfEdgeSix(_mesh.next_halfedge_handle(halfEdgeOne));

    // Cycle around the polygon to find correct Halfedge
    while (_mesh.next_halfedge_handle(_mesh.next_halfedge_handle(halfEdgeFive)) != halfEdgeOne)
    {
        halfEdgeFive = _mesh.next_halfedge_handle(halfEdgeFive);
    }

    Mesh::VertexHandle vertexOne = _mesh.to_vertex_handle(halfEdgeOne);
    Mesh::VertexHandle vertexTwo = _mesh.to_vertex_handle(halfEdgeFive);

    Mesh::HalfedgeHandle halfEdgeTwo(_mesh.next_halfedge_handle(halfEdgeFive));
    // Half-Edge three => New half-edge
    Mesh::HalfedgeHandle halfEdgeThree(_mesh.new_edge(vertexOne, vertexTwo));
    Mesh::HalfedgeHandle halfEdgeFour(_mesh.opposite_halfedge_handle(halfEdgeThree));

    // Old and new Face
    Mesh::FaceHandle oldFace(_mesh.face_handle(halfEdgeSix));
    Mesh::FaceHandle newFace(_mesh.new_face());

    // Re-Set Handles around old Face
    _mesh.set_next_halfedge_handle(halfEdgeFour, halfEdgeSix);
    _mesh.set_next_halfedge_handle(halfEdgeFive, halfEdgeFour);

    _mesh.set_face_handle(halfEdgeFour, oldFace);
    _mesh.set_face_handle(halfEdgeFive, oldFace);
    _mesh.set_face_handle(halfEdgeSix, oldFace);
    _mesh.set_halfedge_handle(oldFace, halfEdgeFour);

    // Re-Set Handles around new Face
    _mesh.set_next_halfedge_handle(halfEdgeOne, halfEdgeThree);
    _mesh.set_next_halfedge_handle(halfEdgeThree, halfEdgeTwo);

    _mesh.set_face_handle(halfEdgeOne, newFace);
    _mesh.set_face_handle(halfEdgeTwo, newFace);
    _mesh.set_face_handle(halfEdgeThree, newFace);

    _mesh.set_halfedge_handle(newFace, halfEdgeOne);
}


