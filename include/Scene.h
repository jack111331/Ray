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
//#include "AreaLight.h"
//
class HittableList;

class IlluminationModel;

class AreaLight;

class Scene {
public:
    Scene() : m_model(nullptr) {};

    void setModel(IlluminationModel *model) {m_model = model;}

    Color castRay(Ray &ray);

    static void *castJitteredRay(void *castRayThreadArgs);

    void displayScene();

    void displayPhotonMappingScene();

    bool photonTracing(Ray &ray, float power, int depth);

    HittableList *m_hittableList;
    std::vector<Light *> m_lightList;
    std::vector<AreaLight *> m_areaLightList;
    Camera *m_camera;
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
