//
// Created by Edge on 2020/7/13.
//

#include "Light.h"

Vec3f Light::getLightOrigin() const {
    return m_origin;
}

bool Light::readLightInfo(const YAML::Node &node) {
    if(!node["position"] || !node["color"]) {
        return false;
    }
    m_origin = Vec3f::toVec3f(node["position"].as<std::vector<float>>());
    m_emitColor = Vec3f::toVec3f(node["color"].as<std::vector<float>>());
}