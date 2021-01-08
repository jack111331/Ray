//
// Created by Edge on 2021/1/8.
//

#include <TransformObj.h>
#include <SelectorObj.h>
#include <GeometryGroupObj.h>
#include "GroupObj.h"

bool GroupObj::isHit(const Ray &ray, IntersectionRecord &record, float tmin, const glm::mat4 &transformMat) const {
    if(m_accel) {
        return m_accel->isHit(ray, record, tmin, transformMat);
    }
    bool isHit = false;
    for(auto member: m_groupMemberList) {
        isHit = std::max(isHit, member->isHit(ray, record, tmin, transformMat));
    }
    return isHit;
}

bool GroupObj::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if (!node["group"]) {
        std::cerr << "No group setting" << std::endl;
        exit(1);
    }
    auto groupNode = node["group"];
    for (uint32_t i = 0; i < groupNode.size(); ++i) {
        if (groupNode[i]["type"].as<std::string>() == "transform") {
            TransformObj *object = new TransformObj();
            object->readObjectInfo(groupNode[i], scene);
            m_boundingBox.updateBoundingBox(object->getBoundingBox());
            m_groupMemberList.push_back(object);
        } else if (groupNode[i]["type"].as<std::string>() == "selector") {
            SelectorObj *object = new SelectorObj();
            object->readObjectInfo(groupNode[i], scene);
            m_boundingBox.updateBoundingBox(object->getBoundingBox());
            m_groupMemberList.push_back(object);
        } else if (groupNode[i]["type"].as<std::string>() == "geometry-group") {
            GeometryGroupObj *object = new GeometryGroupObj();
            object->readObjectInfo(groupNode[i], scene);
            m_boundingBox.updateBoundingBox(object->getBoundingBox());
            m_groupMemberList.push_back(object);
        }
    }
    m_accel = new BVH(this);
    return true;
}

ObjectBoundingBox GroupObj::getBoundingBox() const {
    return m_boundingBox;
}