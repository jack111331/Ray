//
// Created by Edge on 2020/10/14.
//

#include <Photon.h>
#include "IllumModelFactory.h"
#include "Whitted.h"

IlluminationModel *IlluminationModelFactory::createModel(const std::string &modelName) {
    if(modelName == "Whitted") {
        return new WhittedModel();
    } else if(modelName == "PhotonMapping") {
        return new PhotonMappingModel();
    } else {
        return nullptr;
    }
}