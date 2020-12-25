//
// Created by Edge on 2020/7/8.
//

#include <string>
#include <fstream>
#include <sstream>
#include <Dielectric.h>
#include <BVH.h>
#include <Lambertian.h>
#include <IllumModelFactory.h>
#include "Config.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "HittableList.h"
#include "AreaLight.h"
#include <yaml-cpp/yaml.h>
#include <PipelineFactory.h>

using namespace std;

Pipeline *Config::loadConfig(const string &configFilename) {
    YAML::Node configNode = YAML::LoadFile(configFilename);
    if(configNode.IsNull()) {
        return nullptr;
    }

    Pipeline *pipeline = nullptr;
    if(configNode["model"]) {
        if(!configNode["type"] || !configNode["pipeline"]) {
            // TODO better handle error
            exit(1);
        }
        pipeline = PipelineFactory::generatePipeline(configNode["type"].as<std::string>(), configNode["pipeline"].as<std::string>());
    }

    if(configNode["scene"]) {
        Scene *scene = new Scene();
        scene->readSceneInfo(configNode["scene"]);
        pipeline->setupScene(scene);
    }

    if(configNode["viewer"]) {
        Camera *camera = new Camera();
        camera->readCameraInfo(configNode["viewer"]);
        pipeline->setupCamera(camera);
    }
    return pipeline;
}