//
// Created by Edge on 2021/1/8.
//

#include <TransformObj.h>
#include <GeometryGroupObj.h>
#include "SelectorObj.h"

bool SelectorObj::isHit(const Ray &ray, IntersectionRecord &record, float tmin, const glm::mat4 &transformMat) const {
    bool isHit = false;
    for (uint32_t memberId = 0; memberId < m_groupMemberList.size();++memberId) {
        if(m_selectMemberEnableList[memberId]) {
            isHit = std::max(isHit, m_groupMemberList[memberId]->isHit(ray, record, tmin, transformMat));
        }
    }
    return isHit;
}

bool SelectorObj::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    for (uint32_t i = 0; i < node.size(); ++i) {
        if (node[i]["type"].as<std::string>() == "transform") {
            TransformObj *object = new TransformObj();
            object->readObjectInfo(node[i], scene);
            m_groupMemberList.push_back(object);
            m_selectMemberEnableList.push_back(node[i]["enable"].as<bool>());
        } else if (node[i]["type"].as<std::string>() == "selector") {
            SelectorObj *object = new SelectorObj();
            object->readObjectInfo(node[i], scene);
            m_groupMemberList.push_back(object);
            m_selectMemberEnableList.push_back(node[i]["enable"].as<bool>());
        } else if (node[i]["type"].as<std::string>() == "geometry-group") {
            GeometryGroupObj *object = new GeometryGroupObj();
            object->readObjectInfo(node[i], scene);
            m_groupMemberList.push_back(object);
            m_selectMemberEnableList.push_back(node[i]["enable"].as<bool>());
        }
    }
    return true;
}