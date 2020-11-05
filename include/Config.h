//
// Created by Edge on 2020/7/8.
//

#ifndef RAY_CONFIG_H
#define RAY_CONFIG_H


#include "Scene.h"

class Config {
public:
    Scene *loadConfig(const std::string &configFilename);
};


#endif //RAY_CONFIG_H
