//
// Created by Edge on 2020/10/31.
//

#include <Photon.h>
#include <ONB.h>
#include "Lambertian.h"

using namespace std;

Ray LambertianMaterial::calculatePhong(const Scene *scene, const Ray &ray, const IntersectionRecord &record,
                                        const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const {
    // calculate diffuse and specular
    Vec3f normal = record.normal.normalize();
    if (ray.velocity.dot(normal) > .0f) {
        normal = -normal;
    }
    float diffuseIntensity = .0f;
    float specularIntensity = .0f;
    if (shadeLightRecord.isShadedLightList.size() != scene->m_lightList.size()) {
        std::cerr << "Shade light amount isn't consistent with scene light list" << std::endl;
        exit(1);
    }
    for (int i = 0; i < scene->m_lightList.size(); ++i) {
        if (shadeLightRecord.isShadedLightList[i]) {
            Vec3f lightDirection = (scene->m_lightList[i]->getLightOrigin() - record.point).normalize();
            diffuseIntensity += max(.0f, normal.dot(lightDirection));
            Vec3f halfwayVector = (lightDirection - ray.velocity.normalize()).normalize();
            specularIntensity += pow(max(.0f, normal.dot(halfwayVector)), m_constantSpecularExp);
        }
    }
    // FIXME result color bug
    Vec3f resultColor = m_ambientColor + min(1.0f, diffuseIntensity) * m_diffuseColor +
                        min(1.0f, specularIntensity) * m_specularColor;
    shadeRecord.emit = {.0f, .0f, .0f};
    shadeRecord.attenuation =
            resultColor * (m_diffuseTexture ? m_diffuseTexture->getColor(record.texCoord) : Vec3f{1.0f, 1.0f,
                                                                                                  1.0f});
    return {Vec3f(.0f, .0f, .0f), Vec3f(.0f, .0f, .0f)};

}

Ray
LambertianMaterial::calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &photonMappingModel, const Ray &ray,
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
    shadeRecord.attenuation =
            m_diffuseColor * (m_diffuseTexture ? m_diffuseTexture->getColor(record.texCoord) : Vec3f{1.0f, 1.0f, 1.0f});
    Vec3f diffuseDirection = Util::randomSphere();
    // if ray velocity dot record normal is negative and diffuse direction dot record normal is positive
    // or ray velocity dot record normal is positive and diffuse direction dot record normal is negative
    // then we don't need to negative the diffuse direction, otherwise we should fix diffuse direction
    if (ray.velocity.dot(record.normal) * diffuseDirection.dot(record.normal) > 0.0) {
        diffuseDirection = -diffuseDirection;
    }
    return {record.point, diffuseDirection};
}

Vec3f LambertianMaterial::calculateScatter(const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord, float &pdf) const {
    ONB onb(record.ffNormal);
    Vec3f scatteredDirection = onb.toWorldCoordSystem(Util::randomCosineDirection());
    scatteredDirection.normalize();
    shadeRecord.emit = Vec3f(.0f, .0f, .0f);
    shadeRecord.attenuation = m_diffuseColor;
    const float PI = acos(-1);
    pdf = onb.m_direction.dot(scatteredDirection) / PI;
    return scatteredDirection;
}

float LambertianMaterial::calculateScatterPdf(const Ray &ray, const IntersectionRecord &record,
                                              const Vec3f &scatteredDirection) const {
    const float PI = acos(-1);
    float pdf = record.ffNormal.dot(scatteredDirection);
    return pdf < 0 ? 0 : pdf / PI;
}

bool LambertianMaterial::readMaterialInfo(const YAML::Node &node) {
    if (!node["ambient-color"] || !node["diffuse-color"] || !node["specular-color"] || !node["specular-exp"] ||
        !node["roughness"]) {
        return false;
    }
    m_ambientColor = Vec3f::toVec3f(node["ambient-color"].as<std::vector<float>>());
    m_diffuseColor = Vec3f::toVec3f(node["diffuse-color"].as<std::vector<float>>());
    m_specularColor = Vec3f::toVec3f(node["specular-color"].as<std::vector<float>>());
    m_constantSpecularExp = node["specular-exp"].as<float>();
    m_constantRoughness = node["roughness"].as<float>();
    return true;
}

Material::MaterialProperty LambertianMaterial::getProperty() {
    MaterialProperty property;
    property.type = 0;
    property.param0 = Vec4f(m_diffuseColor, 1.0f);
    return property;
}