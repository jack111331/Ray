//
// Created by Edge on 2020/7/8.
//

#ifndef ADVANCED_COMPUTER_GRAPH_SCENE_H
#define ADVANCED_COMPUTER_GRAPH_SCENE_H

#include <vector>
#include <pthread.h>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "AreaLight.h"

class HittableList;

class IlluminationModel;

class Scene {
public:
    Scene(IlluminationModel *model) : m_model(model) {};

    Color castRay(Ray &ray, double intensity, Color &color, int depth);

    static void *castJitteredRay(void *castRayThreadArgs);

    void displayScene();

    void displayPhotonMappingScene();

    bool photonTracing(Ray &ray, float power, int depth);

    enum EmitType {
        ABSORBED, REFLECTED, TRANSMITTED
    };

    EmitType russianRoulette(float reflectivity, float refractivity) const;

    HittableList *m_hittableList;
    std::vector<Light *> m_lightList;
    std::vector<AreaLight *> m_areaLightList;
    Camera *m_camera;
    IlluminationModel *m_model;

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
};


#endif //ADVANCED_COMPUTER_GRAPH_SCENE_H
