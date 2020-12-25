//
// Created by Edge on 2020/10/14.
//

#ifndef RAY_WHITTED_H
#define RAY_WHITTED_H

#include <vector>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "IllumModel.h"

class WhittedModel : public IlluminationModel {
public:
    virtual Color castRay(const Scene *scene, Ray &ray, int depth);
    virtual std::string getModelName() const {
        return "Whitted";
    }
};



#endif //RAY_WHITTED_H
