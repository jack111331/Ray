//
// Created by Edge on 2021/1/23.
//

#include "ONB.h"

ONB::ONB(const Vec3f &direction) {
    m_direction = direction.normalize();
    Vec3f up(1.0f, .0f, .0f);
    if(m_direction.x > 0.9f) {
        up = Vec3f(.0f, 1.0f, .0f);
    }
    m_tangent = m_direction.cross(up).normalize();
    m_cotangent = m_direction.cross(m_tangent).normalize();
}

Vec3f ONB::toWorldCoordSystem(const Vec3f &localVector) {
    return m_cotangent * localVector.x + m_tangent * localVector.y + m_direction * localVector.z;
}