//
// Created by Edge on 2020/11/19.
//

#ifndef RAY_INSTANTRADIOSITY_H
#define RAY_INSTANTRADIOSITY_H

#include "KDTree.h"
#include "IllumModel.h"

class VPL : public CoordData {

};

class InstantRadiosityModel : public IlluminationModel {
public:
    InstantRadiosityModel() {
        m_kdTree = new KDTree<VPL>;
    }
    virtual void constructModel(const std::vector<Light *> pointLightList, const std::vector<AreaLight *> areaLightList);
    virtual Color castRay(const Scene *scene, Ray &ray, int depth);
    virtual std::string getModelName() const {
        return "InstantRadiosity";
    }


    KDTree<VPL> *m_kdTree;
};


#endif //RAY_INSTANTRADIOSITY_H
