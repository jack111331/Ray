//
// Created by Edge on 2020/7/13.
//

#ifndef RAY_MATERIAL_H
#define RAY_MATERIAL_H


#include "Utility.h"
#include <iostream>
#include "yaml-cpp/yaml.h"

class Scene;
class Ray;
class PhotonMappingModel;

class Material {
public:
    enum MaterialType {
        LAMBERTIAN,
        DIELECTRIC,
        AREALIGHT,
        NONE
    };

    struct MaterialProperty {
        uint32_t type;
        uint32_t padding[3];
        Vec4f param0;
        Vec4f param1;
        Vec4f param2;
        Vec4f param3;
        Vec4f param4;
    };

    // return value is for one hit and end, shadeRecord is for following scatter
    virtual Ray calculatePhong(const Scene *scene, const Ray &ray, const IntersectionRecord &record, const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const = 0;

    virtual Ray calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord) const = 0;

    virtual Vec3f calculateScatter(const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord, float &pdf) const {return {};}

    virtual float calculateScatterPdf(const Ray &ray, const IntersectionRecord &record,
                                      const Vec3f &scatteredDirection) const {return 0.0f;}

    virtual MaterialType getType() {return NONE;}

    virtual MaterialProperty getProperty() = 0;

    virtual bool readMaterialInfo(const YAML::Node &node) {return true;};

};


#endif //RAY_MATERIAL_H
