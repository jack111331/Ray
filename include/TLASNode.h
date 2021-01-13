//
// Created by Edge on 2021/1/14.
//

#ifndef RAY_TLASNODE_H
#define RAY_TLASNODE_H

#include "Hittable.h"
#include "TriangleGroup.h"

class TLASNode : public Hittable {
public:
    TLASNode() : m_accel(nullptr), m_boundingBox() {}

    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList) {
        for (auto member: m_groupMemberList) {
            member->createVAO(shadeObjectList);
        }
    };

    std::vector<Hittable *> m_groupMemberList;
protected:
    ObjectBoundingBox m_boundingBox;

    BVH *m_accel;
};

#endif //RAY_TLASNODE_H
