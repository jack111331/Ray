//
// Created by Edge on 2020/7/15.
//

#ifndef RAY_GEOMETRY_GROUP_OBJ_H
#define RAY_GEOMETRY_GROUP_OBJ_H

#include <vector>
#include "Hittable.h"
#include "TLASNode.h"

class GeometryGroupObj : public TLASNode {
public:
    GeometryGroupObj(): TLASNode() {}

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    void addHittable(Hittable *hittable);

    void addHittable(std::vector<Hittable *> &hittableList);

    virtual ObjectBoundingBox getBoundingBox() const;

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);
};


#endif //RAY_GEOMETRY_GROUP_OBJ_H
