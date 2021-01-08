//
// Created by Edge on 2020/12/23.
//

#ifndef RAY_PIPELINE_H
#define RAY_PIPELINE_H

#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Hittable.h"
#include "Camera.h"
#include "IllumModel.h"

class ShaderProgram;

class PassSetting {
public:
    // TODO can switch to UBO to pass
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    Camera *m_camera;
};

// Pass Tree
class Pass {
public:
    Pass(PassSetting *passSetting) : m_shader(nullptr), m_passSetting(passSetting), m_outputFrameBufferId(0) {

    }

    void setupPassSetting(PassSetting *setting) {
        m_passSetting = setting;
    }

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList) {
        for (auto pass: m_requirePass) {
            // TODO filter flag to filter what shadingList should be seperate to shade
            // Assume each pass is configured beforehand
            pass->renderPass(shadingList);
        }
    };

    uint32_t getOutputFrameBuffer() {
        return m_outputFrameBufferId;
    }

    void addRequirePass(Pass *pass) {
        m_requirePass.push_back(pass);
    }

    virtual void specifyInput(size_t inputId, uint32_t textureId) {}

    virtual uint32_t getOutputFrameTexture(size_t frameId) = 0;

    virtual std::string getType() = 0;

protected:
    ShaderProgram *m_shader;
    uint32_t m_outputFrameBufferId;
    PassSetting *m_passSetting;
    std::vector<Pass *> m_requirePass;
};

class Pipeline {
public:
    Pipeline() : m_window(nullptr), m_scene(nullptr), m_camera(nullptr) {}

    void setupScene(Scene *scene) {
        m_scene = scene;
    }

    void setupCamera(Camera *camera) {
        m_camera = camera;
    }

    virtual void setupEnvironment();

    virtual void setupPipeline() = 0;

    virtual void pipelineLoop() = 0;

    virtual bool readPipelineInfo(const YAML::Node &node) { return true; };


protected:
    void printGraphicCardInfo() {
        const GLubyte *vendor = glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte *renderer = glGetString(GL_RENDERER);
        std::cout << "Current Vendor: " << (const char *) vendor << std::endl;
        std::cout << "Current Renderer: " << (const char *) renderer << std::endl;
    }

    void setupGUIEnvironment();

    virtual void setupGUILayout() = 0;

    static const char GLSL_VERSION[];


    GLFWwindow *m_window;
    Scene *m_scene;
    Camera *m_camera;

};

class CPURayTracingPipeline : public Pipeline {
public:
    CPURayTracingPipeline() : m_model(nullptr), m_jitterSampleAmount(0) {}

    virtual void setupEnvironment();

    virtual void pipelineLoop();

    virtual bool readPipelineInfo(const YAML::Node &node);

    void setIlluminationModel(IlluminationModel *model) {
        m_model = model;
    }

protected:
    void generateImage();

    Vec3f traceRay(Ray &ray, bool debugFlag = false) {
        if (m_model) {
            return m_model->castRay(m_scene, ray, 0, debugFlag);
        } else {
            return m_backgroundColor;
        }
    }

    virtual void setupGUILayout();

    IlluminationModel *m_model;

    Vec3f m_backgroundColor = {.0f, .0f, .0f};

private:
    uint32_t m_frameTextureId;

    uint32_t m_quadVao;

    ShaderProgram *m_screenShader;

    int m_jitterSampleAmount;
};

class GPURayTracingPipeline : public Pipeline {
public:
    GPURayTracingPipeline() : m_model(nullptr), m_jitterSampleAmount(0) {}

    virtual void setupEnvironment();

    virtual void pipelineLoop();

    virtual bool readPipelineInfo(const YAML::Node &node);

    void setIlluminationModel(IlluminationModel *model) {
        m_model = model;
    }

protected:
    void generateImage();

    Vec3f traceRay(Ray &ray, bool debugFlag = false) {
        if (m_model) {
            return m_model->castRay(m_scene, ray, 0, debugFlag);
        } else {
            return m_backgroundColor;
        }
    }

    virtual void setupGUILayout();

    IlluminationModel *m_model;

    Vec3f m_backgroundColor = {.0f, .0f, .0f};

private:
    uint32_t m_frameTextureId;

    uint32_t m_quadVao;

    ShaderProgram *m_screenShader;

    int m_jitterSampleAmount;
};

class LocalRenderingPipeline : public Pipeline {
public:
    virtual void pipelineLoop();

protected:
    virtual void renderAllPass() = 0;

    virtual void setupGUILayout();

    virtual void blitFrameBuffer() {};

    Pass *m_shadingPass;

    std::vector<ShadeObject *> m_objectList;

};

#endif //RAY_PIPELINE_H
