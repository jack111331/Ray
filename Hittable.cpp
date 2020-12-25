//
// Created by Edge on 2020/7/13.
//

#include "Hittable.h"
#include "Utility.h"

bool Hittable::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if(!node["material"]) {
        return false;
    }
    auto it = scene->m_materialTable.find(node["material"].as<std::string>());
    if(it == scene->m_materialTable.end()) {
        return false;
    }
    setMaterial(it->second);
    return true;
}