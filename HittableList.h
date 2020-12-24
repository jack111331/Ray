//
// Created by Edge on 2020/7/15.
//

#ifndef ADVANCED_COMPUTER_GRAPH_HITTABLELIST_H
#define ADVANCED_COMPUTER_GRAPH_HITTABLELIST_H

#include <vector>
#include "Hittable.h"

class HittableList : public Hittable {
public:
    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    bool isHitShadow(double tmin, const Ray &ray, HitRecord &record);

    void addHittable(Hittable *hittable);

    virtual ObjectBoundingBox getBoundingBox();

    std::vector<Hittable *> m_hittableList;

public:
    ObjectBoundingBox m_boundingBox;
};


#endif //ADVANCED_COMPUTER_GRAPH_HITTABLELIST_H
