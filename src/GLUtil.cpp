//
// Created by Edge on 2020/12/23.
//

#include "glad/glad.h"
#include "GLUtil.h"
#include "Light.h"

namespace RayUtil {
    uint32_t generateLightUBO(const std::vector<Light *> &lightList) {
        std::vector<LightAttribute> lightAttributeList;
        for (auto light: lightList) {
            lightAttributeList.push_back({{light->m_origin.x,    light->m_origin.y,    light->m_origin.z,    1},
                                          {light->m_emitColor.x, light->m_emitColor.y, light->m_emitColor.z, 1}});
        }
        size_t MAX_LIGHT_AMOUNT = 100;

        uint32_t lightUbo;
        glGenBuffers(1, &lightUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, lightUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(LightAttribute) * MAX_LIGHT_AMOUNT, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightAttribute) * lightAttributeList.size(),
                        lightAttributeList.data());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return lightUbo;
    }

    void generateStaticSSBO(uint32_t &ssboId, void *data, size_t size) {
        glGenBuffers(1, &ssboId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void generateDynamicSSBO(uint32_t &ssboId, void *data, size_t size) {
        glGenBuffers(1, &ssboId);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboId);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    void updateSSBO(uint32_t ssboId, void *data, size_t offset, size_t size) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboId);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
    }

    float lerp(float a, float b, float portion) {
        return portion * a + (1.0 - portion) * b;
    }

}