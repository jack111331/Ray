//
// Created by Edge on 2020/7/13.
//

#ifndef RAY_LIGHT_H
#define RAY_LIGHT_H


#include "Utility.h"
#include "yaml-cpp/yaml.h"

class Light {
public:
    Light() : m_origin{0, 0, 0}, m_emitColor{0, 0, 0} {}
    Light(Coord origin, Color color) : m_origin(origin), m_emitColor(color) {}

    virtual bool readLightInfo(const YAML::Node &node);

    virtual Coord getLightOrigin() const;

    Coord m_origin;
    Color m_emitColor;
};

#endif //RAY_LIGHT_H
