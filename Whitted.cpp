//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include "Whitted.h"
#include "Material.h"
#include "HittableList.h"

Color WhittedModel::castRay(const Scene *scene, Ray &ray, double intensity, Color &color, int depth) {
    if (intensity < 1e-6 || depth > 5) {
        return color;
    }
    HitRecord record;
    record.t = -1.0;
    if (scene->m_hittableList->isHit(0.001, ray, record)) {
        LightRecord lightRecord;
        if (record.material->getType() == Material::MaterialType::DIFFUSE) {
            for (auto light: scene->m_lightList) {
                HitRecord testRecord;
                testRecord.t = -1.0;
                Velocity lightDirection = light->m_origin - record.point;
                Ray testRay = {record.point, lightDirection};
                bool isHit = scene->m_hittableList->isHitShadow(0.001, testRay, testRecord);
                lightRecord.addShadedLight(
                        !isHit || lightDirection.length() < (testRecord.point - record.point).length());
            }
            color += intensity * record.material->calculatePhong(scene, ray, record, lightRecord);
            return color;
        } else if (record.material->getType() == Material::MaterialType::DIELECTRIC) {
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
            float reflectivity = Util::schlickApprox(cosine, dielectricMaterial->m_constantReferenceIndex);

            Velocity reflectedDirection = ray.velocity.reflect(record.normal);
            float refractivity = 1.0 - reflectivity;
            Velocity refractedDirection;
            if (ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
                ray = {record.point, refractedDirection};
                castRay(scene, ray, intensity * refractivity, color, depth + 1);
            } else {
                reflectivity = 1.0;
            }

            ray = {record.point, reflectedDirection};
            castRay(scene, ray, intensity * reflectivity, color, depth + 1);
//            color = dielectricMaterial->m_color * color;
            return color;
        }
    }
    return color;
}