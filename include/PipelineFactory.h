//
// Created by Edge on 2020/12/26.
//

#ifndef RAY_PIPELINEFACTORY_H
#define RAY_PIPELINEFACTORY_H


#include "Pipeline.h"
#include "InstantRadiosity.h"
#include "Whitted.h"
#include "Photon.h"
#include "DefferedShadingPipeline.h"
#include "SSAODefferedShadingPipeline.h"
#include "AmbientOcclusion.h"

class PipelineFactory {
public:
    static Pipeline *generatePipeline(const std::string &typeName, const std::string &pipelineName) {
        if (typeName == "ray-tracing") {
            if (pipelineName == "whitted") {
                return new WhittedPipeline();
            } else if (pipelineName == "photon-mapping") {
                return new PhotonMappingPipeline();
            } else if (pipelineName == "ambient-occlusion") {
                return new AmbientOcclusionPipeline();
            }
        } else if (typeName == "local-rendering") {
            if (pipelineName == "phong-shading") {
                return new PhongShadingPipeline();
            } else if (pipelineName == "instant-radiosity") {
                return new InstantRadiosityPipeline();
            } else if (pipelineName == "deffered-shading") {
                return new DefferedShadingPipeline();
            } else if (pipelineName == "ssao-deffered-shading") {
                return new SSAODefferedShadingPipeline();
            }
        }
        return nullptr;
    }
};


#endif //RAY_PIPELINEFACTORY_H
