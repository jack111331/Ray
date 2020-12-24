//
// Created by Edge on 2020/7/13.
//

#ifndef RAY_MATERIAL_H
#define RAY_MATERIAL_H


#include "Utility.h"
#include "Photon.h"
#include <iostream>

class Scene;
class Ray;

class Material {
public:
    enum MaterialType {
        LAMBERTIAN,
        DIELECTRIC,
        AREALIGHT,
        NONE
    };
    // return value is for one hit and end, shadeRecord is for following scatter
    virtual Color calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const = 0;
    virtual void calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray, const HitRecord &record, ShadeRecord &shadeRecord) const = 0;

    virtual MaterialType getType() {return NONE;}

    virtual bool readMaterialInfo(const YAML::Node &node) {};

};


#endif //RAY_MATERIAL_H
