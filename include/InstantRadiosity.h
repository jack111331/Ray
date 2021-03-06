//
// Created by Edge on 2020/11/19.
//

#ifndef RAY_INSTANTRADIOSITY_H
#define RAY_INSTANTRADIOSITY_H

#include "KDTree.h"
#include "IllumModel.h"
#include "Pipeline.h"
#include "PhongShadingPipeline.h"

class VPL {

};

class InstantRadiosityPipeline : public LocalRenderingPipeline {
public:

    virtual void setupPipeline();

protected:
    virtual void renderAllPass();
    PhongShadingSetting *shadingSetting;
};


#endif //RAY_INSTANTRADIOSITY_H
