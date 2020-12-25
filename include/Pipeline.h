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

// Pass Tree
class Pass {
public:
    Pass() : m_shader(nullptr), m_setting(nullptr) {

    }

    void setupPassSetting(PassSetting *setting) {
        m_setting = setting;
    }

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList) {
        for (auto pass: m_requirePass) {
            // TODO filter
            // Assume each pass is configured beforehand
            pass->renderPass(shadingList);
        }
    };

    std::vector<uint32_t> getOutputFrameBuffer() {
        return m_outputFrameBuffer;
    }

    virtual std::string getType() = 0;

protected:
    Shader *m_shader;
    std::vector<uint32_t> m_outputFrameBuffer;
    PassSetting *m_setting;
    std::vector<Pass *> m_requirePass;
};

class PhongPassSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};


class PhongShadingPass : public Pass {
public:
    PhongShadingPass();

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "PhongShadingPass";
    }


private:

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

protected:
    void printGraphicCardInfo() {
        const GLubyte *vendor = glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte *renderer = glGetString(GL_RENDERER);
        std::cout << "Current Vendor: " << (const char *) vendor << std::endl;
        std::cout << "Current Renderer: " << (const char *) renderer << std::endl;
    }


    GLFWwindow *m_window;
    Scene *m_scene;
    Camera *m_camera;

};

class RayTracingPipeline : public Pipeline {
public:
    RayTracingPipeline(): m_model(nullptr) {}

    virtual void setupEnvironment();

    virtual void pipelineLoop();

    void generateImage();

    Color traceRay(Ray &ray) {
        if (m_model) {
            return m_model->castRay(m_scene, ray);
        } else {
            return m_backgroundColor;
        }
    }

    void setIlluminationModel(IlluminationModel *model) {
        m_model = model;
    }

protected:
    IlluminationModel *m_model;

    Color m_backgroundColor = {.0f, .0f, .0f};

private:
    uint32_t m_frameTextureId;


};

class WhittedPipeline: public RayTracingPipeline {
public:
    virtual void setupPipeline();
};

class PhotonMappingPipeline: public RayTracingPipeline {
public:
    virtual void setupPipeline();

    int m_photonAmount;
    float m_photonPower;
    int m_photonTraceDepth;
};

class LocalRenderingPipeline : public Pipeline {
public:
    virtual void setupEnvironment();

    virtual void pipelineLoop();

protected:
    Pass *m_shadingPass;

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
