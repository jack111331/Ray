//
// Created by Edge on 2021/1/14.
//

#ifndef RAY_BLASNODE_H
#define RAY_BLASNODE_H

#include "Hittable.h"

class BVH;

class BLASNode : public Hittable {
public:
    BLASNode() : Hittable(), m_accel(nullptr), m_boundingBox() {}

protected:
    ObjectBoundingBox m_boundingBox;

    BVH *m_accel;
};

#endif //RAY_BLASNODE_H
