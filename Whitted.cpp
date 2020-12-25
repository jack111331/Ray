//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include "Whitted.h"
#include "Material.h"
#include "HittableList.h"

Color WhittedModel::castRay(const Scene *scene, Ray &ray, int depth) {
    HitRecord record;
    if (scene->m_hittableList->isHit(0.001, ray, record)) {
        LightRecord lightRecord;
        for (auto light: scene->m_lightList) {
            HitRecord testRecord;
            testRecord.t = -1.0;
            Velocity lightDirection = light->m_origin - record.point;
            Ray testRay = {record.point, lightDirection};
            bool isHit = scene->m_hittableList->isHit(0.001, testRay, testRecord);
            lightRecord.addShadedLight(
                    !isHit || lightDirection.length() < (testRecord.point - record.point).length());
        }
        ShadeRecord shadeRecord;
        record.material->calculatePhong(scene, ray, record, lightRecord, shadeRecord);
        if(depth > 5 || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1);
    }
    return m_backgroundColor;
}