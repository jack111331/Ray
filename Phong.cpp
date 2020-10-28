//
// Created by Edge on 2020/10/14.
//

#include "Phong.h"
#include "Material.h"
#include "HittableList.h"

Color PhongModel::castRay(const Scene *scene, Ray &ray, double intensity, Color &color, int depth) {
    if (intensity < 1e-6 || depth > 5) {
        return {0.0, 0.0, 0.0};
    }
    HitRecord record;
    record.t = -1.0;
    if (scene->m_hittableList->isHit(0.001, ray, record)) {
        if (record.material->m_constantReflectionRatio < 1e-6) {
            LightRecord lightRecord;
            for (auto light: scene->m_lightList) {
                HitRecord testRecord;
                testRecord.t = -1.0;
                Velocity lightDirection = light->m_origin - record.point;
                Ray testRay = {record.point, lightDirection};
                bool isHit = scene->m_hittableList->isHit(0.001, testRay, testRecord);
                lightRecord.addShadedLight(!isHit || lightDirection.length() < (testRecord.point - record.point).length());
            }
            color = record.material->calculatePhong(scene, ray, record, lightRecord);
            return color;
        } else {
            LightRecord lightRecord;
            for (auto light: scene->m_lightList) {
                HitRecord testRecord;
                testRecord.t = -1.0;
                Ray testRay = {record.point, light->m_origin - record.point};
                lightRecord.addShadedLight(!scene->m_hittableList->isHit(0.001, testRay, testRecord));
            }
            Color calculateColor = record.material->calculatePhong(scene, ray, record, lightRecord);
            ray = {record.point, ray.velocity.reflect(record.normal)};
            Color castedColor = {.0, .0, .0};
            castRay(scene, ray, intensity * record.material->m_constantReflectionRatio, castedColor, depth + 1);
            color = (1.0 - record.material->m_constantReflectionRatio) * calculateColor +
                    record.material->m_constantReflectionRatio *
                    castedColor;
            return color;
        }
    }
    return {0.0, 0.0, 0.0};
}
