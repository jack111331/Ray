//
// Created by Edge on 2020/10/14.
//

#ifndef RAY_ILLUMMODELFACTORY_H
#define RAY_ILLUMMODELFACTORY_H

#include "IllumModel.h"
#include <string>

class IlluminationModelFactory {
public:
    static IlluminationModel *createModel(const std::string &modelName);
};


#endif //RAY_ILLUMMODELFACTORY_H
