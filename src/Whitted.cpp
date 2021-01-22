//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include <ShaderInclude.h>
#include <GL/glew.h>
#include <Lambertian.h>
#include "Whitted.h"
#include "GroupObj.h"
#include "ShaderProgram.h"
#include "GroupBVHTranslator.h"

void WhittedCPUPipeline::setupPipeline() {
    WhittedModel *model = new WhittedModel();
    model->setupBackgroundColor(m_backgroundColor);
    model->setupMaxDepth(m_maxDepth);
    setIlluminationModel(model);

    m_camera->initializeScreen();
}

bool WhittedCPUPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = CPURayTracingPipeline::readPipelineInfo(node);
    if(!result || !node["max-depth"]) {
        std::cerr << "No require whitted pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    return true;
}

void WhittedGPUPipeline::setupPipeline() {
    m_rayTracingShader = new ShaderProgram(ShaderInclude::load("resource/shader/ray_tracing_shading/whitted_tracing.cs"));
    std::vector<Vec4f> lightList;
    for(auto light: m_scene->m_lightList) {
        lightList.push_back(Vec4f(light->getLightOrigin(), 1.0f));
    }

    m_rayTracingShader->bind();
    glGenBuffers(1, &m_lightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_lightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, lightList.size() * sizeof(Vec4f), lightList.data(), GL_DYNAMIC_DRAW);


    m_rayTracingShader->uniform1i("number_of_lights", lightList.size());

    m_rayTracingShader->bindSSBOBuffer(m_translator->m_meshIndicesSSBO, 1);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_meshVerticesSSBO, 2);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_meshNormalsSSBO, 3);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_materialInstanceSSBO, 5);
    m_rayTracingShader->bindSSBOBuffer(m_lightSSBO, 6);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_materialSSBO, 7);
    // TODO bind material

}

bool WhittedGPUPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = GPURayTracingPipeline::readPipelineInfo(node);
    if(!result || !node["max-depth"]) {
        std::cerr << "No require whitted pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    return true;
}

Vec3f WhittedModel::castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag) {
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
        record.material->calculatePhong(scene, ray, record, lightRecord, shadeRecord);
        if(debugFlag) {
            std::cout << shadeRecord.emit << std::endl;
            std::cout << shadeRecord.attenuation << std::endl;
        }
        if(depth > m_maxDepth || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1, debugFlag);
    }
    return m_backgroundColor;
}

