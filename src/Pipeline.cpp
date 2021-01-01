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

void Pipeline::setupEnvironment() {
    if (!glfwInit()) {
        std::cerr << "[GLFW] init Failed" << std::endl;
        exit(1);
    }
}

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
    // Ray tracing
    generateImage();
    while (!glfwWindowShouldClose(m_window)) {

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
    // Upper left -> lower right
    const double PI = acos(-1);
    Velocity left = m_camera->m_direction.cross(m_camera->m_up);
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
    model->setupMaxDepth(m_maxDepth);
    setIlluminationModel(model);

    m_camera->initializeScreen();
}

bool WhittedPipeline::readPipelineInfo(const YAML::Node &node) {
    if(!node["max-depth"]) {
        std::cerr << "No require whitted pipeline node" << std::endl;
        return false;
    }
    m_maxDepth = node["max-depth"].as<int>();
    return true;
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

