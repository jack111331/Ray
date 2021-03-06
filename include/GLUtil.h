//
// Created by Edge on 2020/12/23.
//

#ifndef RAY_GLUTIL_H
#define RAY_GLUTIL_H

#include <vector>
#include <cstdint>
#include "Utility.h"

class Light;

namespace RayUtil {
    struct LightAttribute {
        float position[4];
        float color[4];
    };

    uint32_t generateLightUBO(const std::vector<Light *> &lightList);

    void generateStaticSSBO(uint32_t &ssboId, void *data, size_t size);

    void generateDynamicSSBO(uint32_t &ssboId, void *data, size_t size);

    void updateSSBO(uint32_t ssboId, void *data, size_t offset, size_t size);

    float lerp(float a, float b, float portion);
}

#endif //RAY_GLUTIL_H
