//
// Created by Edge on 2020/12/31.
//

#ifndef RAY_SUGGESTEDCONTOURPIPELINE_H
#define RAY_SUGGESTEDCONTOURPIPELINE_H

#include "Pipeline.h"

class SuggestedContourShadingSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};

class SuggestedContourShadingPass : public Pass {
public:
    explicit SuggestedContourShadingPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "SuggestedContourShadingPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)SuggestedContourShadingOutput::IMAGE : return m_outputFrameTextureId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    enum class SuggestedContourShadingOutput {
        IMAGE=0
    };


private:
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;
};


class SuggestedContourShadingPipeline : public LocalRenderingPipeline {
public:
    virtual void setupPipeline();

    virtual void blitFrameBuffer();

protected:
    virtual void renderAllPass();

private:
    SuggestedContourShadingSetting *shadingSetting;
};


#endif //RAY_SUGGESTEDCONTOURPIPELINE_H
