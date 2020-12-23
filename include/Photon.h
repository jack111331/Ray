//
// Created by Edge on 2020/10/21.
//

#ifndef RAY_PHOTON_H
#define RAY_PHOTON_H

#include <queue>
#include "Utility.h"
#include "IllumModel.h"
#include "KDTree.h"

class Photon : public CoordData {
public:
    Photon(const Coord &origin, float power, const Velocity &incident, short flag): CoordData(origin), m_power(power), m_incident(incident), m_flag(flag) {}
    enum Flag {
        NORMAL, CAUSTIC
    };
    float m_power;
    Velocity m_incident;
    short m_flag;
};

class HittableList;

class PhotonMappingModel : public IlluminationModel {
public:
    PhotonMappingModel() {
        m_kdTree = new KDTree<Photon>;
    }
    virtual Color castRay(const Scene *scene, Ray &ray, int depth);
    virtual std::string getModelName() const {
        return "PhotonMapping";
    }

    KDTree<Photon> *m_kdTree;
    const int TRACE_PHOTON_AMOUNT = 50;
};

#endif //RAY_PHOTON_H
