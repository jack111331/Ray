//
// Created by Edge on 2020/12/23.
//

#include "Pipeline.h"
#include "Shader.h"
#include <GL/glew.h>
#include <Lambertian.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLUtil.h>
#include <GLFW/glfw3.h>
#include "HittableList.h"

PhongShadingPass::PhongShadingPass() : Pass() {
    m_shader = new Shader("shader/mesh.vs", "shader/mesh.fs");
    m_shader->buildShader();
}

uint32_t PhongShadingPass::renderPass(const std::vector<ShadeObject *> &shadingList) {
    Pass::renderPass(shadingList);

    PhongPassSetting *setting = (PhongPassSetting *) m_setting;

    // Switch to current frame buffer

    // FIXME Bind light buffer to location 1
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, setting->m_lightUBO);

    if (setting) {
        for (auto object: shadingList) {
            if (object->m_material->getType() == Material::MaterialType::LAMBERTIAN) {
                // shader binding
                m_shader->bind();
                // object binding
                glBindVertexArray(object->m_objectInfo.m_vao);
                // uniform
                m_shader->uniformMat4f("projection", setting->m_projectionMatrix);
                m_shader->uniformMat4f("view", setting->m_viewMatrix);

                // world transformation
                // TODO Bundle model matrix
                glm::mat4 model = glm::mat4(1.0f);
                m_shader->uniformMat4f("model", model);

                // TODO Area Light and Light uniform move to shader handle
                // TODO lightAmount
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
    return m_outputFrameBuffer;
}

void Pipeline::setupEnvironment() {
    if (!glfwInit()) {
//        cout << "[GLFW] init Failed" << endl;
        exit(1);
    }
}
// TODO pipeline object's initialization
void RayTracingPipeline::setupEnvironment() {
    if (!m_scene) {
        exit(1);
    }
    Pipeline::setupEnvironment();

    m_window = glfwCreateWindow(m_scene->m_camera->m_width, m_scene->m_camera->m_height, "Ray", NULL,
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
        // Setup GL
        glViewport(0, 0, m_scene->m_camera->m_width, m_scene->m_camera->m_height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = m_scene->m_camera->m_width / (float) m_scene->m_camera->m_height;
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        m_scene->m_camera->bufferToTexture(m_frameTextureId);
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

void LocalRenderingPipeline::setupEnvironment() {
    if (!m_scene) {
        exit(1);
    }
    Pipeline::setupEnvironment();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_scene->m_camera->m_width, m_scene->m_camera->m_height, "Ray", NULL,
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
        shadingPass->renderPass(m_objectList);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();

}

void PhongShadingPipeline::setupPipeline() {
    shadingPass = new PhongShadingPass();
    shadingSetting = new PhongPassSetting();
    Camera *camera = m_scene->m_camera;
    shadingSetting->m_projectionMatrix = glm::perspective(glm::radians(camera->m_fov * 2.0f),
                                                          (float) camera->m_width / (float) camera->m_height,
                                                          0.1f, 100.0f);
    shadingSetting->m_viewMatrix = glm::lookAt(
            glm::vec3(camera->m_eyeCoord.x, camera->m_eyeCoord.y, camera->m_eyeCoord.z),
            glm::vec3(camera->m_eyeCoord.x + camera->m_direction.x,
                      camera->m_eyeCoord.y + camera->m_direction.y,
                      camera->m_eyeCoord.z + camera->m_direction.z),
            glm::vec3(camera->m_up.x, camera->m_up.y, camera->m_up.z));
    shadingSetting->m_camera = m_scene->m_camera;
    shadingSetting->m_lightList = m_scene->m_lightList;
    shadingSetting->m_lightUBO = RayUtil::generateLightUBO(shadingSetting->m_lightList);

    shadingPass->setupPassSetting(shadingSetting);

    auto hittableList = m_scene->m_hittableList->m_hittableList;
    for (auto hittable : hittableList) {
        std::vector<ObjectInfo> objectInfoList = hittable->createVAO(hittable->m_material);
        for (auto objectInfo: objectInfoList) {
            m_objectList.push_back(new ShadeObject{objectInfo, hittable->m_material});
        }
    }
}

void PhongShadingPipeline::pipelineLoop() {

}