//
// Created by Edge on 2021/1/14.
//

#ifndef RAY_BLASNODE_H
#define RAY_BLASNODE_H

#include "ObjectNode.h"

class BVH;

class BLASNode : public ObjectNode {
public:
    BLASNode() : ObjectNode(), m_material(nullptr), m_accel(nullptr), m_boundingBox() {}

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    void setMaterial(Material *material) {
        m_material = material;
    }

    virtual TwoLevelBVHType getTypeInTwoLevelBVH() {
        return TwoLevelBVHType::BLAS;
    }

    enum class BLASNodeType {
        SPHERE,
        TRIANGLE,
        TRIANGLE_GROUP
    };

    virtual BLASNodeType getBLASNodeType() = 0;

    BVH *m_accel;
protected:
    ObjectBoundingBox m_boundingBox;

    Material *m_material;
};

#endif //RAY_BLASNODE_H
