//
// Created by Edge on 2020/7/8.
//

#include <Photon.h>
#include <Dielectric.h>
#include "Scene.h"
#include "GroupObj.h"
#include "IllumModel.h"
#include <Lambertian.h>
#include <Sphere.h>
#include "AreaLight.h"
#include "ObjectNode.h"

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
    if(node["mesh"]) {
        auto meshNode = node["mesh"];
        for (uint32_t i = 0; i < meshNode.size(); ++i) {
            if (meshNode[i]["type"].as<std::string>() == "sphere") {
                Sphere *object = new Sphere();
                object->readObjectInfo(meshNode[i], this);
                addHittable(meshNode[i]["name"].as<std::string>(), object);
            } else if (meshNode[i]["type"].as<std::string>() == "triangle-group") {
                TriangleGroup *object = new TriangleGroup();
                object->readObjectInfo(meshNode[i], this);
                addHittable(meshNode[i]["name"].as<std::string>(), object);
            } else if (meshNode[i]["type"].as<std::string>() == "obj") {
                auto object = TriangleGroup::fromObj(meshNode[i], this);
                addHittable(object);
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

void Scene::addHittable(const std::string &meshName, ObjectNode *mesh) {
    m_meshTable[meshName] = mesh;
}

void Scene::addHittable(std::map<std::string, ObjectNode *> &meshTable) {
    for (auto mesh: meshTable) {
        addHittable(mesh.first, mesh.second);
    }
}