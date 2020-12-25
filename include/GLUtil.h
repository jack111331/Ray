//
// Created by Edge on 2020/12/23.
//

#ifndef RAY_GLUTIL_H
#define RAY_GLUTIL_H

#include <vector>
#include <cstdint>

class Light;

namespace RayUtil {
    uint32_t generateLightUBO(const std::vector<Light *> &lightList);
}

#endif //RAY_GLUTIL_H
