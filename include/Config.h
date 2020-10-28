//
// Created by Edge on 2020/7/8.
//

#ifndef ADVANCED_COMPUTER_GRAPH_CONFIG_H
#define ADVANCED_COMPUTER_GRAPH_CONFIG_H


#include "Scene.h"

class Config {
public:
    void loadConfig(const std::string &configFilename, Scene *scene);
};


#endif //ADVANCED_COMPUTER_GRAPH_CONFIG_H
