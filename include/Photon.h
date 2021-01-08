//
// Created by Edge on 2020/10/21.
//

#ifndef RAY_PHOTON_H
#define RAY_PHOTON_H

#include <queue>
#include "Utility.h"
#include "IllumModel.h"
#include "KDTree.h"
#include "Pipeline.h"

class Photon : public CoordData {
public:
    Photon(const Vec3f &origin, float power, const Vec3f &incident, short flag) : CoordData(origin), m_power(power),
                                                                                  m_incident(incident),
                                                                                  m_flag(flag) {}

    enum Flag {
        NORMAL, CAUSTIC
    };
    float m_power;
    Vec3f m_incident;
    short m_flag;
};

class GeometryGroupObj;

class PhotonMappingPipeline : public CPURayTracingPipeline {
public:
    virtual void setupPipeline();

    int m_photonAmount;
    float m_photonPower;
    int m_photonTraceDepth;
};

class PhotonMappingModel : public IlluminationModel {
public:
    PhotonMappingModel() : m_photonAmount(0), m_photonPower(0), m_photonTraceDepth(0) {
        m_kdTree = new KDTree<Photon>;
    }

    virtual void setup(Scene *scene, int photonAmount, float photonPower, int photonTraceDepth);

    bool photonTracing(const Scene *scene, Ray &ray, float power, int depth);

    virtual Vec3f castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag);


    virtual std::string getModelName() const {
        return "PhotonMapping";
    }

    KDTree<Photon> *m_kdTree;
    const int TRACE_PHOTON_AMOUNT = 50;
private:
    int m_photonAmount;
    float m_photonPower;
    int m_photonTraceDepth;

};

#endif //RAY_PHOTON_H
