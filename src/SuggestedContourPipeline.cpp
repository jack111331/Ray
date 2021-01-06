//
// Created by Edge on 2020/12/31.
//

#include <GL/glew.h>
#include <Lambertian.h>
#include <GLUtil.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include "Shader.h"
#include "HittableList.h"
#include "SuggestedContourPipeline.h"

SuggestedContourShadingPass::SuggestedContourShadingPass(PassSetting *passSetting) : Pass(passSetting), m_outputFrameTextureId{0},
                                                                     m_outputFrameDepthStencilBufferId(0) {
    m_shader = new Shader("resource/shader/suggested_contour.vs", "resource/shader/suggested_contour.fs");
    m_shader->buildShader();

    glGenFramebuffers(1, &m_outputFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glGenTextures(1, &m_outputFrameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_passSetting->m_camera->m_width, m_passSetting->m_camera->m_height,
                 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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
        std::cout << "Suggested contour pass frame buffer object successful setup" << std::endl;
    } else {
        std::cout << "Suggested contour pass frame buffer object failed setup" << std::endl;
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SuggestedContourShadingPass::renderPass(const std::vector<ShadeObject *> &shadingList) {
    Pass::renderPass(shadingList);

    SuggestedContourShadingSetting *setting = (SuggestedContourShadingSetting *) m_passSetting;

    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader binding
    m_shader->bind();

    if (setting) {
        int renderCount = 0;
        for (auto object: shadingList) {
            // object binding
            glBindVertexArray(object->m_objectInfo.m_vao);

            m_shader->uniformMat4f("projection", setting->m_projectionMatrix);
            m_shader->uniformMat4f("view", setting->m_viewMatrix);

            // world transformation
            // TODO Bundle model matrix
            glm::mat4 model = glm::mat4(1.0f);
            m_shader->uniformMat4f("model", model);

            const Coord &eyeCoord = setting->m_camera->m_eyeCoord;
            m_shader->uniform3f("viewPos", eyeCoord.x, eyeCoord.y, eyeCoord.z);

            m_shader->uniform1f("fz", setting->m_fz);
            m_shader->uniform1f("c_limit", setting->m_c_limit);
            m_shader->uniform1f("sc_limit", setting->m_sc_limit);
            m_shader->uniform1f("dwkr_limit", setting->m_dwkr_limit);

            // draw call
            glDrawElements(GL_TRIANGLES, object->m_objectInfo.m_indicesAmount, GL_UNSIGNED_INT, 0);
            renderCount++;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SuggestedContourShadingPipeline::setupPipeline() {
    if (!m_scene || !m_camera) {
        std::cerr << "No camera or scene specified" << std::endl;
        exit(1);
    }
    m_shadingSetting = new SuggestedContourShadingSetting();
    m_shadingSetting->m_projectionMatrix = glm::perspective(glm::radians(m_camera->m_fov * 2.0f),
                                                          (float) m_camera->m_width / (float) m_camera->m_height,
                                                            0.1f, 100.0f);
    m_shadingSetting->m_viewMatrix = glm::lookAt(
            glm::vec3(m_camera->m_eyeCoord.x, m_camera->m_eyeCoord.y, m_camera->m_eyeCoord.z),
            glm::vec3(m_camera->m_eyeCoord.x + m_camera->m_direction.x,
                      m_camera->m_eyeCoord.y + m_camera->m_direction.y,
                      m_camera->m_eyeCoord.z + m_camera->m_direction.z),
            glm::vec3(m_camera->m_up.x, m_camera->m_up.y, m_camera->m_up.z));
    m_shadingSetting->m_camera = m_camera;


    m_shadingPass = new SuggestedContourShadingPass(m_shadingSetting);

    auto hittableList = m_scene->m_hittableList->m_hittableList;
    for (auto hittable : hittableList) {
        std::vector<ShadeObject *> shadeObjectList = hittable->createVAO();
        m_objectList.insert(m_objectList.end(), shadeObjectList.begin(), shadeObjectList.end());
    }
}

void SuggestedContourShadingPipeline::renderAllPass() {
    m_shadingPass->renderPass(m_objectList);
    blitFrameBuffer();
}

void SuggestedContourShadingPipeline::blitFrameBuffer() {
    SuggestedContourShadingPass *pass = (SuggestedContourShadingPass *) m_shadingPass;
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      pass->getOutputFrameBuffer());
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_camera->m_width, m_camera->m_width, 0, 0, m_camera->m_width, m_camera->m_height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void SuggestedContourShadingPipeline::setupGUILayout() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Local rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::SliderFloat("feature size", &m_shadingSetting->m_fz, 0.0f, 1.0f);
    ImGui::SliderFloat("contour limit", &m_shadingSetting->m_c_limit, 0.0f, 1.0f);
    ImGui::SliderFloat("suggested contour limit", &m_shadingSetting->m_sc_limit, 0.0f, 1.0f);
    ImGui::SliderFloat("derivative radiant curvater", &m_shadingSetting->m_dwkr_limit, 0.0f, 1.0f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
