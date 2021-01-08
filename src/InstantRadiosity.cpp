//
// Created by Edge on 2020/11/19.
//

#include <glm/gtc/matrix_transform.hpp>
#include <GLUtil.h>
#include "InstantRadiosity.h"
#include "GeometryGroupObj.h"

void InstantRadiosityPipeline::setupPipeline() {
    if(!m_scene || !m_camera) {
        std::cerr << "No camera or scene specified" << std::endl;
        exit(1);
    }
    shadingSetting = new PhongShadingSetting();
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

    m_shadingPass = new PhongShadingPass(shadingSetting);
    m_shadingPass->setupPassSetting(shadingSetting);

    m_scene->m_group->createVAO(m_objectList);
}

void InstantRadiosityPipeline::renderAllPass() {
    m_shadingPass->renderPass(m_objectList);
}