//
// Created by Edge on 2021/1/14.
//

#ifndef RAY_TLASNODE_H
#define RAY_TLASNODE_H

#include "ObjectNode.h"
#include "TriangleGroup.h"

class TLASNode : public ObjectNode {
public:
    TLASNode() : ObjectNode(), m_accel(nullptr), m_boundingBox() {}

    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList, const glm::mat4 &transformMat = glm::mat4(1.0f)) {
        for (auto member: m_groupMemberList) {
            member->createVAO(shadeObjectList, transformMat);
        }
    };

    virtual TwoLevelBVHType getTypeInTwoLevelBVH() {
        return TwoLevelBVHType::TLAS;
    }

    enum class TLASNodeType {
        GROUP,
        GEOMETRY_GROUP,
        SELECTOR,
        TRANSFORM
    };

    virtual TLASNodeType getTlasNodeType() = 0;

    std::vector<ObjectNode *> m_groupMemberList;
protected:
    ObjectBoundingBox m_boundingBox;

    BVH *m_accel;
};

#endif //RAY_TLASNODE_H
