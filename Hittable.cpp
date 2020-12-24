//
// Created by Edge on 2020/7/13.
//

#include "Hittable.h"
#include "Utility.h"

bool Hittable::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if(!node["material"]) {
        return false;
    }
    if(scene->m_materialTable.find(node["material"].as<std::string>()) == scene->m_materialTable.end()) {
        return false;
    }
    m_material = scene->m_materialTable[node["material"].as<std::string>()];
    return true;
}