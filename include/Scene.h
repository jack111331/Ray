//
// Created by Edge on 2020/7/8.
//

#ifndef RAY_SCENE_H
#define RAY_SCENE_H

#include <vector>
#include <pthread.h>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include <yaml-cpp/yaml.h>
//#include "AreaLight.h"
//
class HittableList;

class IlluminationModel;

class AreaLight;

class Scene {
public:
    Scene() : m_model(nullptr) {};

    // Need more abstraction, better use adapter to adapt different type of file
    bool readSceneInfo(const YAML::Node &node);

    void setModel(IlluminationModel *model) {m_model = model;}

    Color castRay(Ray &ray);

    static void *castJitteredRay(void *castRayThreadArgs);

    void displayScene();

    void displayPhotonMappingScene();

    bool photonTracing(Ray &ray, float power, int depth);

    HittableList *m_hittableList;
    std::vector<Light *> m_lightList;
    std::map<std::string, Material *> m_materialTable;
    IlluminationModel *m_model;
    Color m_backgroundColor = {.0f, .0f, .0f};

    // damn conversion
    struct CastRayThreadArgs {
        Scene *scene;
        int i;
        int j;
        Ray ray;
        int sampleAmount;
        Velocity unitHorizontalScreen;
        Velocity unitVerticalScreen;
    };

    struct ThreadInfo {
        pthread_t id;
        CastRayThreadArgs *castRayThreadArgs;
    };

    void photonGenerating(float photonPower, float photonAmount);
};


#endif //RAY_SCENE_H
