#include "geometry.h"

Geometry::Geometry(QObject *parent) : QObject(parent)
{

}

Geometry::~Geometry()
{

}

void Geometry::setScene(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    for (unsigned int i = 0 ; i < vertices.size() ; i += 3)
    {
        _vertexHandles.push_back(_mesh.add_vertex(Mesh::Point(vertices[i], vertices[i + 1], vertices[i + 2])));
    }

    for (unsigned int i = 0 ; i < indices.size() ; i += 3)
    {
        _mesh.add_face({_vertexHandles[indices[i]], _vertexHandles[indices[i + 1]], _vertexHandles[indices[i + 2]]});
    }

    _dirty = true;
    emit geometryRefreshed();
}

void Geometry::refreshGeometryCache()
{
    _vertices.clear();

    // Start index on 1
    int v_index = 1;

 //   auto indexProperty = OpenMesh::makeTemporaryProperty<OpenMesh::VertexHandle, int>(_mesh, "v_index");


    // Refresh normals
    // we need face normals to update the vertex normals
    _mesh.request_face_normals();
    // let the mesh update the normals
    _mesh.update_normals();
    // dispose the face normals, as we don't need them anymore
    _mesh.release_face_normals();

    for (const auto& vh : _mesh.vertices())
    {
    //    indexProperty[vh] = v_index++;

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
    }

    for (Mesh::FaceIter f_it = _mesh.faces_begin() ; f_it != _mesh.faces_end() ; ++f_it)
    {
        for (Mesh::FaceVertexIter fv_it = _mesh.fv_iter(*f_it) ; fv_it.is_valid() ; ++fv_it)
        {
            // Fill indices cache
       //     _indices.push_back(indexProperty[_mesh.point(*fv_it)]);
        }
    }

    _mesh.garbage_collection();
    _dirty = false;
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
