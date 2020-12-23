//
// Created by Edge on 2020/12/23.
//

#ifndef RAY_PIPELINE_H
#define RAY_PIPELINE_H

#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>
#include "Hittable.h"

class Shader;

struct ShadeObject {
    ObjectInfo m_objectInfo;
    Material *m_material;
};

class PipelineSetting {
public:
    // TODO can switch to UBO to pass
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    Camera *m_camera;
};

class Pipeline {
public:
    Pipeline() : m_shader(nullptr), m_outputFrameBuffer(0), m_setting(nullptr) {

    }

    void setupPipelineSetting(PipelineSetting *setting) {
        m_setting = setting;
    }

    virtual void startPipeline(const std::vector<ShadeObject *> shadingList) = 0;

protected:
    Shader *m_shader;
    uint32_t m_outputFrameBuffer;
    PipelineSetting *m_setting;
};

class PhongPipelineSetting : public PipelineSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};


class PhongShadingPipeline : public Pipeline {
public:
    PhongShadingPipeline();

    virtual void startPipeline(const std::vector<ShadeObject *> shadingList);

private:

};


#endif //RAY_PIPELINE_H
