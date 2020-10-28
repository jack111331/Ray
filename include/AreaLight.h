//
// Created by Edge on 2020/10/23.
//

#ifndef RAY_AREALIGHT_H
#define RAY_AREALIGHT_H

#include "Material.h"

class AreaLight : public Material {
public:
    AreaLight(Coord (*point)[4]) {
        // circular point
        m_point = (*point)[0];
        m_uDirection = (*point)[1] - (*point)[0];
        m_vDirection = (*point)[3] - (*point)[0];
        m_normal = m_uDirection.cross(m_vDirection);
    }

    virtual MaterialType getType() {return AREALIGHT;}

    Coord m_point;
    Velocity m_uDirection, m_vDirection;
    Velocity m_normal;
};


#endif //RAY_AREALIGHT_H
