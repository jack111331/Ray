//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include <Lambertian.h>
#include "AmbientOcclusion.h"
#include "HittableList.h"

void AmbientOcclusionPipeline::setupPipeline() {
    AmbientOcclusionModel *model = new AmbientOcclusionModel();
    model->setupBackgroundColor(m_backgroundColor);
    model->setupMaxDepth(m_maxDepth);
    model->setupOcclusionRadius(m_occlusionRadius);
    model->setupOcclusionSampleAmount(m_occlusionSampleAmount);
    setIlluminationModel(model);

    for (int i = 0; i < m_occlusionSampleAmount; ++i) {
        Velocity testDirection = Util::randomSphere();
        model->m_testDirectionList.push_back(testDirection);
    }

    m_camera->initializeScreen();
}

bool AmbientOcclusionPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = RayTracingPipeline::readPipelineInfo(node);
    if (!result || !node["max-depth"] || !node["occlusion-radius"] || !node["occlusion-sample-amount"]) {
        std::cerr << "No require ambient occlusion pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    m_occlusionRadius = node["occlusion-radius"].as<float>();
    m_occlusionSampleAmount = node["occlusion-sample-amount"].as<int>();

    return true;
}

Color AmbientOcclusionModel::castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag) {
    HitRecord record;
    if (scene->m_hittableList->isHit(ray, record)) {
        LightRecord lightRecord;
        for (auto light: scene->m_lightList) {
            HitRecord testRecord;
            Velocity lightDirection = light->m_origin - record.point;
            Ray testRay = {record.point, lightDirection};
            bool isHit = scene->m_hittableList->isHit(testRay, testRecord);
            lightRecord.addShadedLight(
                    !isHit || lightDirection.length() < (testRecord.point - record.point).length());
        }
        ShadeRecord shadeRecord;
        // temporary change material ambient color
        Color originalAmbientColor;
        if (record.material->getType() == Material::LAMBERTIAN) {
            LambertianMaterial *material = (LambertianMaterial *) record.material;
            originalAmbientColor = material->m_ambientColor;
            float totalHitRay = 0;
            const int ambientOcclusionSampleRayAmount = m_occlusionSampleAmount;

            for (int i = 0; i < ambientOcclusionSampleRayAmount; ++i) {
                HitRecord testRecord;
                Ray testRay = {record.point, m_testDirectionList[i]};
                bool isHitted = scene->m_hittableList->isHit(testRay, testRecord);
                float hitRay = (testRecord.point - record.point).length() / m_occlusionRadius;
                if (isHitted && hitRay <= 1.0f) {
                    totalHitRay += hitRay;
                }
            }
            float occlusion = 1.0f - ((float) totalHitRay / (float) ambientOcclusionSampleRayAmount);
            material->m_ambientColor = occlusion * material->m_ambientColor;
        }
        record.material->calculatePhong(scene, ray, record, lightRecord, shadeRecord);
        if (record.material->getType() == Material::LAMBERTIAN) {
            LambertianMaterial *material = (LambertianMaterial *) record.material;
            material->m_ambientColor = originalAmbientColor;
        }
        if (debugFlag) {
            std::cout << shadeRecord.emit.r << " " << shadeRecord.emit.g << " " << shadeRecord.emit.b << std::endl;
            std::cout << shadeRecord.attenuation.r << " " << shadeRecord.attenuation.g << " "
                      << shadeRecord.attenuation.b << std::endl;
        }
        if (depth > m_maxDepth || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1, debugFlag);
    }
    return m_backgroundColor;
}

