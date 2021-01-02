//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include "Whitted.h"
#include "HittableList.h"

void WhittedPipeline::setupPipeline() {
    WhittedModel *model = new WhittedModel();
    model->setupBackgroundColor(m_backgroundColor);
    model->setupMaxDepth(m_maxDepth);
    setIlluminationModel(model);

    m_camera->initializeScreen();
}

bool WhittedPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = RayTracingPipeline::readPipelineInfo(node);
    if(!result || !node["max-depth"]) {
        std::cerr << "No require whitted pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    return true;
}

Color WhittedModel::castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag) {
    HitRecord record;
    if (scene->m_hittableList->isHit(ray, record)) {
        LightRecord lightRecord;
        for (auto light: scene->m_lightList) {
            HitRecord testRecord;
            Velocity lightDirection = record.point - light->m_origin;
            Ray testRay = {record.point, lightDirection};
            bool isHit = scene->m_hittableList->isHit(testRay, testRecord);
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

