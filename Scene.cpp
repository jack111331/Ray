//
// Created by Edge on 2020/7/8.
//

#include <Photon.h>
#include <Dielectric.h>
#include "Scene.h"
#include "HittableList.h"
#include "IllumModel.h"
#include <Lambertian.h>
#include "AreaLight.h"
#include <Sphere.h>
#include <TriangleGroup.h>

bool Scene::readSceneInfo(const YAML::Node &node) {
    m_hittableList = new HittableList();

    if (node["light"]) {
        for (YAML::const_iterator it = node["light"].begin(); it != node["light"].end(); ++it) {
            if (it->second["type"].as<std::string>() == "point-light") {
                Light *light = new Light();
                light->readLightInfo(it->second);
                m_lightList.push_back(light);
            } else if (it->second["type"].as<std::string>() == "area-light") {
                AreaLight *light = new AreaLight();
                light->readLightInfo(it->second);
                m_lightList.push_back(light);
            }
        }
    }
    if (node["material"]) {
        for (YAML::const_iterator it = node["material"].begin(); it != node["material"].end(); ++it) {
            if (it->second["type"].as<std::string>() == "lambertian") {
                LambertianMaterial *material = new LambertianMaterial();
                material->readMaterialInfo(it->second);
                m_materialTable[it->first.as<std::string>()] = material;
            } else if (it->second["type"].as<std::string>() == "dielectric") {
                DielectricMaterial *material = new DielectricMaterial();
                material->readMaterialInfo(it->second);
                m_materialTable[it->first.as<std::string>()] = material;
            }
        }
    }
    if(node["object"]) {
        for (YAML::const_iterator it = node["object"].begin(); it != node["object"].end(); ++it) {
            if (it->second["type"].as<std::string>() == "sphere") {
                Sphere *object = new Sphere();
                object->readObjectInfo(it->second, this);
                m_hittableList->addHittable(object);
            } else if (it->second["type"].as<std::string>() == "triangle-group") {
                TriangleGroup *object = new TriangleGroup();
                object->readObjectInfo(it->second, this);
                m_hittableList->addHittable(object);
            } else if(it->second["type"].as<std::string>() == "obj") {
                m_hittableList->addHittable(TriangleGroup::fromObj(it->second, this));
            }
        }
    }
}