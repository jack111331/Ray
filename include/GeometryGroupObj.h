//
// Created by Edge on 2020/7/15.
//

#ifndef RAY_GEOMETRY_GROUP_OBJ_H
#define RAY_GEOMETRY_GROUP_OBJ_H

#include <vector>
#include "Hittable.h"
#include "GroupObj.h"

class GeometryGroupObj : public Hittable {
public:
    GeometryGroupObj(): m_accel(nullptr), m_boundingBox() {}

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    void addHittable(Hittable *hittable);

    void addHittable(std::vector<Hittable *> &hittableList);

    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList) {
        for (auto member: m_geometryList) {
            member->createVAO(shadeObjectList);
        }
    };

    virtual ObjectBoundingBox getBoundingBox() const;

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    std::vector<Hittable *> m_geometryList;

public:
    BVH *m_accel;

    ObjectBoundingBox m_boundingBox;
};


#endif //RAY_GEOMETRY_GROUP_OBJ_H
