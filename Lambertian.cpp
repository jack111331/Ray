//
// Created by Edge on 2020/10/31.
//

#include <Photon.h>
#include "Lambertian.h"

using namespace std;

Color LambertianMaterial::calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record,
                                         const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const {
    // calculate diffuse and specular
    Velocity normal = record.normal;
    if (ray.velocity.dot(normal) > .0f) {
        normal = -normal;
    }
    float diffuseIntensity = .0f;
    float specularIntensity = .0f;
    for (int i = 0; i < scene->m_lightList.size(); ++i) {
        if (shadeLightRecord.isShadedLightList[i]) {
            Velocity lightDirection = (scene->m_lightList[i]->getLightOrigin() - record.point).normalize();
            diffuseIntensity += max(.0f, normal.dot(lightDirection));
            Velocity halfwayVector = (lightDirection - ray.velocity.normalize()).normalize();
            specularIntensity += pow(max(.0f, normal.dot(halfwayVector)), m_constantSpecularExp);
        }
    }
    // FIXME result color bug
    Color resultColor = m_ambientColor + min(1.0f, diffuseIntensity) * m_diffuseColor +
                        min(1.0f, specularIntensity) * m_specularColor;
    shadeRecord.emit = {.0f, .0f, .0f};
    shadeRecord.attenuation =
            resultColor * (m_diffuseTexture ? m_diffuseTexture->getColor(record.texCoord) : Color{1.0f, 1.0f,
                                                                                                  1.0f});
    return shadeRecord.attenuation;
}

void
LambertianMaterial::calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, Ray &ray,
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
    shadeRecord.attenuation =
            m_diffuseColor * (m_diffuseTexture ? m_diffuseTexture->getColor(record.texCoord) : Color{1.0f, 1.0f, 1.0f});
    Velocity diffuseDirection = Util::randomSphere();
    // if ray velocity dot record normal is negative and diffuse direction dot record normal is positive
    // or ray velocity dot record normal is positive and diffuse direction dot record normal is negative
    // then we don't need to negative the diffuse direction, otherwise we should fix diffuse direction
    if (ray.velocity.dot(record.normal) * diffuseDirection.dot(record.normal) > 0.0) {
        diffuseDirection = -diffuseDirection;
    }
    ray = {record.point, diffuseDirection};
}

bool LambertianMaterial::readMaterialInfo(const YAML::Node &node) {
    if (!node["ambient-color"] || !node["diffuse-color"] || !node["specular-color"] || !node["specular-exp"] ||
        !node["roughness"]) {
        return false;
    }
    m_ambientColor = Color::toColor(node["ambient-color"].as<std::vector<float>>());
    m_diffuseColor = Color::toColor(node["diffuse-color"].as<std::vector<float>>());
    m_specularColor = Color::toColor(node["specular-color"].as<std::vector<float>>());
    m_constantSpecularExp = node["specular-exp"].as<float>();
    m_constantRoughness = node["roughness"].as<float>();
    return true;
}