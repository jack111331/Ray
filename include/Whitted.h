//
// Created by Edge on 2020/10/14.
//

#ifndef ADVANCED_COMPUTER_GRAPH_WHITTED_H
#define ADVANCED_COMPUTER_GRAPH_WHITTED_H

#include <vector>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "IllumModel.h"

class HittableList;

class WhittedModel : public IlluminationModel {
public:
    virtual Color castRay(const Scene *scene, Ray &ray, double intensity, Color &color, int depth);
};



#endif //ADVANCED_COMPUTER_GRAPH_WHITTED_H
