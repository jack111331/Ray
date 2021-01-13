//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_GROUPOBJ_H
#define RAY_GROUPOBJ_H


#include "TriangleGroup.h"
#include "TLASNode.h"

class GroupObj : public TLASNode {
public:
    GroupObj() : TLASNode() {}

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f,
                       const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const;
};


#endif //RAY_GROUPOBJ_H
