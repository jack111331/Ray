//
// Created by Edge on 2020/10/14.
//

#include <Dielectric.h>
#include <ShaderInclude.h>
#include <GL/glew.h>
#include "PathTrace.h"
#include "GroupObj.h"
#include "ShaderProgram.h"
#include "GroupBVHTranslator.h"

void PathTraceCPUPipeline::setupPipeline() {
    PathTraceModel *model = new PathTraceModel();
    model->setupBackgroundColor(m_backgroundColor);
    model->setupMaxDepth(m_maxDepth);
    setIlluminationModel(model);

    m_camera->initializeScreen();
}

bool PathTraceCPUPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = CPURayTracingPipeline::readPipelineInfo(node);
    if(!result || !node["max-depth"]) {
        std::cerr << "No require path trace pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    return true;
}

void PathTraceGPUPipeline::setupPipeline() {
    m_screenShader = new ShaderProgram(ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.vs"),
                                       ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading_divide.fs"));

    m_rayTracingShader = new ShaderProgram(ShaderInclude::load("resource/shader/ray_tracing_shading/path_tracing.cs"));
    m_rayTracingShader->bind();
    m_rayTracingShader->uniform1i("max_depth", m_maxDepth);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_meshIndicesSSBO, 1);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_meshVerticesSSBO, 2);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_meshNormalsSSBO, 3);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_materialInstanceSSBO, 5);
    m_rayTracingShader->bindSSBOBuffer(m_lightSSBO, 6);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_materialSSBO, 7);
    // TODO bind material

}

bool PathTraceGPUPipeline::readPipelineInfo(const YAML::Node &node) {
    bool result = GPURayTracingPipeline::readPipelineInfo(node);
    if(!result || !node["max-depth"]) {
        std::cerr << "No require path trace pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    return true;
}

void PathTraceGPUPipeline::drawScreen() {
    m_screenShader->bind();
    glBindVertexArray(m_quadVao);

    m_screenShader->uniform1i("gScreen", 0);
    m_screenShader->uniform1i("casted_ray", m_cumulatedRay);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

Vec3f PathTraceModel::castRay(const Scene *scene, const Ray &ray, int depth, bool debugFlag) {
    IntersectionRecord record;
    if (scene->m_group->isHit(ray, record)) {
        ShadeRecord shadeRecord;

        float samplePdf;
        Ray scatteredRay = {record.point, record.material->calculateScatter(ray, record, shadeRecord, samplePdf)};

        if(debugFlag) {
            std::cout << "Emit is " << shadeRecord.emit << std::endl;
            std::cout << "Attenuation is " << shadeRecord.attenuation << std::endl;
            std::cout << "Depth is " << depth << std::endl;
        }
        if(depth > m_maxDepth || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit;
        }
        float scatterDistributionPdf = record.material->calculateScatterPdf(ray, record, scatteredRay.velocity);
        Vec3f rayColor = castRay(scene, scatteredRay, depth + 1, debugFlag);
        Vec3f radiance = shadeRecord.emit + shadeRecord.attenuation * scatterDistributionPdf * rayColor / samplePdf;
//        if(debugFlag) {
//            std::cout << "Radiance is " << radiance << ", Depth " << depth << std::endl;
//        }
        return radiance;
    }
    return m_backgroundColor;
}

