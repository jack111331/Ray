#ifndef OPENGL_CUSTOM_SHADER_H
#define OPENGL_CUSTOM_SHADER_H

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include "ShaderInclude.h"

class ShaderProgram {
public:

    explicit ShaderProgram(const ShaderInclude::ShaderSource &computeShaderSource);

    ShaderProgram(const ShaderInclude::ShaderSource &vertexShaderSource, const ShaderInclude::ShaderSource &fragmentShaderSource);

    ShaderProgram(const ShaderInclude::ShaderSource &vertexShaderSource, const ShaderInclude::ShaderSource &geometryShaderSource, const ShaderInclude::ShaderSource &fragmentShaderSource);

    virtual ~ShaderProgram();

    ShaderProgram &addShader(uint32_t shaderType, const ShaderInclude::ShaderSource &shaderSource);

    void bind() const;

    void bindSSBOBuffer(uint32_t bufferId, int location) const;

    void bindImageTextureWrite(uint32_t bufferId, int location) const;

    void dispatch(int x, int y, int z) const;

    void uniform1i(const char* uniformName, int val) const;

    void uniform1f(const char* uniformName, float val) const;

    void uniform3f(const char* uniformName, float val[]) const;

    void uniform3f(const char* uniformName, float val0, float val1, float val2) const;

    void uniformMat4f(const char* uniformName, const glm::mat4 &mat) const;

    void uniformBlockBind(const char* uniformName, int bindPoint);

private:
    uint32_t m_programId;

};

#endif
