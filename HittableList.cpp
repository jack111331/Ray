//
// Created by Edge on 2020/7/15.
//

#include "HittableList.h"

using namespace std;

bool ObjectBoundingBox::isHit(const Ray &ray) {
    float tMin = (m_bounding[0].x - ray.origin.x) / ray.velocity.x;
    float tMax = (m_bounding[1].x - ray.origin.x) / ray.velocity.x;

    if (tMin > tMax) swap(tMin, tMax);

    float tyMin = (m_bounding[0].y - ray.origin.y) / ray.velocity.y;
    float tyMax = (m_bounding[1].y - ray.origin.y) / ray.velocity.y;

    if (tyMin > tyMax) swap(tyMin, tyMax);

    // Test if it intersect in 2d
    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (m_bounding[0].z - ray.origin.z) / ray.velocity.z;
    float tzMax = (m_bounding[1].z - ray.origin.z) / ray.velocity.z;

    if (tzMin > tzMax) swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

bool HittableList::isHit(double tmin, const Ray &ray, HitRecord &record) {
    bool isHit = false;
    for (auto hittable: m_hittableList) {
        isHit = std::max(isHit, hittable->isHit(tmin, ray, record));
    }
    return isHit;
}

bool HittableList::isHitShadow(double tmin, const Ray &ray, HitRecord &record) {
    bool isHit = false;
    for (auto hittable: m_hittableList) {
        if(hittable->m_material->getType() != Material::MaterialType::LAMBERTIAN) {
            isHit = std::max(isHit, hittable->isHit(tmin, ray, record));
        }
    }
    return isHit;
}

void HittableList::addHittable(Hittable * hittable) {
    m_hittableList.push_back(hittable);
    m_boundingBox.updateBoundingBox(hittable->getBoundingBox());
}

void HittableList::addHittable(std::vector<Hittable *> hittableList) {
    for(auto hittable: hittableList) {
        addHittable(hittable);
    }
}

ObjectBoundingBox HittableList::getBoundingBox() {
    return m_boundingBox;
}