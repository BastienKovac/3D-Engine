#include "bone.h"

Bone::Bone(unsigned int id) :
    _boneID(id),
    _rotation(0, 0, 0),
    _translation(0, 0,0 ),
    _scale(1, 1, 1),
    _transform(identity())
{};

void Bone::addChildBone(unsigned int child_id) {
    _children.push_back(std::make_unique<Bone>(child_id));
}

glm::mat4 Bone::getTransformMatrix() const {
    auto rotationX = glm::rotate(identity(), glm::radians(_rotation.x), { 1.0, 0, 0 });
    auto rotationY = glm::rotate(identity(), glm::radians(_rotation.y), { 0, 1.0, 0 });
    auto rotationZ = glm::rotate(identity(), glm::radians(_rotation.z), { 0, 0, 1.0 });
    auto translation = glm::translate(identity(), _translation);
    auto scale = glm::scale(identity(), _scale);

    return translation * (rotationZ * rotationY * rotationX) * scale;
}

void Bone::resetAllTransforms() {
    _rotation = { 0, 0, 0 };
    _translation = { 0, 0, 0 };
    _scale = { 1, 1, 1};
    _transform = identity();

    for(auto& child : _children) {
        child->resetAllTransforms();
    }
}

void Bone::resetAllRotations() {
    _rotation = { 0, 0, 0 };

    for(auto& child : _children) {
        child->resetAllRotations();
    }
}

void Bone::resetAllScales() {
    _scale = { 1,1,1 };

    for(auto& child : _children) {
        child->resetAllScales();
    }
}

void Bone::resetAllTranslations() {
    _translation = { 0,0,0 };

    for(auto& child : _children) {
        child->resetAllTranslations();
    }
}
