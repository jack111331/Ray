//
// Created by Edge on 2020/12/23.
//

#include "ShaderProgram.h"
#include <GL/glew.h>
#include "Pipeline.h"
#include <Lambertian.h>
#include <Timer.h>
#include <unistd.h>
#include <random>
#include "GeometryGroupObj.h"
#include "GroupBVHTranslator.h"
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
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_screenShader = new ShaderProgram(ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.vs"),
                                       ShaderInclude::load("resource/shader/ray_tracing_shading/screen_shading.fs"));
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

    ImGui::Begin(
            "Ray rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool CPURayTracingPipeline::readPipelineInfo(const YAML::Node &node) {
    if (node["jitter-sample-amount"]) {
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
    Vec3f left = m_camera->m_direction.cross(m_camera->m_up);
    Vec3f center = m_camera->m_eyeCoord + m_camera->m_direction.normalize();
    double widthFactor = tan(m_camera->m_fov / 180.0 * PI);
    double heightFactor = widthFactor * m_camera->m_height / m_camera->m_width;
    Vec3f leftUpper = center + widthFactor * left.normalize() + heightFactor * m_camera->m_up.normalize();
    Vec3f unitHorizontalScreen = 1 / (float) (m_camera->m_width - 1) * 2.0 * widthFactor * left.normalize();
    Vec3f unitVerticalScreen =
            1 / (float) (m_camera->m_height - 1) * 2.0 * heightFactor * m_camera->m_up.normalize();
    for (int i = 0; i < m_camera->m_height; ++i) {
        for (int j = 0; j < m_camera->m_width; ++j) {
            bool debugFlag = false;
            Vec3f currentRayOnScreen = leftUpper - (float) j * unitHorizontalScreen - (float) i * unitVerticalScreen;
            Ray ray = {
                    m_camera->m_eyeCoord,
                    currentRayOnScreen - m_camera->m_eyeCoord
            };
            m_camera->m_screen[i][j] = traceRay(ray, debugFlag);
            for (int k = 0; k < m_jitterSampleAmount; ++k) {
                Ray perturbatedRay = {m_camera->m_eyeCoord,
                                      currentRayOnScreen - (Util::randomInUnit() * unitHorizontalScreen) -
                                      (Util::randomInUnit() * unitVerticalScreen) - m_camera->m_eyeCoord};
                m_camera->m_screen[i][j] += traceRay(perturbatedRay, debugFlag);
            }
            m_camera->m_screen[i][j] /= (float) (1 + m_jitterSampleAmount);
        }
    }
}

void GPURayTracingPipeline::setupEnvironment() {
    Pipeline::setupEnvironment();

    glGenTextures(1, &m_frameTextureId);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_camera->m_width, m_camera->m_height, 0, GL_RGBA,
                 GL_FLOAT, nullptr);
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_translator = new GroupBVHTranslator(m_scene);
}

void GPURayTracingPipeline::drawScreen() {
    m_screenShader->bind();
    glBindVertexArray(m_quadVao);

    m_screenShader->uniform1i("gScreen", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GPURayTracingPipeline::pipelineLoop() {
    // Ray tracing
    m_cumulatedRay = 0;

    Timer timer;
    const float RESTRICT_FRAMERATE = 1.f;
    const float framePerMs = 1000 / RESTRICT_FRAMERATE;
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        timer.updateCurrentTime();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        generateImage();

        float diffMs = timer.calculateDiffMilliSecondTime();
        if (diffMs < framePerMs) {
//            std::cout << diffMs << " " << framePerMs << std::endl;
            usleep(framePerMs - diffMs);
        }
        m_cumulatedRay += m_jitterSampleAmount;

        drawScreen();

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

    ImGui::Begin(
            "Ray rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GPURayTracingPipeline::readPipelineInfo(const YAML::Node &node) {
    if (node["jitter-sample-amount"]) {
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
    Vec3f left = m_camera->m_direction.cross(m_camera->m_up);
    Vec3f center = m_camera->m_eyeCoord + m_camera->m_direction.normalize();
    double widthFactor = tan(m_camera->m_fov / 180.0 * PI);
    double heightFactor = widthFactor * m_camera->m_height / m_camera->m_width;
    Vec3f leftUpper = center + widthFactor * left.normalize() + heightFactor * m_camera->m_up.normalize();
    Vec3f unitHorizontalScreen = 1 / (float) (m_camera->m_width - 1) * 2.0 * widthFactor * left.normalize();
    Vec3f unitVerticalScreen =
            1 / (float) (m_camera->m_height - 1) * 2.0 * heightFactor * m_camera->m_up.normalize();

    m_translator->updateTranslator();

    std::vector<uint32_t> randomNumberList(m_camera->m_width * m_camera->m_height, 0);
    static std::random_device randomDevice;   // non-deterministic generator
    static std::mt19937 rNGGenerator(randomDevice());

    for(int i = 0;i < m_jitterSampleAmount; ++i) {
        for(int j = 0;j < randomNumberList.size(); ++j) {
            randomNumberList[j] = rNGGenerator();
        }
        glBindTexture(GL_TEXTURE_2D, m_randomTextureId[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, m_camera->m_width, m_camera->m_height, 0, GL_RED_INTEGER,
                     GL_UNSIGNED_INT, randomNumberList.data());
        glGenerateMipmap(GL_TEXTURE_2D);
//        glTexBuffer(GL_TEXTURE_2D, GL_R32UI, m_randomTextureId[i]);
//        m_rayTracingShader->bindImageTextureWriteR32Ui(m_randomTextureId[i], i+1);
    }

    m_rayTracingShader->bind();
    // random vector
    m_rayTracingShader->uniform3f("random_vector", Util::randomInUnit(), Util::randomInUnit(), Util::randomInUnit());
    m_rayTracingShader->uniform3f("initial_pos", m_camera->m_eyeCoord);
    m_rayTracingShader->uniform3f("initial_vel", leftUpper);
    m_rayTracingShader->uniform3f("horizon_unit", unitHorizontalScreen);
    m_rayTracingShader->uniform3f("vertical_unit", unitVerticalScreen);
    m_rayTracingShader->uniform1i("top_level_bvh_index", m_translator->m_tlasStartNodeIndex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_frameTextureId);
    m_rayTracingShader->bindImageTextureWrite(m_frameTextureId, 0);

    for(int i = 0;i < m_jitterSampleAmount; ++i) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        glBindTexture(GL_TEXTURE_2D, m_randomTextureId[i]);
        // Notice: sampler texture id can't use uniform1ui..
        m_rayTracingShader->uniform1i(("init_random_seed["+std::to_string(i)+"]").c_str(), i + 1);
    }
    m_rayTracingShader->uniform1i("input_color", 0);
    m_rayTracingShader->uniform1i("sample_per_iteration", m_jitterSampleAmount); // TODO need to refactor name to sample_per_iteration

    m_rayTracingShader->bindSSBOBuffer(m_translator->m_bvhSSBO, 0);
    m_rayTracingShader->bindSSBOBuffer(m_translator->m_transformSSBO, 4);

    m_rayTracingShader->dispatch(m_camera->m_width, m_camera->m_height);
}

void LocalRenderingPipeline::setupGUILayout() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin(
            "Local rendering Pipeline Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
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

