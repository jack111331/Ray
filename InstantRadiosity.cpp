//
// Created by Edge on 2020/11/19.
//

#include <glm/gtc/matrix_transform.hpp>
#include <GLUtil.h>
#include "InstantRadiosity.h"
#include "HittableList.h"

void InstantRadiosityPipeline::setupPipeline() {
    if(!m_scene || !m_camera) {
        // TODO error handler
    }
    m_shadingPass = new PhongShadingPass();
    shadingSetting = new PhongPassSetting();
    shadingSetting->m_projectionMatrix = glm::perspective(glm::radians(m_camera->m_fov * 2.0f),
                                                          (float) m_camera->m_width / (float) m_camera->m_height,
                                                          0.1f, 100.0f);
    shadingSetting->m_viewMatrix = glm::lookAt(
            glm::vec3(m_camera->m_eyeCoord.x, m_camera->m_eyeCoord.y, m_camera->m_eyeCoord.z),
            glm::vec3(m_camera->m_eyeCoord.x + m_camera->m_direction.x,
                      m_camera->m_eyeCoord.y + m_camera->m_direction.y,
                      m_camera->m_eyeCoord.z + m_camera->m_direction.z),
            glm::vec3(m_camera->m_up.x, m_camera->m_up.y, m_camera->m_up.z));
    shadingSetting->m_camera = m_camera;
    shadingSetting->m_lightList = m_scene->m_lightList;
    shadingSetting->m_lightUBO = RayUtil::generateLightUBO(shadingSetting->m_lightList);

    m_shadingPass->setupPassSetting(shadingSetting);

    auto hittableList = m_scene->m_hittableList->m_hittableList;
    for (auto hittable : hittableList) {
        std::vector<ObjectInfo> objectInfoList = hittable->createVAO(hittable->m_material);
        for (auto objectInfo: objectInfoList) {
            m_objectList.push_back(new ShadeObject{objectInfo, hittable->m_material});
        }
    }
}

void InstantRadiosityPipeline::renderAllPass() {
    m_shadingPass->renderPass(m_objectList);
}