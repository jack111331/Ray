//
// Created by Edge on 2020/7/13.
//

#include "Material.h"
#include "Scene.h"

using namespace std;

istream &operator>>(istream &is, Material &rhs) {
    is >> rhs.m_color;
    is >> rhs.m_constantAmbient;
    is >> rhs.m_constantDiffuse;
    is >> rhs.m_constantSpecular;
    is >> rhs.m_constantSpecularExp;
    is >> rhs.m_constantReflectionRatio;

}

Color Material::calculatePhong(const Scene *scene, const Ray &ray, const HitRecord &record,
                               const LightRecord &shadeLightRecord) {
    // calculate ambient
    double ambientIntensity = record.material->m_constantAmbient;

    // calculate diffuse and specular
    // calculate specular
    Velocity normal = record.normal;
    if(ray.velocity.dot(normal) > 0.0) {
        normal = -normal;
    }
    double diffuseIntensity = 0.0;
    double specularIntensity = 0.0;
    for (int i = 0;i < scene->m_lightList.size();++i) {
        if(shadeLightRecord.isShadedLightList[i]) {
            Velocity lightDirection = (scene->m_lightList[i]->m_origin - record.point).normalize();
            diffuseIntensity += max(0.0, normal.dot(lightDirection));
            Velocity halfwayVector = (lightDirection - ray.velocity.normalize()).normalize();
            specularIntensity += pow(max(0.0, normal.dot(halfwayVector)), record.material->m_constantSpecularExp);
        }
    }
    diffuseIntensity *= record.material->m_constantDiffuse;
    specularIntensity *= record.material->m_constantSpecular;
    return min(1.0, (ambientIntensity + diffuseIntensity + specularIntensity)) * record.material->m_color;
}