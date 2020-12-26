//
// Created by Edge on 2020/10/31.
//

#ifndef RAY_LAMBERTIAN_H
#define RAY_LAMBERTIAN_H


#include "Material.h"
#include "Texture.h"

class LambertianMaterial : public Material {
public:

    virtual Color calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const;
    virtual void calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray,
                                               const HitRecord &record, ShadeRecord &shadeRecord) const;
    virtual MaterialType getType() {return LAMBERTIAN;}
    virtual bool readMaterialInfo(const YAML::Node &node);


    Color m_diffuseColor = {.0f, .0f, .0f};
    Color m_ambientColor = {.0f, .0f, .0f};
    Color m_specularColor = {.0f, .0f, .0f};

    float m_constantSpecularExp = .0f;

    float m_constantRoughness = .0f;

    Texture *m_ambientTexture = nullptr;
    Texture *m_diffuseTexture = nullptr;
};


#endif //RAY_LAMBERTIAN_H
