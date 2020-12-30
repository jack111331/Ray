//
// Created by Edge on 2020/7/13.
//

#include "Light.h"

Coord Light::getLightOrigin() const {
    return m_origin;
}

bool Light::readLightInfo(const YAML::Node &node) {
    if(!node["position"] || !node["color"]) {
        return false;
    }
    m_origin = Coord::toCoord(node["position"].as<std::vector<float>>());
    m_emitColor = Color::toColor(node["color"].as<std::vector<float>>());
}