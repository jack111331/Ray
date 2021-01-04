//
// Created by Edge on 2020/12/23.
//

#include <GL/glew.h>
#include "GLUtil.h"
#include "Light.h"

namespace RayUtil {
    uint32_t generateLightUBO(const std::vector<Light *> &lightList) {
        std::vector<LightAttribute> lightAttributeList;
        for (auto light: lightList) {
            lightAttributeList.push_back({{light->m_origin.x,    light->m_origin.y,    light->m_origin.z,    1},
                                          {light->m_emitColor.r, light->m_emitColor.g, light->m_emitColor.b, 1}});
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

    float lerp(float a, float b, float portion) {
        return portion * a + (1.0 - portion) * b;
    }

}