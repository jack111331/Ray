//
// Created by Edge on 2020/1/23.
//

#ifndef RAY_PATHTRACE_H
#define RAY_PATHTRACE_H

#include <vector>
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include "IllumModel.h"
#include "Pipeline.h"

class PathTraceCPUPipeline: public CPURayTracingPipeline {
public:
    virtual void setupPipeline();

    virtual bool readPipelineInfo(const YAML::Node &node);

private:
    int m_maxDepth;
};

class PathTraceGPUPipeline: public GPURayTracingPipeline {
public:
    virtual void setupPipeline();

    virtual bool readPipelineInfo(const YAML::Node &node);

protected:
    virtual void drawScreen();

private:
    int m_maxDepth;
    uint32_t m_lightSSBO;

};

class PathTraceModel : public IlluminationModel {
public:
    virtual Vec3f castRay(const Scene *scene, const Ray &ray, int depth, bool debugFlag);

    virtual std::string getModelName() const {
        return "PathTrace";
    }

    void setupMaxDepth(int maxDepth) { m_maxDepth = maxDepth; }

private:
    int m_maxDepth;
};


#endif //RAY_PATHTRACE_H
