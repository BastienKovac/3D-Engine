#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>

class Geometry : QObject
{
    Q_OBJECT

public:
    Geometry(QObject *parent = nullptr);
    ~Geometry();

    void setScene(std::vector<GLfloat> vertices, std::vector<GLuint> indices);

    bool hasGeometry();

    std::vector<GLfloat> vertices() const;
    std::vector<GLfloat> normals() const;
    std::vector<GLuint> indices() const;

signals:
    void geometryRefreshed();

private:
    // A simple geometry
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

    // Compute normals
    void computeNormalsFor(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    glm::vec3 computeNormalFor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

};

#endif // GEOMETRY_H
