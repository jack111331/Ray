#include "ShaderProgram.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Shader.h>

ShaderProgram::ShaderProgram(const ShaderInclude::ShaderSource &computeShaderSource) {
    m_programId = glCreateProgram();

    addShader(GL_COMPUTE_SHADER, computeShaderSource);

    glLinkProgram(m_programId);
    int success = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        std::string errorMsg("Error while linking program\n");
        int infoLogSize = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogSize);
        char *info = new char[infoLogSize + 1];
        glGetShaderInfoLog(m_programId, infoLogSize, nullptr, info);
        errorMsg += info;
        delete[] info;
        glDeleteProgram(m_programId);
        std::cerr << "Error " << errorMsg << std::endl;

        exit(1);
    }
}

ShaderProgram::ShaderProgram(const ShaderInclude::ShaderSource &vertexShaderSource, const ShaderInclude::ShaderSource &fragmentShaderSource) {
    m_programId = glCreateProgram();

    addShader(GL_VERTEX_SHADER, vertexShaderSource);
    addShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glLinkProgram(m_programId);
    int success = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        std::string errorMsg("Error while linking program\n");
        int infoLogSize = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogSize);
        char *info = new char[infoLogSize + 1];
        glGetShaderInfoLog(m_programId, infoLogSize, nullptr, info);
        errorMsg += info;
        delete[] info;
        glDeleteProgram(m_programId);
        std::cerr << "Error " << errorMsg << std::endl;

        exit(1);
    }

}

ShaderProgram::ShaderProgram(const ShaderInclude::ShaderSource &vertexShaderSource, const ShaderInclude::ShaderSource &geometryShaderSource, const ShaderInclude::ShaderSource &fragmentShaderSource) {
    m_programId = glCreateProgram();

    addShader(GL_VERTEX_SHADER, vertexShaderSource);
    addShader(GL_GEOMETRY_SHADER, geometryShaderSource);
    addShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glLinkProgram(m_programId);
    int success = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        std::string errorMsg("Error while linking program\n");
        int infoLogSize = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogSize);
        char *info = new char[infoLogSize + 1];
        glGetShaderInfoLog(m_programId, infoLogSize, nullptr, info);
        errorMsg += info;
        delete[] info;
        glDeleteProgram(m_programId);
        std::cerr << "Error " << errorMsg << std::endl;

        exit(1);
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_programId);
}


ShaderProgram &ShaderProgram::addShader(uint32_t shaderType, const ShaderInclude::ShaderSource &source) {
    Shader shader(source, shaderType);
    glAttachShader(m_programId, shader.m_shaderId);
    // TODO test delete after attach and before compile
}

void ShaderProgram::bind() const {
    glUseProgram(m_programId);
}

void ShaderProgram::bindSSBOBuffer(uint32_t bufferId, int location) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, bufferId);
}

void ShaderProgram::bindImageTextureRead(uint32_t bufferId, int location) const {
    glBindImageTexture(location, bufferId, 0, GL_FALSE, 0, GL_READ_ONLY,
                       GL_RGBA32F);
}

void ShaderProgram::bindImageTextureWrite(uint32_t bufferId, int location) const {
    glBindImageTexture(location, bufferId, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                       GL_RGBA32F);
}

void ShaderProgram::dispatch(int x, int y, int z) const {
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ShaderProgram::uniformMat4f(const char* uniformName, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, uniformName), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::uniform3f(const char* uniformName, float val[]) const {
    glUniform3f(glGetUniformLocation(m_programId, uniformName), val[0], val[1], val[2]);
}

void ShaderProgram::uniform3f(const char* uniformName, float val0, float val1, float val2) const {
    glUniform3f(glGetUniformLocation(m_programId, uniformName), val0, val1, val2);
}

void ShaderProgram::uniform1f(const char *uniformName, float val) const {
    glUniform1f(glGetUniformLocation(m_programId, uniformName), val);
}

void ShaderProgram::uniform1i(const char* uniformName, int val) const {
    glUniform1i(glGetUniformLocation(m_programId, uniformName), val);
}

void ShaderProgram::uniformBlockBind(const char* uniformName, int bindPoint) {
    glUniformBlockBinding(m_programId, glGetUniformBlockIndex(m_programId, uniformName), bindPoint);
}