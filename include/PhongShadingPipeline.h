//
// Created by Edge on 2020/12/31.
//

#ifndef RAY_PHONGSHADINGPIPELINE_H
#define RAY_PHONGSHADINGPIPELINE_H

#include "Pipeline.h"

class PhongShadingSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};


class PhongShadingPass : public Pass {
public:
    explicit PhongShadingPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "PhongShadingPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch (frameId) {
            case (size_t) PhongShadingOutput::IMAGE :
                return m_outputFrameBufferId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    enum class PhongShadingUBOInput {
        LIGHT = 1
    };

    enum class PhongShadingOutput {
        IMAGE = 0
    };


private:
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;
};

class PhongShadingPipeline : public LocalRenderingPipeline {
public:
    virtual void setupPipeline();

    virtual void blitFrameBuffer();

protected:
    virtual void renderAllPass();

private:
    PhongShadingSetting *shadingSetting;
};


#endif //RAY_PHONGSHADINGPIPELINE_H
