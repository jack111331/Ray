//
// Created by Edge on 2021/1/14.
//

#include "BLASNode.h"
#include "Scene.h"

bool BLASNode::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if(!node["material"]) {
        std::cerr << "No require object node" << std::endl;
        return false;
    }
    auto it = scene->m_materialTable.find(node["material"].as<std::string>());
    if(it == scene->m_materialTable.end()) {
        std::cerr << "Can't find material in material table" << std::endl;
        return false;
    }
    setMaterial(it->second);
    return true;
}