//
// Created by Edge on 2020/7/8.
//

#include <Photon.h>
#include <Dielectric.h>
#include "Scene.h"
#include "GroupObj.h"
#include "IllumModel.h"
#include <Lambertian.h>
#include "AreaLight.h"
#include <Sphere.h>
#include <TriangleGroup.h>

bool Scene::readSceneInfo(const YAML::Node &node) {
    m_group = new GroupObj();

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
        if(node["object"][0]) {
            m_group->readObjectInfo(node["object"][0], this);
        } else {
            std::cerr << "No main group" << std::endl;
            exit(1);
        }
    }
    return true;
}