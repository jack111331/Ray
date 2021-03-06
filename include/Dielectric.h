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
    virtual Ray calculatePhong(const Scene *scene, const Ray &ray, const IntersectionRecord &record, const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const;

    virtual Ray calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &model, const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord) const;

    virtual MaterialType getType() {return DIELECTRIC;}

    virtual bool readMaterialInfo(const YAML::Node &node);

    virtual MaterialProperty getProperty();

    float m_constantReferenceIndex;

};


#endif //RAY_DIELECTRIC_H
