//
// Created by Edge on 2021/1/8.
//

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GeometryGroupObj.h>
#include "TransformObj.h"

glm::mat4 TransformObj::getTransformMat() const {
    // translate * orientation * scale
    return glm::translate(glm::orientation(
            glm::vec3(m_objectToWorldOrientNormal.x, m_objectToWorldOrientNormal.y, m_objectToWorldOrientNormal.z),
            glm::vec3(m_objectToWorldOrientUp.x, m_objectToWorldOrientUp.y, m_objectToWorldOrientUp.z)) *
                          glm::scale(glm::mat4(1.0f),
                                     glm::vec3(m_objectToWorldScale.x, m_objectToWorldScale.y, m_objectToWorldScale.z)),
                          glm::vec3(m_objectToWorldPos.x, m_objectToWorldPos.y, m_objectToWorldPos.z));
}

bool TransformObj::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if (node["geometry-group"]) {
        GeometryGroupObj *object = new GeometryGroupObj();
        object->readObjectInfo(node["geometry-group"], scene);
        m_transformMember = object;
    }
    if(node["position"]) {
        m_objectToWorldPos = Vec3f::toVec3f(node["position"].as<std::vector<float>>());
    }
    if(node["orientation-normal"]) {
        m_objectToWorldOrientNormal = Vec3f::toVec3f(node["orientation-normal"].as<std::vector<float>>());
    }
    if( node["orientation-up"]) {
        m_objectToWorldOrientUp = Vec3f::toVec3f(node["orientation-up"].as<std::vector<float>>());

    } if(node["scale"]) {
        m_objectToWorldScale = Vec3f::toVec3f(node["scale"].as<std::vector<float>>());
    }
    return true;
}