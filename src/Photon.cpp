//
// Created by Edge on 2020/10/21.
//

#include <Photon.h>
#include <Dielectric.h>
#include "GroupObj.h"

using namespace std;

void PhotonMappingPipeline::setupPipeline() {
    if (!m_photonAmount || m_photonPower < 1e-6) {
        std::cerr << "photon amount or power not specified" << std::endl;
        exit(1);
    }
    PhotonMappingModel *model = new PhotonMappingModel();
    model->setup(m_scene, m_photonAmount, m_photonPower, m_photonTraceDepth);
    model->setupBackgroundColor(m_backgroundColor);
    setIlluminationModel(model);
}

void PhotonMappingModel::setup(Scene *scene, int photonAmount, float photonPower, int photonTraceDepth) {
    m_photonAmount = photonAmount;
    m_photonPower = photonPower;
    m_photonTraceDepth = photonTraceDepth;

    for (auto light : scene->m_lightList) {
        for (int i = 0; i < m_photonAmount; ++i) {
            Ray photonTraceRay;
            Vec3f photonPosition;
            Vec3f photonDirection;
            do {
                // determine photon position and emit direction
                photonPosition = light->getLightOrigin();
                photonDirection = Util::randomSphere();

                // photon tracing and store photon on hitted surface
                photonTraceRay = {photonPosition, photonDirection};
            } while (!photonTracing(scene, photonTraceRay, m_photonPower, 0));
        }
    }
}

bool PhotonMappingModel::photonTracing(const Scene *scene, Ray &ray, float power, int depth) {
    if (power < 1e-6 || depth > m_photonTraceDepth) {
        return true;
    }
    // hit photon and store photon
    IntersectionRecord record;
    if (scene->m_group->isHit(ray, record)) {
        // calculate refractivity and reflectivity
        float reflectivity = 0.0, refractivity = 0.0;
        Vec3f refractedDirection;
        if (record.material->getType() == Material::MaterialType::DIELECTRIC) {
            DielectricMaterial *dielectricMaterial = (DielectricMaterial *) record.material;
            Vec3f outwardNormal;
            float cosine;
            float niOverNt;
            if (ray.velocity.dot(record.normal) > 0.0) {
                outwardNormal = -record.normal;
                niOverNt = dielectricMaterial->m_constantReferenceIndex;
                cosine = dielectricMaterial->m_constantReferenceIndex * ray.velocity.dot(record.normal) /
                         ray.velocity.length();
            } else {
                outwardNormal = record.normal;
                niOverNt = 1.0 / dielectricMaterial->m_constantReferenceIndex;
                cosine = -ray.velocity.dot(record.normal) / ray.velocity.length();
            }
            reflectivity = Util::schlickApprox(cosine, dielectricMaterial->m_constantReferenceIndex);
            if (!ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
                reflectivity = 1.0;
            }
            refractivity = 1.0 - reflectivity;
        } else if (record.material->getType() == Material::MaterialType::LAMBERTIAN) {
            Photon photon(
                    record.point,
                    power,
                    ray.velocity,
                    Photon::Flag::NORMAL
            );
            m_kdTree->insert(photon);
        } else if (record.material->getType() == Material::MaterialType::AREALIGHT) {
            Photon photon(
                    record.point,
                    power,
                    ray.velocity,
                    Photon::Flag::NORMAL
            );
            m_kdTree->insert(photon);
        }
        EmitType emitType = Util::russianRoulette(reflectivity, refractivity);
        if (emitType == EmitType::ABSORBED) {
            // reflected or transmitted ray
            // influenced power
        } else if (emitType == EmitType::REFLECTED) {
            Vec3f reflectedDirection = ray.velocity.reflect(record.normal);
            ray = {record.point, reflectedDirection};
            photonTracing(scene, ray, power, depth + 1);
        } else if (emitType == EmitType::TRANSMITTED) {
            ray = {record.point, refractedDirection};
            photonTracing(scene, ray, power, depth + 1);
        }
        return true;
    }
    return false;
}

Vec3f PhotonMappingModel::castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag) {
    IntersectionRecord record;
    if (scene->m_group->isHit(ray, record)) {
        ShadeRecord shadeRecord;
        record.material->calculatePhotonMapping(scene, *this, ray, record, shadeRecord);
        if (depth > 4 || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1, debugFlag);
    }
    return m_backgroundColor;
}