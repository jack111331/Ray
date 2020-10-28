//
// Created by Edge on 2020/10/14.
//

#ifndef ADVANCED_COMPUTER_GRAPH_ILLUMMODEL_H
#define ADVANCED_COMPUTER_GRAPH_ILLUMMODEL_H

#include "Ray.h"
#include "Scene.h"

class IlluminationModel {
public:
    // FIXME for efficiency, don't return color
    virtual Color castRay(const Scene* scene, Ray &ray, double intensity, Color &color, int depth) = 0;
};


#endif //ADVANCED_COMPUTER_GRAPH_ILLUMMODEL_H
