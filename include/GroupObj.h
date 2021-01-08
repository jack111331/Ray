//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_GROUPOBJ_H
#define RAY_GROUPOBJ_H


#include "Hittable.h"
#include "TriangleGroup.h"

class GroupObj : public Hittable {
public:
    GroupObj() : m_accel(nullptr), m_boundingBox() {}

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f,
                       const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    // For local shading
    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList) {
        for (auto member: m_groupMemberList) {
            member->createVAO(shadeObjectList);
        }
    };

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const;

    std::vector<Hittable *> m_groupMemberList;
private:
    ObjectBoundingBox m_boundingBox;

    BVH *m_accel;
};


#endif //RAY_GROUPOBJ_H
