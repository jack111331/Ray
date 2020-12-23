//
// Created by Edge on 2020/10/23.
//

#include "AreaLight.h"

using namespace std;

istream &operator>>(istream &lhs, AreaLight &areaLight) {
    lhs >> areaLight.m_emitColor;

    Coord planePoint[4];
    for (int i = 0; i < 4; ++i) {
        lhs >> planePoint[i];
    }
    areaLight.m_origin = planePoint[0];
    areaLight.m_uDirection = planePoint[1] - planePoint[0];
    areaLight.m_vDirection = planePoint[3] - planePoint[0];
}


Color
AreaLight::calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record, const LightRecord &shadeLightRecord,
                          ShadeRecord &shadeRecord) const {
    shadeRecord.emit = m_emitColor;
    return shadeRecord.emit;
}

void AreaLight::calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray,
                                       const HitRecord &record, ShadeRecord &shadeRecord) const {
    priority_queue<CoordDataElement<Photon>> photonHeap;
    photonMappingModel.m_kdTree->nearestSearch(record.point, photonMappingModel.TRACE_PHOTON_AMOUNT, photonHeap);
    if (!photonHeap.empty()) {
        // squared distance
        float squaredRadius = photonHeap.top().m_distance;
        float area = squaredRadius * acos(-1);
        while (!photonHeap.empty()) {
            shadeRecord.emit += photonHeap.top().m_photon->m_power / area * Color{1.0, 1.0, 1.0};
            photonHeap.pop();
        }
    }
    shadeRecord.attenuation = m_emitColor;
    Velocity diffuseDirection = Util::randomSphere();
    // if ray velocity dot record normal is negative and diffuse direction dot record normal is positive
    // or ray velocity dot record normal is positive and diffuse direction dot record normal is negative
    // then we don't need to negative the diffuse direction, otherwise we should fix diffuse direction
    if (ray.velocity.dot(record.normal) * diffuseDirection.dot(record.normal) > 0.0) {
        diffuseDirection = -diffuseDirection;
    }
    ray = {record.point, diffuseDirection};
}

Coord AreaLight::getLightOrigin() const {
    float photonPositionU, photonPositionV;
    do {
        photonPositionU = Util::randomInUnit();
        photonPositionV = Util::randomInUnit();
    } while (photonPositionU < 0 || photonPositionV < 0 || photonPositionU + photonPositionV > 1);
    return m_origin + photonPositionU * m_uDirection + photonPositionV * m_vDirection;
}