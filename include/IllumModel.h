//
// Created by Edge on 2020/10/14.
//

#ifndef RAY_ILLUMMODEL_H
#define RAY_ILLUMMODEL_H

#include "Ray.h"
#include "Scene.h"

class IlluminationModel {
public:
    virtual void setup(Scene *scene) {};

    virtual Color castRay(const Scene *scene, Ray &ray, int depth = 0) = 0;

    void setupBackgroundColor(const Color &backgroundColor) {
        m_backgroundColor = backgroundColor;
    }

    virtual std::string getModelName() const = 0;

protected:
    Color m_backgroundColor;
};


#endif //RAY_ILLUMMODEL_H
