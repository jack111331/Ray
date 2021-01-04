#ifndef OPENGL_CUSTOM_SHADER_H
#define OPENGL_CUSTOM_SHADER_H

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

class Shader {
public:
//    template <uint32_t shaderType, const char *source>
//    Shader();

    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

    Shader(const char *vertexShaderSource, const char *geometryShaderSource, const char *fragmentShaderSource);

    explicit Shader(const char *computeShaderSource);

    Shader() = default;

    virtual ~Shader();

    Shader &addShader(uint32_t shaderType, const char *source);

    Shader &addAttachShader(const Shader *source);

    void attachToProgram(uint32_t program) const;

    Shader &buildShader();

    void bind() const;

    void bindBuffer(uint32_t bufferId, int location) const;

    void dispatch(int x, int y, int z) const;

    void uniformMat4f(const char* uniformName, const glm::mat4 &mat) const;

    void uniform3f(const char* uniformName, float val[]) const;

    void uniform3f(const char* uniformName, float val0, float val1, float val2) const;

    void uniform1f(const char* uniformName, float val) const;

    void uniform1i(const char* uniformName, int val) const;

    void uniformBlockBind(const char* uniformName, int bindPoint);

    uint32_t getProgramId();

private:
    bool glLoadShaderFile(const char *szFile, uint32_t shader) const;

    std::vector<uint32_t> m_shaderList;

    std::vector<const Shader *> m_attachShaderList;

    uint32_t m_program;

    bool m_isCompiled;

};

#endif
