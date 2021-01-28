//
// Created by Edge on 2020/7/13.
//

#include "Dielectric.h"
#include "Scene.h"

using namespace std;

Ray DielectricMaterial::calculatePhong(const Scene *scene, const Ray &ray, const IntersectionRecord &record,
                               const LightRecord &shadeLightRecord, ShadeRecord &shadeRecord) const {
    Vec3f outwardNormal;
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
    Vec3f refractedDirection;
    if (!ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
        reflectivity = 1.0f;
        refractivity = 0.0f;
    }

    EmitType emitType = Util::russianRoulette(reflectivity, refractivity);
    if (emitType == EmitType::REFLECTED) {
        return {record.point, ray.velocity.reflect(record.normal)};
    } else if (emitType == EmitType::TRANSMITTED) {
        return {record.point, refractedDirection};
    }
    shadeRecord.attenuation = {1.0f, 1.0f, 1.0f};
}

Ray DielectricMaterial::calculatePhotonMapping(const Scene *scene, const PhotonMappingModel &model, const Ray &ray, const IntersectionRecord &record, ShadeRecord &shadeRecord) const {
    Vec3f outwardNormal;
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
    Vec3f refractedDirection;
    if (!ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
        reflectivity = 1.0f;
        refractivity = 0.0f;
    }

    EmitType emitType = Util::russianRoulette(reflectivity, refractivity);
    if (emitType == EmitType::REFLECTED) {
        return {record.point, ray.velocity.reflect(record.normal)};
    } else if (emitType == EmitType::TRANSMITTED) {
        return {record.point, refractedDirection};
    }
    shadeRecord.attenuation = {1.0f, 1.0f, 1.0f};
}

Material::MaterialProperty DielectricMaterial::getProperty() {
    MaterialProperty property;
    property.type = 1;
    property.param0.x = m_constantReferenceIndex;
    return property;
}

bool DielectricMaterial::readMaterialInfo(const YAML::Node &node) {
    if(!node["reference-index"]) {
        return false;
    }
    m_constantReferenceIndex = node["reference-index"].as<float>();
    return true;
}