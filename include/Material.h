//
// Created by Edge on 2020/7/13.
//

#ifndef ADVANCED_COMPUTER_GRAPH_MATERIAL_H
#define ADVANCED_COMPUTER_GRAPH_MATERIAL_H


#include "Utility.h"
#include <iostream>

class Scene;
class Ray;

class Material {
public:
    enum MaterialType {
        DIFFUSE,
        DIELECTRIC,
        AREALIGHT
    };

    friend std::istream &operator >> (std::istream &is, Material &rhs);

    virtual Color calculatePhong(const Scene *scene, const Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord);

    virtual MaterialType getType() {return DIFFUSE;}

    Color m_color;
    double m_constantAmbient;
    double m_constantDiffuse;

    double m_constantSpecular;
    double m_constantSpecularExp;

    double m_constantReflectionRatio;

};


#endif //ADVANCED_COMPUTER_GRAPH_MATERIAL_H
