//
// Created by Edge on 2020/12/31.
//

#include <GL/glew.h>
#include <Lambertian.h>
#include <GLUtil.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "HittableList.h"
#include "AmbientOcclusionVolumeShadingPipeline.h"

AmbientOcclusionVolumeGBufferPass::AmbientOcclusionVolumeGBufferPass(PassSetting *passSetting) : Pass(passSetting),
                                                                                                 m_outputFrameTextureId{
                                                                                                         0},
                                                                                                 m_outputFrameDepthStencilBufferId(
                                                                                                         0) {
    m_shader = new ShaderProgram(ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_deffered_gbuffer.vs"),
                                 ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_deffered_gbuffer.fs"));

    glGenFramebuffers(1, &m_outputFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glGenTextures(3, m_outputFrameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_passSetting->m_camera->m_width, m_passSetting->m_camera->m_height,
                 0,
                 GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputFrameTextureId[0], 0);

    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_passSetting->m_camera->m_width, m_passSetting->m_camera->m_height,
                 0,
                 GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D, m_outputFrameTextureId[1], 0);

    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_passSetting->m_camera->m_width, m_passSetting->m_camera->m_height,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 2, GL_TEXTURE_2D, m_outputFrameTextureId[2], 0);

    glGenRenderbuffers(1, &m_outputFrameDepthStencilBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_outputFrameDepthStencilBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_passSetting->m_camera->m_width,
                          m_passSetting->m_camera->m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              m_outputFrameDepthStencilBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Ambient occlusion volume deffered g-buffer pass frame buffer object successful setup"
                  << std::endl;
    } else {
        std::cout << "Ambient occlusion volume deffered g-buffer pass frame buffer object failed setup" << std::endl;
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AmbientOcclusionVolumeGBufferPass::renderPass(const std::vector<ShadeObject *> &shadingList) {
    Pass::renderPass(shadingList);

    AmbientOcclusionVolumeShadingSetting *setting = (AmbientOcclusionVolumeShadingSetting *) m_passSetting;

    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader binding
    m_shader->bind();

    if (setting) {
        int renderCount = 0;
        for (auto object: shadingList) {
            if (object->m_material->getType() == Material::MaterialType::LAMBERTIAN) {
                // object binding
                glBindVertexArray(object->m_objectInfo.m_vao);

                m_shader->uniformMat4f("projection", setting->m_projectionMatrix);
                m_shader->uniformMat4f("view", setting->m_viewMatrix);

                // world transformation
                // TODO Bundle model matrix
                glm::mat4 model = glm::mat4(1.0f);
                m_shader->uniformMat4f("model", model);

                const LambertianMaterial *material = (LambertianMaterial *) object->m_material;
                m_shader->uniform3f("objectColor", material->m_diffuseColor.r, material->m_diffuseColor.g,
                                    material->m_diffuseColor.b);

                // draw call
                glDrawElements(GL_TRIANGLES, object->m_objectInfo.m_indicesAmount, GL_UNSIGNED_INT, 0);
                renderCount++;
            }
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

AmbientOcclusionVolumeBoundingPass::AmbientOcclusionVolumeBoundingPass(PassSetting *passSetting) : Pass(passSetting),
                                                                                                   m_outputFrameTextureId{
                                                                                                           0},
                                                                                                   m_outputFrameDepthStencilBufferId(
                                                                                                           0) {
    m_shader = new ShaderProgram(ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_bounding.vs"),
                                                     ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_bounding.gs"),
                                                                         ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_bounding.fs"));

    glGenFramebuffers(1, &m_outputFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glGenTextures(1, &m_outputFrameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_passSetting->m_camera->m_width, m_passSetting->m_camera->m_height,
                 0,
                 GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputFrameTextureId, 0);

    glGenRenderbuffers(1, &m_outputFrameDepthStencilBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_outputFrameDepthStencilBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_passSetting->m_camera->m_width,
                          m_passSetting->m_camera->m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              m_outputFrameDepthStencilBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Ambient occlusion volume bounding pass frame buffer object successful setup" << std::endl;
    } else {
        std::cout << "Ambient occlusion volume bounding pass frame buffer object failed setup" << std::endl;
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AmbientOcclusionVolumeBoundingPass::renderPass(const std::vector<ShadeObject *> &shadingList) {
    Pass::renderPass(shadingList);

    AmbientOcclusionVolumeShadingSetting *setting = (AmbientOcclusionVolumeShadingSetting *) m_passSetting;

    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_BLEND);

    // shader binding
    m_shader->bind();

    if (setting) {
        for (auto object: shadingList) {
            // object binding
            glBindVertexArray(object->m_objectInfo.m_vao);
            // world transformation
            // TODO Bundle model matrix
            glm::mat4 model = glm::mat4(1.0f);
            m_shader->uniformMat4f("model", model);
            m_shader->uniformMat4f("worldToScreenMatrix", setting->m_projectionMatrix * setting->m_viewMatrix);
            m_shader->uniform1f("maxObscuranceDistance", 1.5f);
            m_shader->uniform1f("invMaxObscuranceDistance", 1.0 / 1.5f);
            m_shader->uniform1f("falloffExponent", 3.0f);

            m_shader->uniform1i("gPosition", 0);
            m_shader->uniform1i("gNormal", 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_inputFrameTextureId[0]);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, m_inputFrameTextureId[1]);

            const Coord &eyeCoord = setting->m_camera->m_eyeCoord;
            m_shader->uniform3f("viewPos", eyeCoord.x, eyeCoord.y, eyeCoord.z);

            // draw call
            glDrawElements(GL_TRIANGLES, object->m_objectInfo.m_indicesAmount, GL_UNSIGNED_INT, 0);
        }
    }
    glDisable(GL_DEPTH_CLAMP);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

AmbientOcclusionVolumeShadingPass::AmbientOcclusionVolumeShadingPass(PassSetting *passSetting) : Pass(passSetting),
                                                                                                 m_outputFrameTextureId(
                                                                                                         0),
                                                                                                 m_outputFrameDepthStencilBufferId(
                                                                                                         0) {
    m_shader = new ShaderProgram(ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_deffered_shading.vs"),
                                                     ShaderInclude::load("resource/shader/local_shading/ambient_occlusion_volume_deffered_shading.fs"));

    AmbientOcclusionVolumeShadingSetting *setting = (AmbientOcclusionVolumeShadingSetting *) passSetting;

    glGenFramebuffers(1, &m_outputFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glGenTextures(1, &m_outputFrameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, setting->m_camera->m_width, setting->m_camera->m_height,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputFrameTextureId, 0);

    glGenRenderbuffers(1, &m_outputFrameDepthStencilBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_outputFrameDepthStencilBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, setting->m_camera->m_width,
                          setting->m_camera->m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              m_outputFrameDepthStencilBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Ambient occlusion volume deffered shading pass frame buffer object successful setup" << std::endl;
    } else {
        std::cout << "Ambient occlusion volume deffered shading pass frame buffer object failed setup" << std::endl;
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // Setup plane VAO
    glGenVertexArrays(1, &m_quadVao);
    uint32_t quadVbo;
    glGenBuffers(1, &quadVbo);
    glBindVertexArray(m_quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void AmbientOcclusionVolumeShadingPipeline::setupPipeline() {
    if (!m_scene || !m_camera) {
        std::cerr << "No camera or scene specified" << std::endl;
        exit(1);
    }
    shadingSetting = new AmbientOcclusionVolumeShadingSetting();
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

    AmbientOcclusionVolumeGBufferPass *gBufferPass = new AmbientOcclusionVolumeGBufferPass(shadingSetting);
    AmbientOcclusionVolumeBoundingPass *boundingPass = new AmbientOcclusionVolumeBoundingPass(shadingSetting);
    boundingPass->specifyInput(
            (size_t) AmbientOcclusionVolumeBoundingPass::AmbientOcclusionVolumeBoundingInput::POSITION,
            gBufferPass->getOutputFrameTexture(
                    (size_t) AmbientOcclusionVolumeGBufferPass::AmbientOcclusionVolumeGBufferOutput::POSITION));
    boundingPass->specifyInput((size_t) AmbientOcclusionVolumeBoundingPass::AmbientOcclusionVolumeBoundingInput::NORMAL,
                               gBufferPass->getOutputFrameTexture(
                                       (size_t) AmbientOcclusionVolumeGBufferPass::AmbientOcclusionVolumeGBufferOutput::NORMAL));
    boundingPass->addRequirePass(gBufferPass);
    m_shadingPass = new AmbientOcclusionVolumeShadingPass(shadingSetting);
    m_shadingPass->specifyInput(
            (size_t) AmbientOcclusionVolumeShadingPass::AmbientOcclusionVolumeShadingInput::POSITION,
            gBufferPass->getOutputFrameTexture(
                    (size_t) AmbientOcclusionVolumeGBufferPass::AmbientOcclusionVolumeGBufferOutput::POSITION));
    m_shadingPass->specifyInput((size_t) AmbientOcclusionVolumeShadingPass::AmbientOcclusionVolumeShadingInput::NORMAL,
                                gBufferPass->getOutputFrameTexture(
                                        (size_t) AmbientOcclusionVolumeGBufferPass::AmbientOcclusionVolumeGBufferOutput::NORMAL));
    m_shadingPass->specifyInput((size_t) AmbientOcclusionVolumeShadingPass::AmbientOcclusionVolumeShadingInput::COLOR,
                                gBufferPass->getOutputFrameTexture(
                                        (size_t) AmbientOcclusionVolumeGBufferPass::AmbientOcclusionVolumeGBufferOutput::COLOR));
    m_shadingPass->specifyInput(
            (size_t) AmbientOcclusionVolumeShadingPass::AmbientOcclusionVolumeShadingInput::OCCLUSION,
            boundingPass->getOutputFrameTexture(
                    (size_t) AmbientOcclusionVolumeBoundingPass::AmbientOcclusionVolumeBoundingOutput::OCCLUSION));

    m_shadingPass->addRequirePass(boundingPass);

    auto hittableList = m_scene->m_hittableList->m_hittableList;
    for (auto hittable : hittableList) {
        std::vector<ShadeObject *> shadeObjectList = hittable->createVAO();
        m_objectList.insert(m_objectList.end(), shadeObjectList.begin(), shadeObjectList.end());
    }
}


void AmbientOcclusionVolumeShadingPass::renderPass(const std::vector<ShadeObject *> &shadingList) {
    Pass::renderPass(shadingList);

    AmbientOcclusionVolumeShadingSetting *setting = (AmbientOcclusionVolumeShadingSetting *) m_passSetting;

    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader binding
    m_shader->bind();

    if (setting) {
        // object binding
        glBindVertexArray(m_quadVao);

        // uniform
        m_shader->uniform1i("gPosition", 0);
        m_shader->uniform1i("gNormal", 1);
        m_shader->uniform1i("gColor", 2);
        m_shader->uniform1i("gOcclusion", 3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_inputFrameTextureId[0]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_inputFrameTextureId[1]);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, m_inputFrameTextureId[2]);
        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, m_inputFrameTextureId[3]);

        // FIXME bad value
        m_shader->uniformBlockBind("Lights", (int) AmbientOcclusionVolumeShadingUBOInput::LIGHT);
        glBindBufferBase(GL_UNIFORM_BUFFER, (int) AmbientOcclusionVolumeShadingUBOInput::LIGHT, setting->m_lightUBO);

        // TODO Area Light and Light uniform move to shader handle
        m_shader->uniform1i("lightAmount", setting->m_lightList.size());

        const Coord &eyeCoord = setting->m_camera->m_eyeCoord;
        m_shader->uniform3f("viewPos", eyeCoord.x, eyeCoord.y, eyeCoord.z);

        // draw call
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AmbientOcclusionVolumeShadingPipeline::renderAllPass() {
    m_shadingPass->renderPass(m_objectList);
    blitFrameBuffer();

}

void AmbientOcclusionVolumeShadingPipeline::blitFrameBuffer() {
    AmbientOcclusionVolumeShadingPass *pass = (AmbientOcclusionVolumeShadingPass *) m_shadingPass;
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      pass->getOutputFrameBuffer());
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_camera->m_width, m_camera->m_width, 0, 0, m_camera->m_width, m_camera->m_height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}