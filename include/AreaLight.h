//
// Created by Edge on 2020/10/23.
//

#ifndef RAY_AREALIGHT_H
#define RAY_AREALIGHT_H

#include "Material.h"

class AreaLight : public Material, public Light {
public:
    AreaLight() : Light(), m_uDirection{0, 0, 0}, m_vDirection{0, 0, 0} {}

    friend std::istream &operator>>(std::istream &lhs, AreaLight &areaLight);

    virtual Color
    calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord,
                   ShadeRecord &shadeRecord) const;

    virtual void calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray,
                                        const HitRecord &record, ShadeRecord &shadeRecord) const;

    virtual MaterialType getType() { return AREALIGHT; }

    virtual bool readLightInfo(const YAML::Node &node);

    virtual Coord getLightOrigin() const;

    Velocity m_uDirection, m_vDirection;

};


#endif //RAY_AREALIGHT_H
