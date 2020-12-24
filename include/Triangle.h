//
// Created by Edge on 2020/7/9.
//

#ifndef RAY_TRIANGLE_H
#define RAY_TRIANGLE_H


#include "Utility.h"
#include "Hittable.h"
#include "Material.h"

class Triangle : public Hittable {
public:
    Triangle(Coord (*coord)[3]) {
        for (int i = 0; i < 3; ++i) {
            m_point[i] = (*coord)[i];
        }
    };

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    virtual std::vector<ObjectInfo> createVAO(const Material *material);

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);


    Coord m_point[3];
};


#endif //RAY_TRIANGLE_H
