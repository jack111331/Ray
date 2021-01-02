//
// Created by Edge on 2020/10/14.
//

#ifndef RAY_DIELECTRIC_H
#define RAY_DIELECTRIC_H


#include "Utility.h"
#include "Scene.h"
#include <iostream>
#include "Material.h"

class DielectricMaterial : public Material {
public:
    virtual void calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const;

    virtual void calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &model, Ray &ray, const HitRecord &record, ShadeRecord &shadeRecord) const;

    virtual MaterialType getType() {return DIELECTRIC;}

    virtual bool readMaterialInfo(const YAML::Node &node);

    float m_constantReferenceIndex;

};


#endif //RAY_DIELECTRIC_H
