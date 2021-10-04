#include "geometry.h"

Geometry::Geometry(QObject *parent) : QObject(parent)
{

}

Geometry::~Geometry()
{

}

void Geometry::setScene(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    _vertices.clear();
    _vertices.insert(_vertices.end(), vertices.begin(), vertices.end());

    _indices.clear();
    _indices.insert(_indices.end(), indices.begin(), indices.end());

    computeNormalsFor(vertices, indices);

    emit geometryRefreshed();
}

bool Geometry::hasGeometry()
{
    return !(_vertices.empty() && _indices.empty());
}

std::vector<GLfloat> Geometry::vertices() const
{
    return _vertices;
}

std::vector<GLfloat> Geometry::normals() const
{
    return _normals;
}

std::vector<GLuint> Geometry::indices() const
{
    return _indices;
}

void Geometry::computeNormalsFor(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    std::vector<glm::vec3> normals = std::vector<glm::vec3>(vertices.size() / 3);

    for (unsigned int i = 0 ; i < indices.size() ; i += 3)
    {
        glm::vec3 a = glm::vec3(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        glm::vec3 b = glm::vec3(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
        glm::vec3 c = glm::vec3(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);

        glm::vec3 normal = computeNormalFor(a, b, c);

        normals[indices[i]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }

    for (unsigned int i = 0 ; i < normals.size() ; ++i)
    {
        normals[i] = glm::normalize(normals[i]);

        _normals.push_back(normals[i][0]);
        _normals.push_back(normals[i][1]);
        _normals.push_back(normals[i][2]);
    }
}

glm::vec3 Geometry::computeNormalFor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    // Uses p2 as a new origin for p1,p3
    glm::vec3 ab = p2 - p1;
    glm::vec3 ac = p3 - p1;

    // Compute the cross product a X b to get the face normal
    return glm::normalize(glm::cross(ab, ac));
}
