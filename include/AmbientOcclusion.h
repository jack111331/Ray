//
// Created by Edge on 2021/1/1.
//

#ifndef RAY_AMBIENT_OCCLUSION_H
#define RAY_AMBIENT_OCCLUSION_H

#include <vector>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "IllumModel.h"
#include "Pipeline.h"

class AmbientOcclusionPipeline : public CPURayTracingPipeline {
public:
    virtual void setupPipeline();

    virtual bool readPipelineInfo(const YAML::Node &node);

private:
    int m_maxDepth;
    float m_occlusionRadius;
    int m_occlusionSampleAmount;
};


class AmbientOcclusionModel : public IlluminationModel {
public:
    virtual Vec3f castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag);

    virtual std::string getModelName() const {
        return "AmbientOcclusion";
    }

    void setupMaxDepth(int maxDepth) { m_maxDepth = maxDepth; }

    void setupOcclusionRadius(float occlusionRadius) { m_occlusionRadius = occlusionRadius; }

    void setupOcclusionSampleAmount(float occlusionSampleAmount) { m_occlusionSampleAmount = occlusionSampleAmount; }

    std::vector<Vec3f> m_testDirectionList;

private:
    int m_maxDepth;
    float m_occlusionRadius;
    int m_occlusionSampleAmount;
};


#endif //RAY_AMBIENT_OCCLUSION_H
