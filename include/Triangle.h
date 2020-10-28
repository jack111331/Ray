//
// Created by Edge on 2020/7/9.
//

#ifndef ADVANCED_COMPUTER_GRAPH_TRIANGLE_H
#define ADVANCED_COMPUTER_GRAPH_TRIANGLE_H


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

    Coord m_point[3];
};


#endif //ADVANCED_COMPUTER_GRAPH_TRIANGLE_H
