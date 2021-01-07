//
// Created by Edge on 2020/12/23.
//

#include "ShaderProgram.h"
#include <GL/glew.h>
#include "Pipeline.h"
#include <Lambertian.h>
#include "HittableList.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const char Pipeline::GLSL_VERSION[] = "#version 450";

void Pipeline::setupEnvironment() {
    if (!glfwInit()) {
        std::cerr << "[GLFW] init Failed" << std::endl;
        exit(1);
    }

    if (!m_scene) {
        exit(1);
    }

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

    setupGUIEnvironment();
}

void Pipeline::setupGUIEnvironment() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void CPURayTracingPipeline::setupEnvironment() {
    Pipeline::setupEnvironment();

    glGenTextures(1, &m_frameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_screenShader = new ShaderProgram(ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.vs"), ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.fs"));
}

void CPURayTracingPipeline::pipelineLoop() {
    // Ray tracing
    generateImage();
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera->bufferToTexture(m_frameTextureId);

        m_screenShader->bind();
        glBindVertexArray(m_quadVao);

        m_screenShader->uniform1i("gScreen", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifdef GUI_SUPPORT
        setupGUILayout();
#endif

        glfwSwapBuffers(m_window);
    }
    glfwTerminate();

}

void CPURayTracingPipeline::setupGUILayout() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Ray rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool CPURayTracingPipeline::readPipelineInfo(const YAML::Node &node) {
    if(node["jitter-sample-amount"]) {
        m_jitterSampleAmount = node["jitter-sample-amount"].as<int>();
    }
    return true;
}

void CPURayTracingPipeline::generateImage() {
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
    for (int i = 0; i < m_camera->m_height; ++i) {
        for (int j = 0; j < m_camera->m_width; ++j) {
            bool debugFlag = false;
            Coord currentRayOnScreen = leftUpper - (float) j * unitHorizontalScreen - (float) i * unitVerticalScreen;
            Ray ray = {
                    m_camera->m_eyeCoord,
                    currentRayOnScreen - m_camera->m_eyeCoord
            };
            m_camera->m_screen[i][j] = traceRay(ray, debugFlag);
            for(int k = 0;k < m_jitterSampleAmount;++k) {
                Ray perturbatedRay = {ray.origin,
                                      ray.velocity + Util::randomInUnit() * unitHorizontalScreen +
                                      Util::randomInUnit() * unitVerticalScreen};
                m_camera->m_screen[i][j] += traceRay(perturbatedRay, debugFlag);
            }
            m_camera->m_screen[i][j] /= (float)(1 + m_jitterSampleAmount);
        }
    }
}

void GPURayTracingPipeline::setupEnvironment() {
    Pipeline::setupEnvironment();

    glGenTextures(1, &m_frameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_screenShader = new ShaderProgram(ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.vs"), ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.fs"));
}

void GPURayTracingPipeline::pipelineLoop() {
    // Ray tracing
    generateImage();
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera->bufferToTexture(m_frameTextureId);

        m_screenShader->bind();
        glBindVertexArray(m_quadVao);

        m_screenShader->uniform1i("gScreen", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifdef GUI_SUPPORT
        setupGUILayout();
#endif

        glfwSwapBuffers(m_window);
    }
    glfwTerminate();

}

void GPURayTracingPipeline::setupGUILayout() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Ray rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GPURayTracingPipeline::readPipelineInfo(const YAML::Node &node) {
    if(node["jitter-sample-amount"]) {
        m_jitterSampleAmount = node["jitter-sample-amount"].as<int>();
    }
    return true;
}

void GPURayTracingPipeline::generateImage() {
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
    for (int i = 0; i < m_camera->m_height; ++i) {
        for (int j = 0; j < m_camera->m_width; ++j) {
            bool debugFlag = false;
            Coord currentRayOnScreen = leftUpper - (float) j * unitHorizontalScreen - (float) i * unitVerticalScreen;
            Ray ray = {
                    m_camera->m_eyeCoord,
                    currentRayOnScreen - m_camera->m_eyeCoord
            };
            m_camera->m_screen[i][j] = traceRay(ray, debugFlag);
            for(int k = 0;k < m_jitterSampleAmount;++k) {
                Ray perturbatedRay = {ray.origin,
                                      ray.velocity + Util::randomInUnit() * unitHorizontalScreen +
                                      Util::randomInUnit() * unitVerticalScreen};
                m_camera->m_screen[i][j] += traceRay(perturbatedRay, debugFlag);
            }
            m_camera->m_screen[i][j] /= (float)(1 + m_jitterSampleAmount);
        }
    }
}

void LocalRenderingPipeline::setupGUILayout() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Local rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void LocalRenderingPipeline::pipelineLoop() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderAllPass();

#ifdef GUI_SUPPORT
        setupGUILayout();
#endif

        glfwSwapBuffers(m_window);
    }

    glfwTerminate();

}

