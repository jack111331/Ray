//
// Created by Edge on 2020/7/13.
//

#include "Dielectric.h"
#include "Scene.h"

using namespace std;

istream &operator>>(istream &is, DielectricMaterial &rhs) {
    is >> rhs.m_constantReferenceIndex;
}

Color DielectricMaterial::calculatePhong(const Scene *scene, Ray &ray, const HitRecord &record,
                               const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const {
    Velocity outwardNormal;
    float cosine;
    float niOverNt;
    if (ray.velocity.dot(record.normal) > 0.0) {
        outwardNormal = -record.normal;
        niOverNt = m_constantReferenceIndex;
        cosine = m_constantReferenceIndex * ray.velocity.dot(record.normal) /
                 ray.velocity.length();
    } else {
        outwardNormal = record.normal;
        niOverNt = 1.0 / m_constantReferenceIndex;
        cosine = -ray.velocity.dot(record.normal) / ray.velocity.length();
    }
    float reflectivity = Util::schlickApprox(cosine, m_constantReferenceIndex);
    float refractivity = 1.0 - reflectivity;
    Velocity refractedDirection;
    if (!ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
        reflectivity = 1.0f;
        refractivity = 0.0f;
    }

    EmitType emitType = Util::russianRoulette(reflectivity, refractivity);
    if (emitType == EmitType::REFLECTED) {
        ray = {record.point, ray.velocity.reflect(record.normal)};
    } else if (emitType == EmitType::TRANSMITTED) {
        ray = {record.point, refractedDirection};
    }
    shadeRecord.attenuation = {1.0f, 1.0f, 1.0f};
    // FIXME in local shading color
    return shadeRecord.attenuation;
}

void DielectricMaterial::calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &model, Ray &ray, const HitRecord &record, ShadeRecord &shadeRecord) const {
    Velocity outwardNormal;
    float cosine;
    float niOverNt;
    if (ray.velocity.dot(record.normal) > 0.0) {
        outwardNormal = -record.normal;
        niOverNt = m_constantReferenceIndex;
        cosine = m_constantReferenceIndex * ray.velocity.dot(record.normal) /
                 ray.velocity.length();
    } else {
        outwardNormal = record.normal;
        niOverNt = 1.0 / m_constantReferenceIndex;
        cosine = -ray.velocity.dot(record.normal) / ray.velocity.length();
    }
    float reflectivity = Util::schlickApprox(cosine, m_constantReferenceIndex);
    float refractivity = 1.0 - reflectivity;
    Velocity refractedDirection;
    if (!ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
        reflectivity = 1.0f;
        refractivity = 0.0f;
    }

    EmitType emitType = Util::russianRoulette(reflectivity, refractivity);
    if (emitType == EmitType::REFLECTED) {
        ray = {record.point, ray.velocity.reflect(record.normal)};
    } else if (emitType == EmitType::TRANSMITTED) {
        ray = {record.point, refractedDirection};
    }
    shadeRecord.attenuation = {1.0f, 1.0f, 1.0f};
}

bool DielectricMaterial::readMaterialInfo(const YAML::Node &node) {
    if(!node["reference-index"]) {
        return false;
    }
    m_constantReferenceIndex = node["reference-index"].as<float>();
    return true;
}