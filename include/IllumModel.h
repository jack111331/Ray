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

    virtual Vec3f castRay(const Scene *scene, Ray &ray, int depth = 0, bool debugFlag = 0) = 0;

    void setupBackgroundColor(const Vec3f &backgroundColor) {
        m_backgroundColor = backgroundColor;
    }

    virtual std::string getModelName() const = 0;

protected:
    Vec3f m_backgroundColor;
};


#endif //RAY_ILLUMMODEL_H
