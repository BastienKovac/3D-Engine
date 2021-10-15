#include "multiplegeometry.h"

MultipleGeometry::MultipleGeometry(Geometry *parent) : Geometry(parent)
{

}

void MultipleGeometry::clearObjects()
{
    for (auto o : _objects)
    {
        delete o;
    }
    _objects.clear();
}

MultipleGeometry::~MultipleGeometry()
{
    clearObjects();
}

void MultipleGeometry::addObject(std::string fileName)
{
    Geometry *newObject = new Geometry();
    newObject->setScene(fileName);

    _objects.push_back(newObject);
}

void MultipleGeometry::setScene(std::string fileName)
{
    namespace fs = std::experimental::filesystem;

    const fs::path path(fileName);
    std::error_code ec;

    if (fs::is_directory(path, ec))
    {
        // Load full scene
        clearObjects();
        for (auto& entry : fs::directory_iterator(path)) {
            auto entryPath = entry.path();
            if (fs::is_regular_file(entryPath, ec) && entryPath.extension() == ".obj")
            {
                addObject(entryPath.string());
            }
        }
    }
    if (fs::is_regular_file(path, ec) && path.extension() == ".obj")
    {
        // Add single object to the scene (without clear)
        addObject(path.string());
    }
}

std::vector<GLfloat> MultipleGeometry::vertices()
{
    if (_objects.size() > 0)
    {
        std::vector<GLfloat> allVertices(_objects[0]->vertices());

        for (unsigned int i = 1 ; i < _objects.size() ; ++i)
        {
            allVertices.insert(allVertices.end(), _objects[i]->vertices().begin(), _objects[i]->vertices().end());
        }

        return allVertices;
    }

    return {};
}

std::vector<GLfloat> MultipleGeometry::normals()
{
    if (_objects.size() > 0)
    {
        std::vector<GLfloat> allNormals(_objects[0]->normals());

        for (unsigned int i = 1 ; i < _objects.size() ; ++i)
        {
            allNormals.insert(allNormals.end(), _objects[i]->normals().begin(), _objects[i]->normals().end());
        }

        return allNormals;
    }

    return {};
}

std::vector<GLuint> MultipleGeometry::indices()
{
    if (_objects.size() > 0)
    {
        std::vector<GLuint> allIndices(_objects[0]->indices());

        for (unsigned int i = 1 ; i < _objects.size() ; ++i)
        {
            allIndices.insert(allIndices.end(), _objects[i]->indices().begin(), _objects[i]->indices().end());
        }

        return allIndices;
    }

    return {};
}
