//
// Created by Edge on 2020/7/15.
//

#ifndef ADVANCED_COMPUTER_GRAPH_HITTABLELIST_H
#define ADVANCED_COMPUTER_GRAPH_HITTABLELIST_H

#include <vector>
#include "Hittable.h"

class HittableList : public Hittable {
public:
    virtual bool isHit(const Ray &ray, HitRecord &record, float tmin = 0.0001f) const;

    bool isHitShadow(const Ray &ray, HitRecord &record, float tmin = 0.0001f);

    void addHittable(Hittable *hittable);

    void addHittable(std::vector<Hittable *> hittableList);

    virtual ObjectBoundingBox getBoundingBox() const;

    std::vector<Hittable *> m_hittableList;

public:
    ObjectBoundingBox m_boundingBox;
};


#endif //ADVANCED_COMPUTER_GRAPH_HITTABLELIST_H
