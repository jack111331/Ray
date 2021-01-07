//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_SHADER_H
#define RAY_SHADER_H


#include "ShaderInclude.h"

class Shader {
public:
    Shader(const ShaderInclude::ShaderSource &shaderInclude, uint32_t shaderType);

    ~Shader();

    uint32_t m_shaderId;

};


#endif //RAY_SHADER_H
