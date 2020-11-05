//
// Created by Edge on 2020/10/23.
//

#ifndef RAY_AREALIGHT_H
#define RAY_AREALIGHT_H

#include "Material.h"

class AreaLight : public Material {
public:
    friend std::istream &operator >> (std::istream &lhs, AreaLight &areaLight);

    virtual Color calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const;
    virtual void calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray, const HitRecord &record, ShadeRecord &shadeRecord) const;

    virtual MaterialType getType() {return AREALIGHT;}

    Color m_emitColor = {.0f, .0f, .0f};

    Coord m_point;
    Velocity m_uDirection, m_vDirection;

};


#endif //RAY_AREALIGHT_H
