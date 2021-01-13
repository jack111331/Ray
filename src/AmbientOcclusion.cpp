//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include <Lambertian.h>
#include "AmbientOcclusion.h"
#include "GroupObj.h"

void AmbientOcclusionPipeline::setupPipeline() {
    AmbientOcclusionModel *model = new AmbientOcclusionModel();
    model->setupBackgroundColor(m_backgroundColor);
    model->setupMaxDepth(m_maxDepth);
    model->setupOcclusionRadius(m_occlusionRadius);
    model->setupOcclusionSampleAmount(m_occlusionSampleAmount);
    setIlluminationModel(model);

    for (int i = 0; i < m_occlusionSampleAmount; ++i) {
        Vec3f testDirection = Util::randomSphere();
        model->m_testDirectionList.push_back(testDirection);
    }

    m_camera->initializeScreen();
}

bool AmbientOcclusionPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = CPURayTracingPipeline::readPipelineInfo(node);
    if (!result || !node["max-depth"] || !node["occlusion-radius"] || !node["occlusion-sample-amount"]) {
        std::cerr << "No require ambient occlusion pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    m_occlusionRadius = node["occlusion-radius"].as<float>();
    m_occlusionSampleAmount = node["occlusion-sample-amount"].as<int>();

    return true;
}

Vec3f AmbientOcclusionModel::castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag) {
    IntersectionRecord record;
    if (scene->m_group->isHit(ray, record)) {
        LightRecord lightRecord;
        for (auto light: scene->m_lightList) {
            IntersectionRecord testRecord;
            Vec3f lightDirection = light->m_origin - record.point;
            Ray testRay = {record.point, lightDirection};
            bool isHit = scene->m_group->isHit(testRay, testRecord);
            lightRecord.addShadedLight(
                    !isHit || lightDirection.length() < (testRecord.point - record.point).length());
        }
        ShadeRecord shadeRecord;
        // temporary change material ambient color
        Vec3f originalAmbientColor;
        if (record.material->getType() == Material::LAMBERTIAN) {
            LambertianMaterial *material = (LambertianMaterial *) record.material;
            originalAmbientColor = material->m_ambientColor;
            int totalHitRay = 0;
            const int ambientOcclusionSampleRayAmount = m_occlusionSampleAmount;

            for (int i = 0; i < ambientOcclusionSampleRayAmount; ++i) {
                IntersectionRecord testRecord;
                Vec3f testDirection = Util::randomSphere();
                if(testDirection.dot(record.normal) < 0.0) {
                    testDirection = -testDirection;
                }
                Ray testRay = {record.point, testDirection};
                bool isHitted = scene->m_group->isHit(testRay, testRecord, 0.00001f);
                if (isHitted && (testRecord.point - record.point).length() <= m_occlusionRadius) {
                    totalHitRay++;
                }
            }
            float occlusion = 1.0f - ((float) totalHitRay / (float) ambientOcclusionSampleRayAmount);
            material->m_ambientColor = occlusion * Vec3f{1.0, 1.0, 1.0};
        }
        record.material->calculatePhong(scene, ray, record, lightRecord, shadeRecord);
        if (record.material->getType() == Material::LAMBERTIAN) {
            LambertianMaterial *material = (LambertianMaterial *) record.material;
            material->m_ambientColor = originalAmbientColor;
        }
        if (debugFlag) {
            std::cout << shadeRecord.emit << std::endl;
            std::cout << shadeRecord.attenuation << std::endl;
        }
        if (depth > m_maxDepth || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1, debugFlag);
    }
    return m_backgroundColor;
}

