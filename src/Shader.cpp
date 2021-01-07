//
// Created by Edge on 2021/1/8.
//

#include <GL/glew.h>
#include "Shader.h"
#include "ShaderInclude.h"

Shader::Shader(const ShaderInclude::ShaderSource &shaderInclude, uint32_t shaderType) {
    m_shaderId = glCreateShader(shaderType);
    std::cout << "Compiling Shader " << shaderInclude.path << " -> " << static_cast<int>(m_shaderId) << std::endl;
    const char *source = static_cast<const char *>(shaderInclude.source.c_str());
    glShaderSource(m_shaderId, 1, &source, nullptr);
    glCompileShader(m_shaderId);

    int success;
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        int infoLogSize;
        glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &infoLogSize);
        char *info = new char[infoLogSize + 1];
        glGetShaderInfoLog(m_shaderId, infoLogSize, nullptr, info);

        std::string errorMsg;
        errorMsg += shaderInclude.path + "\n";
        errorMsg += info;
        delete[] info;
        glDeleteShader(m_shaderId);
        std::cerr << "Shader compilation error " << errorMsg << std::endl;
        exit(1);

    }
}

Shader::~Shader() {
    glDeleteShader(m_shaderId);
}