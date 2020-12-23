//
// Created by Edge on 2020/10/14.
//

#ifndef RAY_ILLUMMODEL_H
#define RAY_ILLUMMODEL_H

#include "Ray.h"
#include "Scene.h"

class IlluminationModel {
public:
    virtual void constructModel(const std::vector<Light *> pointLightList, const std::vector<AreaLight *> areaLightList) {};
    virtual Color castRay(const Scene* scene, Ray &ray, int depth = 0) = 0;
    virtual std::string getModelName() const = 0;
};


#endif //RAY_ILLUMMODEL_H
