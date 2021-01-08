//
// Created by Edge on 2020/10/23.
//

#include "AreaLight.h"
#include "Photon.h"

using namespace std;

void
AreaLight::calculatePhong(const Scene *scene, Ray &ray, const IntersectionRecord &record, const LightRecord &shadeLightRecord,
                          ShadeRecord &shadeRecord) const {
    shadeRecord.emit = m_emitColor;
    shadeRecord.attenuation = {0.0, .0, .0};
}

void AreaLight::calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray,
                                       const IntersectionRecord &record, ShadeRecord &shadeRecord) const {
    priority_queue<CoordDataElement<Photon>> photonHeap;
    photonMappingModel.m_kdTree->nearestSearch(record.point, photonMappingModel.TRACE_PHOTON_AMOUNT, photonHeap);
    if (!photonHeap.empty()) {
        // squared distance
        float squaredRadius = photonHeap.top().m_distance;
        float area = squaredRadius * acos(-1);
        while (!photonHeap.empty()) {
            shadeRecord.emit += photonHeap.top().m_photon->m_power / area * Vec3f{1.0, 1.0, 1.0};
            photonHeap.pop();
        }
    }
    shadeRecord.attenuation = m_emitColor;
    Vec3f diffuseDirection = Util::randomSphere();
    // if ray velocity dot record normal is negative and diffuse direction dot record normal is positive
    // or ray velocity dot record normal is positive and diffuse direction dot record normal is negative
    // then we don't need to negative the diffuse direction, otherwise we should fix diffuse direction
    if (ray.velocity.dot(record.normal) * diffuseDirection.dot(record.normal) > 0.0) {
        diffuseDirection = -diffuseDirection;
    }
    ray = {record.point, diffuseDirection};
}

Vec3f AreaLight::getLightOrigin() const {
    float photonPositionU, photonPositionV;
    do {
        photonPositionU = Util::randomInUnit();
        photonPositionV = Util::randomInUnit();
    } while (photonPositionU < 0 || photonPositionV < 0 || photonPositionU + photonPositionV > 1);
    return m_origin + photonPositionU * m_uDirection + photonPositionV * m_vDirection;
}

bool AreaLight::readLightInfo(const YAML::Node &node) {
    bool result = Light::readLightInfo(node);
    result = min(result, !node["u-direction"] || !node["v-direction"]);
    if(!result) {
        return false;
    }
    m_uDirection = Vec3f::toVec3f(node["u-direction"].as<std::vector<float>>());
    m_vDirection = Vec3f::toVec3f(node["v-direction"].as<std::vector<float>>());
    return result;
}