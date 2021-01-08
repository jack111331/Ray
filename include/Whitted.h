//
// Created by Edge on 2020/10/14.
//

#ifndef RAY_WHITTED_H
#define RAY_WHITTED_H

#include <vector>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "IllumModel.h"
#include "Pipeline.h"

class WhittedPipeline: public CPURayTracingPipeline {
public:
    virtual void setupPipeline();

    virtual bool readPipelineInfo(const YAML::Node &node);

private:
    int m_maxDepth;
};


class WhittedModel : public IlluminationModel {
public:
    virtual Vec3f castRay(const Scene *scene, Ray &ray, int depth, bool debugFlag);

    virtual std::string getModelName() const {
        return "Whitted";
    }

    void setupMaxDepth(int maxDepth) { m_maxDepth = maxDepth; }

private:
    int m_maxDepth;
};


#endif //RAY_WHITTED_H
