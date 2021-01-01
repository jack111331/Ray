//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include "Whitted.h"
#include "HittableList.h"

Color WhittedModel::castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag) {
    HitRecord record;
    if (scene->m_hittableList->isHit(0.001, ray, record)) {
        LightRecord lightRecord;
        for (auto light: scene->m_lightList) {
            HitRecord testRecord;
            Velocity lightDirection = record.point - light->m_origin;
            Ray testRay = {record.point, lightDirection};
            bool isHit = scene->m_hittableList->isHit(0.001, testRay, testRecord);
            lightRecord.addShadedLight(
                    !isHit || lightDirection.length() < (testRecord.point - record.point).length());
        }
        ShadeRecord shadeRecord;
        record.material->calculatePhong(scene, ray, record, lightRecord, shadeRecord);
        if(debugFlag) {
            std::cout << shadeRecord.emit.r << shadeRecord.emit.g << shadeRecord.emit.b << std::endl;
            std::cout << shadeRecord.attenuation.r << shadeRecord.attenuation.g << shadeRecord.attenuation.b << std::endl;
        }
        if(depth > m_maxDepth || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1, debugFlag);
    }
    return m_backgroundColor;
}

