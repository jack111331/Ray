//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_TRANSFORMOBJ_H
#define RAY_TRANSFORMOBJ_H


#include <glm/ext/matrix_float4x4.hpp>
#include "Utility.h"
#include "ObjectNode.h"
#include "TLASNode.h"

class TransformObj : public TLASNode {
public:
    TransformObj() : TLASNode(), m_objectToWorldPos(), m_objectToWorldOrientNormal(0.0f, 1.0f, 0.0f), m_objectToWorldOrientUp(0.0f, 1.0f, 0.0f), m_objectToWorldScale(1.0f, 1.0f, 1.0f) {}

    glm::mat4 getTransformMat() const;

    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList, const glm::mat4 &transformMat = glm::mat4(1.0f)) {
        glm::mat4 newTransformMat = getTransformMat() * transformMat;
        for (auto member: m_groupMemberList) {
            member->createVAO(shadeObjectList, newTransformMat);
        }
    };

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const {
        Ray transformedRay = ray;
        auto objectToWorldMat = getTransformMat();
        auto worldToObjectMat = glm::inverse(objectToWorldMat);
        transformedRay.origin = worldToObjectMat * transformedRay.origin;
        transformedRay.velocity = worldToObjectMat * transformedRay.velocity;
        return m_groupMemberList[0]->isHit(transformedRay, record, tmin, objectToWorldMat * transformMat);
    }

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const {
        return ObjectBoundingBox::transformToLocal(getTransformMat(), m_groupMemberList[0]->getBoundingBox());
    }

    virtual TLASNodeType getTlasNodeType() {
        return TLASNodeType::TRANSFORM;
    }


    Vec3f m_objectToWorldPos;
    Vec3f m_objectToWorldOrientNormal;
    Vec3f m_objectToWorldOrientUp;
    Vec3f m_objectToWorldScale;
    // TODO cache needed
};


#endif //RAY_TRANSFORMOBJ_H
