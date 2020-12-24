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

class Shader;

struct ShadeObject {
    ObjectInfo m_objectInfo;
    Material *m_material;
};

class PassSetting {
public:
    // TODO can switch to UBO to pass
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    Camera *m_camera;
};

class Pass {
public:
    Pass() : m_shader(nullptr), m_outputFrameBuffer(0), m_setting(nullptr), m_pass(nullptr) {

    }

    void setupPassSetting(PassSetting *setting) {
        m_setting = setting;
    }

    virtual uint32_t renderPass(const std::vector<ShadeObject *> &shadingList) {
        if(m_pass) {
            // TODO filter
            //m_pass->renderPass();
        }
    };

protected:
    Shader *m_shader;
    uint32_t m_outputFrameBuffer;
    PassSetting *m_setting;
    Pass *m_pass;
};

class PhongPassSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};


class PhongShadingPass : public Pass {
public:
    PhongShadingPass();

    virtual uint32_t renderPass(const std::vector<ShadeObject *> &shadingList);

private:

};

class Pipeline {
public:
    Pipeline() : m_scene(nullptr), m_window(nullptr) {}

    void setupScene(Scene *scene) {
        m_scene = scene;
    }

    virtual void setupEnvironment();

    virtual void setupPipeline() = 0;

    virtual void pipelineLoop() = 0;

protected:
    void printGraphicCardInfo() {
        const GLubyte *vendor = glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte *renderer = glGetString(GL_RENDERER);
        std::cout << "Current Vendor: " << (const char *) vendor << std::endl;
        std::cout << "Current Renderer: " << (const char *) renderer << std::endl;
    }


    GLFWwindow *m_window;
    Scene *m_scene;
};

class RayTracingPipeline : public Pipeline {
public:
    virtual void setupEnvironment();

    virtual void pipelineLoop();

private:
    uint32_t m_frameTextureId;
};

class LocalRenderingPipeline : public Pipeline {
public:
    virtual void setupEnvironment();

    virtual void pipelineLoop();

protected:
    PhongShadingPass *shadingPass;

    std::vector<ShadeObject *> m_objectList;

};
// TODO possible refactor: Frame on Pipeline?

class PhongShadingPipeline : public LocalRenderingPipeline {
public:
    void setupPipeline();

    virtual void pipelineLoop();

private:
    PhongPassSetting *shadingSetting;
};


#endif //RAY_PIPELINE_H
