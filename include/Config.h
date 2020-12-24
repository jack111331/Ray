//
// Created by Edge on 2020/7/8.
//

#ifndef RAY_CONFIG_H
#define RAY_CONFIG_H


#include "Pipeline.h"

class Config {
public:
    Pipeline *loadConfig(const std::string &configFilename);
};


#endif //RAY_CONFIG_H
