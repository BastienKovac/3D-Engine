#ifndef BONE_H
#define BONE_H

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

class Bone
{
public :

    Bone(unsigned int id);

    void addChildBone(unsigned int child_id);

    std::unique_ptr<Bone>& firstChild() { return _children.front(); }

    glm::mat4 getTransformMatrix() const;

    inline unsigned int ID() const { return _boneID; }

    inline const glm::vec3& rotation() const { return _rotation; }
    inline void setRotation(glm::vec3 newRotation) { _rotation = newRotation; }

    inline GLfloat rotationX() const { return _rotation.x; };
    inline void setRotationX(GLfloat newRotationX) { _rotation.x = newRotationX; }

    GLfloat rotationY() const { return _rotation.y; };
    void setRotationY(GLfloat newRotationY) { _rotation.y = newRotationY; }

    GLfloat rotationZ() const { return _rotation.z; };
    void setRotationZ(GLfloat newRotationZ) { _rotation.z = newRotationZ; }

    // Translation getters and setters
    inline const glm::vec3& traslation() const { return _translation; }
    inline void setTranslation(glm::vec3 newTranslation) { _translation = newTranslation; }

    inline GLfloat TranslationX() const { return _translation.x; };
    inline void setTranslationX(GLfloat newTranslationX) { _translation.x = newTranslationX; }

    inline GLfloat TranslationY() const { return _translation.y; };
    inline void setTranslationY(GLfloat newTranslationY) { _translation.y = newTranslationY; }

    inline GLfloat TranslationZ() const { return _translation.z; };
    inline void setTranslationZ(GLfloat newTranslationZ) { _translation.z = newTranslationZ; }

    // Scale getters and setters
    inline const glm::vec3& scale() const { return _scale; }
    inline void setScale(glm::vec3 newScale) { _scale = newScale; }

    inline GLfloat ScaleX() const { return _scale.x; };
    inline void setScaleX(GLfloat newScaleX) { _scale.x = newScaleX; }

    inline GLfloat ScaleY() const { return _scale.y; };
    inline void setScaleY(GLfloat newScaleY) { _scale.y = newScaleY; }

    inline GLfloat ScaleZ() const { return _scale.z; };
    inline void setScale(GLfloat newScaleZ) { _scale.z = newScaleZ; }

    void resetAllTransforms();
    void resetAllRotations();
    void resetAllTranslations();
    void resetAllScales();

private :

    unsigned int _boneID = 0;

    glm::vec3 _rotation;
    glm::vec3 _translation;
    glm::vec3 _scale;

    glm::mat4 _transform;

    std::vector<std::unique_ptr<Bone>> _children;

    inline GLfloat degToRad(GLfloat degree)
    {
        return degree / (M_PI * 180.0f);
    }

    static const glm::mat4 identity()
    {
        return glm::mat4(1.0f);
    }
};

#endif // BONE_H
