//
// Created by Edge on 2020/10/21.
//

#include <Photon.h>
#include <Dielectric.h>
#include "HittableList.h"

using namespace std;

void PhotonMappingModel::setup(Scene *scene, int photonAmount, float photonPower, int photonTraceDepth) {
    m_photonAmount = photonAmount;
    m_photonPower = photonPower;
    m_photonTraceDepth = photonTraceDepth;

    for (auto light : scene->m_lightList) {
        for (int i = 0; i < m_photonAmount; ++i) {
            Ray photonTraceRay;
            Coord photonPosition;
            Velocity photonDirection;
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
    HitRecord record;
    if (scene->m_hittableList->isHit(0.001, ray, record)) {
        // calculate refractivity and reflectivity
        float reflectivity = 0.0, refractivity = 0.0;
        Velocity refractedDirection;
        if (record.material->getType() == Material::MaterialType::DIELECTRIC) {
            DielectricMaterial *dielectricMaterial = (DielectricMaterial *) record.material;
            Velocity outwardNormal;
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
            Velocity reflectedDirection = ray.velocity.reflect(record.normal);
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

Color PhotonMappingModel::castRay(const Scene *scene, Ray &ray, int depth) {
    HitRecord record;
    if (scene->m_hittableList->isHit(0.00001, ray, record)) {
        ShadeRecord shadeRecord;
        record.material->calculatePhotonMapping(scene, *this, ray, record, shadeRecord);
        if(depth > 4 || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1);
    }
    return m_backgroundColor;
}