//
// Created by Edge on 2020/10/31.
//

#ifndef RAY_LAMBERTIAN_H
#define RAY_LAMBERTIAN_H


#include "Material.h"
#include "Texture.h"

class LambertianMaterial : public Material {
public:

    virtual Ray
    calculatePhong(const Scene *scene, const Ray &ray, const IntersectionRecord &record, const LightRecord &shadeLightRecord,
                   ShadeRecord &shadeRecord) const;

    virtual Ray calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, const Ray &ray,
                                        const IntersectionRecord &record, ShadeRecord &shadeRecord) const;

    virtual Vec3f
    calculateScatter(const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord, float &pdf) const;

    virtual float calculateScatterPdf(const Ray &ray, const IntersectionRecord &record,
                                      const Vec3f &scatteredDirection) const;

    virtual MaterialType getType() { return LAMBERTIAN; }

    virtual bool readMaterialInfo(const YAML::Node &node);

    virtual MaterialProperty getProperty();


    Vec3f m_diffuseColor = {.0f, .0f, .0f};
    Vec3f m_ambientColor = {.0f, .0f, .0f};
    Vec3f m_specularColor = {.0f, .0f, .0f};

    float m_constantSpecularExp = .0f;

    float m_constantRoughness = .0f;

    Texture *m_ambientTexture = nullptr;
    Texture *m_diffuseTexture = nullptr;
};


#endif //RAY_LAMBERTIAN_H
