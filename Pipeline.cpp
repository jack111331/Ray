//
// Created by Edge on 2020/12/23.
//

#include "Pipeline.h"
#include "Shader.h"
#include <GL/glew.h>
#include <Lambertian.h>

PhongShadingPipeline::PhongShadingPipeline() : Pipeline() {
    m_shader = new Shader("shader/mesh.vs", "shader/mesh.fs");
    m_shader->buildShader();
}

void PhongShadingPipeline::startPipeline(const std::vector<ShadeObject *> shadingList) {
    PhongPipelineSetting *setting = (PhongPipelineSetting *)m_setting;

    // FIXME Bind light buffer to location 1
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, setting->m_lightUBO);

    if(setting) {
        for(auto object: shadingList) {
            if (object->m_material->getType() == Material::MaterialType::LAMBERTIAN) {
                // shader binding
                m_shader->bind();
                // object binding
                glBindVertexArray(object->m_objectInfo.m_vao);
                // uniform
                m_shader->uniformMat4f("projection", setting->m_projectionMatrix);
                m_shader->uniformMat4f("view", setting->m_viewMatrix);

                // world transformation
                // TODO Bundle model matrix
                glm::mat4 model = glm::mat4(1.0f);
                m_shader->uniformMat4f("model", model);

                // TODO Area Light and Light uniform move to shader handle
                // TODO lightAmount
                m_shader->uniform1i("lightAmount", setting->m_lightList.size());

                const Coord &eyeCoord = setting->m_camera->m_eyeCoord;
                m_shader->uniform3f("viewPos", eyeCoord.x, eyeCoord.y, eyeCoord.z);

                const LambertianMaterial *material = (LambertianMaterial *) object->m_material;
                m_shader->uniform3f("objectColor", material->m_diffuseColor.r, material->m_diffuseColor.g,
                                    material->m_diffuseColor.b);
                // draw call
                glDrawElements(GL_TRIANGLES, object->m_objectInfo.m_indicesAmount, GL_UNSIGNED_INT, 0);
            }

        }
    }
}