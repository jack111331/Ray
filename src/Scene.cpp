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
        auto lightNode = node["light"];
        for (uint32_t i = 0;i < lightNode.size();++i) {
            if (lightNode[i]["type"].as<std::string>() == "point-light") {
                Light *light = new Light();
                light->readLightInfo(lightNode[i]);
                m_lightList.push_back(light);
            } else if (lightNode[i]["type"].as<std::string>() == "area-light") {
                AreaLight *light = new AreaLight();
                light->readLightInfo(lightNode[i]);
                m_lightList.push_back(light);
            }
        }
    }
    if (node["material"]) {
        auto materialNode = node["material"];
        for (uint32_t i = 0;i < materialNode.size();++i) {
            if (materialNode[i]["type"].as<std::string>() == "lambertian") {
                LambertianMaterial *material = new LambertianMaterial();
                material->readMaterialInfo(materialNode[i]);
                m_materialTable[materialNode[i]["name"].as<std::string>()] = material;
            } else if (materialNode[i]["type"].as<std::string>() == "dielectric") {
                DielectricMaterial *material = new DielectricMaterial();
                material->readMaterialInfo(materialNode[i]);
                m_materialTable[materialNode[i]["name"].as<std::string>()] = material;
            }
        }
    }
    if(node["object"]) {
        auto objectNode = node["object"];
        for (uint32_t i = 0; i < objectNode.size(); ++i) {
            if (objectNode[i]["type"].as<std::string>() == "sphere") {
                Sphere *object = new Sphere();
                object->readObjectInfo(objectNode[i], this);
                m_hittableList->addHittable(object);
            } else if (objectNode[i]["type"].as<std::string>() == "triangle-group") {
                TriangleGroup *object = new TriangleGroup();
                object->readObjectInfo(objectNode[i], this);
                m_hittableList->addHittable(object);
            } else if(objectNode[i]["type"].as<std::string>() == "obj") {
                m_hittableList->addHittable(TriangleGroup::fromObj(objectNode[i], this));
            }
        }
    }
    return true;
}