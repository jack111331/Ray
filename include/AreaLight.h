//
// Created by Edge on 2020/10/23.
//

#ifndef RAY_AREALIGHT_H
#define RAY_AREALIGHT_H

#include "Material.h"
#include "Light.h"

class AreaLight : public Material, public Light {
public:
    AreaLight() : Light(), m_uDirection{0, 0, 0}, m_vDirection{0, 0, 0} {}

    virtual Ray
    calculatePhong(const Scene *scene, const Ray &ray, const IntersectionRecord &record, const LightRecord &shadeLightRecord,
                   ShadeRecord &shadeRecord) const;

    virtual Ray calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, const Ray &ray,
                                        const IntersectionRecord &record, ShadeRecord &shadeRecord) const;

    virtual Vec3f calculateScatter(const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord, float &pdf) const;

    virtual float calculateScatterPdf(const Ray &ray, const IntersectionRecord &record,
                                      const Vec3f &scatteredDirection) const;

    virtual MaterialType getType() { return AREALIGHT; }

    virtual MaterialProperty getProperty();

    virtual bool readLightInfo(const YAML::Node &node);

    virtual Vec3f getLightOrigin() const;

    Vec3f m_uDirection, m_vDirection;
    float m_area;

};


#endif //RAY_AREALIGHT_H
