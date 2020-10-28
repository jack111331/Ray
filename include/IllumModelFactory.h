//
// Created by Edge on 2020/10/14.
//

#ifndef ADVANCED_COMPUTER_GRAPH_ILLUMMODELFACTORY_H
#define ADVANCED_COMPUTER_GRAPH_ILLUMMODELFACTORY_H

#include "IllumModel.h"
#include <string>

class IlluminationModelFactory {
public:
    static IlluminationModel *createModel(const std::string &modelName);
};


#endif //ADVANCED_COMPUTER_GRAPH_ILLUMMODELFACTORY_H
