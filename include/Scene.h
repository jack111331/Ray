//
// Created by Edge on 2020/7/8.
//

#ifndef RAY_SCENE_H
#define RAY_SCENE_H

#include <vector>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include <yaml-cpp/yaml.h>
//#include "AreaLight.h"
//
class GroupObj;

class IlluminationModel;

class AreaLight;
class ObjectNode;

class Scene {
public:
    Scene() {}

    // Need more abstraction, better use adapter to adapt different type of file
    bool readSceneInfo(const YAML::Node &node);

    void addHittable(const std::string &meshName, ObjectNode *mesh);
    void addHittable(std::map<std::string, ObjectNode *> &meshTable);

    GroupObj *m_group;
    std::vector<Light *> m_lightList;
    std::map<std::string, Material *> m_materialTable;
    std::map<std::string, ObjectNode *> m_meshTable;

};


#endif //RAY_SCENE_H
