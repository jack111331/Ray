//
// Created by Edge on 2020/10/14.
//

#ifndef ADVANCED_COMPUTER_GRAPH_DIELECTRIC_H
#define ADVANCED_COMPUTER_GRAPH_DIELECTRIC_H


#include "Utility.h"
#include "Scene.h"
#include <iostream>
#include "Material.h"

class DielectricMaterial : public Material {
public:
    friend std::istream &operator >> (std::istream &is, DielectricMaterial &rhs);

    virtual Color calculatePhong(const Scene *scene, const Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord);

    virtual MaterialType getType() {return DIELECTRIC;}

    float m_constantReferenceIndex;

};


#endif //ADVANCED_COMPUTER_GRAPH_DIELECTRIC_H
