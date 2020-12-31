#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource) {
    addShader(GL_VERTEX_SHADER, vertexShaderSource);
    addShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_isCompiled = false;
}

Shader::Shader(const char *computeShaderSource) {
    addShader(GL_COMPUTE_SHADER, computeShaderSource);
    m_isCompiled = false;
}

Shader::~Shader() {
    for (auto shader: m_shaderList) {
        glDeleteShader(shader);
    }
    if(m_isCompiled) {
        glDeleteProgram(m_program);
    }
}


Shader &Shader::addShader(uint32_t shaderType, const char *source) {
    int success;
    char infoLog[1024];
    uint32_t shader = glCreateShader(shaderType);
    if (glLoadShaderFile(source, shader) == false) {
        fprintf(stderr, "[%s] Error reading file %s\n", __FILE__, source);
        exit(1);
    }

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "[%s] Shader %s:\n%s\n", __FILE__, source, infoLog);
        exit(1);
    }
    m_shaderList.push_back(shader);
}

Shader &Shader::addAttachShader(const Shader *source) {
    m_attachShaderList.push_back(source);
    return *this;
}

void Shader::attachToProgram(uint32_t program) const {
    for (auto shader: m_shaderList) {
        glAttachShader(program, shader);
    }
}

Shader &Shader::buildShader() {
    int success;
    char infoLog[1024];

    if(m_isCompiled) {
        glDeleteProgram(m_program);
        m_isCompiled = false;
    }

    m_program = glCreateProgram();

    for (auto attachShader: m_attachShaderList) {
        attachShader->attachToProgram(m_program);
    }

    attachToProgram(m_program);

    glLinkProgram(m_program);

    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "[%s] Shader linking : %s\n", __FILE__, infoLog);
        glDeleteProgram(m_program);
        exit(1);
    }
    m_isCompiled = true;
    m_attachShaderList.clear();
    return *this;
}

bool Shader::glLoadShaderFile(const char *szFile, GLuint shader) const {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::badbit);
    std::string str;
    try {
        shaderFile.open(szFile);
        std::stringstream ss;
        ss << shaderFile.rdbuf();
        shaderFile.close();
        str = ss.str();
    } catch (std::ifstream::failure e) {
        fprintf(stderr, "[%s] Shader file not success loaded\n", __FILE__);
        return false;
    }
    const char *shaderSource = str.c_str();
    glShaderSource(shader, 1, &shaderSource, NULL);
    return true;
}

void Shader::bind() const {
    glUseProgram(m_program);
}

void Shader::bindBuffer(uint32_t bufferId, int location) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, bufferId);
}

void Shader::dispatch(int x, int y, int z) const {
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Shader::uniformMat4f(const char* uniformName, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_program, uniformName), 1, GL_FALSE, &mat[0][0]);
}

void Shader::uniform3f(const char* uniformName, float val[]) const {
    glUniform3f(glGetUniformLocation(m_program, uniformName), val[0], val[1], val[2]);
}

void Shader::uniform3f(const char* uniformName, float val0, float val1, float val2) const {
    glUniform3f(glGetUniformLocation(m_program, uniformName), val0, val1, val2);
}

void Shader::uniform1f(const char *uniformName, float val) const {
    glUniform1f(glGetUniformLocation(m_program, uniformName), val);
}

void Shader::uniform1i(const char* uniformName, int val) const {
    glUniform1i(glGetUniformLocation(m_program, uniformName), val);
}

void Shader::uniformBlockBind(const char* uniformName, int bindPoint) {
    glUniformBlockBinding(m_program, glGetUniformBlockIndex(m_program, uniformName), bindPoint);
}

GLuint Shader::getProgramId() {
    return m_program;
}
