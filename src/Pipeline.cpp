//
// Created by Edge on 2020/12/23.
//

#include "Shader.h"
#include <GL/glew.h>
#include "Pipeline.h"
#include <Lambertian.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLUtil.h>
#include <Whitted.h>
#include "HittableList.h"

PhongShadingPass::PhongShadingPass(PassSetting *passSetting) : Pass(passSetting), m_outputFrameBufferId(0),
                                                               m_outputFrameTextureId(0),
                                                               m_outputFrameDepthStencilBufferId(0) {
    m_shader = new Shader("resource/shader/mesh.vs", "resource/shader/mesh.fs");
    m_shader->buildShader();

    PhongPassSetting *phongPassSetting = (PhongPassSetting *) passSetting;

    glGenFramebuffers(1, &m_outputFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);

    glGenTextures(1, &m_outputFrameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_outputFrameTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, phongPassSetting->m_camera->m_width, phongPassSetting->m_camera->m_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputFrameTextureId, 0);

    glGenRenderbuffers(1, &m_outputFrameDepthStencilBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_outputFrameDepthStencilBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, phongPassSetting->m_camera->m_width,
                          phongPassSetting->m_camera->m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              m_outputFrameDepthStencilBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Phong shading pass frame buffer object successful setup" << std::endl;
    } else {
        std::cout << "Phong shading pass frame buffer object failed setup" << std::endl;
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PhongShadingPass::renderPass(const std::vector<ShadeObject *> &shadingList) {
    Pass::renderPass(shadingList);

    PhongPassSetting *setting = (PhongPassSetting *) m_passSetting;

    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFrameBufferId);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader binding
    m_shader->bind();

    if (setting) {
        for (auto object: shadingList) {
            if (object->m_material->getType() == Material::MaterialType::LAMBERTIAN) {
                // object binding
                glBindVertexArray(object->m_objectInfo.m_vao);

                // uniform
                m_shader->uniformBlockBind("Lights", (int) PhongShadingUBOInput::LIGHT);
                glBindBufferBase(GL_UNIFORM_BUFFER, (int) PhongShadingUBOInput::LIGHT, setting->m_lightUBO);

                m_shader->uniformMat4f("projection", setting->m_projectionMatrix);
                m_shader->uniformMat4f("view", setting->m_viewMatrix);

                // world transformation
                // TODO Bundle model matrix
                glm::mat4 model = glm::mat4(1.0f);
                m_shader->uniformMat4f("model", model);

                // TODO Area Light and Light uniform move to shader handle
                m_shader->uniform1i("lightAmount", setting->m_lightList.size());

                const Coord &eyeCoord = setting->m_camera->m_eyeCoord;
                m_shader->uniform3f("viewPos", eyeCoord.x, eyeCoord.y, eyeCoord.z);

                const LambertianMaterial *material = (LambertianMaterial *) object->m_material;
                m_shader->uniform3f("objectColor", material->m_diffuseColor.r, material->m_diffuseColor.g,
                                    material->m_diffuseColor.b);
                // draw call
                glDrawElements(GL_TRIANGLES, object->m_objectInfo.m_indicesAmount, GL_UNSIGNED_INT, 0);
            }
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Pipeline::setupEnvironment() {
    if (!glfwInit()) {
        std::cerr << "[GLFW] init Failed" << std::endl;
        exit(1);
    }
}

// TODO pipeline object's initialization
void RayTracingPipeline::setupEnvironment() {
    Pipeline::setupEnvironment();

    m_window = glfwCreateWindow(m_camera->m_width, m_camera->m_height, "Ray", NULL,
                                NULL);

    if (m_window == nullptr) {
        std::cout << "[GLFW] failed to create window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(m_window);

    glewInit();

    printGraphicCardInfo();

    glGenTextures(1, &m_frameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void RayTracingPipeline::pipelineLoop() {
    while (!glfwWindowShouldClose(m_window)) {
        // Ray tracing
        generateImage();

        // Setup GL
        glViewport(0, 0, m_camera->m_width, m_camera->m_height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = m_camera->m_width / (float) m_camera->m_height;
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        m_camera->bufferToTexture(m_frameTextureId);
//        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(1.0, -1.0, 0.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFlush();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    glfwTerminate();

}

void RayTracingPipeline::generateImage() {
    if (!m_camera) {
        return;
    }
    // Upper left start
    // Lower right end
    Velocity left = m_camera->m_direction.cross(m_camera->m_up);
    const double PI = acos(-1);
    Coord center = m_camera->m_eyeCoord + m_camera->m_direction.normalize();
    double widthFactor = tan(m_camera->m_fov / 180.0 * PI);
    double heightFactor = widthFactor * m_camera->m_height / m_camera->m_width;
    Coord leftUpper = center + widthFactor * left.normalize() + heightFactor * m_camera->m_up.normalize();
    Velocity unitHorizontalScreen = 1 / (float) (m_camera->m_width - 1) * 2.0 * widthFactor * left.normalize();
    Velocity unitVerticalScreen =
            1 / (float) (m_camera->m_height - 1) * 2.0 * heightFactor * m_camera->m_up.normalize();
    const int sampleAmount = 2;//30
    for (int i = 0; i < m_camera->m_height; ++i) {
        for (int j = 0; j < m_camera->m_width; ++j) {
            m_camera->m_screen[i][j] = Color{0.0f, 0.0f, 0.0f};
            Coord currentRayOnScreen = leftUpper - (float) j * unitHorizontalScreen - (float) i * unitVerticalScreen;
            Ray ray = {
                    m_camera->m_eyeCoord,
                    currentRayOnScreen - m_camera->m_eyeCoord
            };
            m_camera->m_screen[i][j] = traceRay(ray);
        }
    }
}

void WhittedPipeline::setupPipeline() {
    WhittedModel *model = new WhittedModel();
    model->setupBackgroundColor(m_backgroundColor);
    setIlluminationModel(model);
}

void PhotonMappingPipeline::setupPipeline() {
    if (!m_photonAmount || m_photonPower < 1e-6) {
        std::cerr << "photon amount or power not specified" << std::endl;
        exit(1);
    }
    PhotonMappingModel *model = new PhotonMappingModel();
    model->setup(m_scene, m_photonAmount, m_photonPower, m_photonTraceDepth);
    model->setupBackgroundColor(m_backgroundColor);
    setIlluminationModel(model);
}

void LocalRenderingPipeline::setupEnvironment() {
    if (!m_scene) {
        exit(1);
    }
    Pipeline::setupEnvironment();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_camera->m_width, m_camera->m_height, "Ray", NULL,
                                NULL);

    if (m_window == nullptr) {
        std::cout << "[GLFW] failed to create window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(m_window);

    glewInit();

    printGraphicCardInfo();

}

void LocalRenderingPipeline::pipelineLoop() {
    while (!glfwWindowShouldClose(m_window)) {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderAllPass();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();

}

void PhongShadingPipeline::setupPipeline() {
    if (!m_scene || !m_camera) {
        std::cerr << "No camera or scene specified" << std::endl;
        exit(1);
    }
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

    m_shadingPass = new PhongShadingPass(shadingSetting);
    m_shadingPass->setupPassSetting(shadingSetting);

    auto hittableList = m_scene->m_hittableList->m_hittableList;
    for (auto hittable : hittableList) {
        std::vector<ShadeObject *> shadeObjectList = hittable->createVAO();
        m_objectList.insert(m_objectList.end(), shadeObjectList.begin(), shadeObjectList.end());
    }
}

void PhongShadingPipeline::renderAllPass() {
    m_shadingPass->renderPass(m_objectList);
    blitFrameBuffer();
}


void PhongShadingPipeline::blitFrameBuffer() {
    PhongShadingPass *pass = (PhongShadingPass *) m_shadingPass;
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      pass->getOutputFrameBuffer((size_t) PhongShadingPass::PhongShadingOutput::IMAGE));
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, m_camera->m_width, m_camera->m_width, 0, 0, m_camera->m_width, m_camera->m_height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}