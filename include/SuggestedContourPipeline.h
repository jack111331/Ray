//
// Created by Edge on 2020/12/31.
//

#ifndef RAY_SUGGESTEDCONTOURPIPELINE_H
#define RAY_SUGGESTEDCONTOURPIPELINE_H

#include "Pipeline.h"

class SuggestedContourGBufferPass : public Pass {
public:
    explicit SuggestedContourGBufferPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "SuggestedContourGBufferPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)SuggestedContourGBufferOutput::POSITION : return m_outputFrameTextureId[0];
            case (size_t)SuggestedContourGBufferOutput::NORMAL: return m_outputFrameTextureId[1];
            case (size_t)SuggestedContourGBufferOutput::PDIR1 : return m_outputFrameTextureId[2];
            case (size_t)SuggestedContourGBufferOutput::PDIR2 : return m_outputFrameTextureId[3];
            case (size_t)SuggestedContourGBufferOutput::CURV1 : return m_outputFrameTextureId[4];
            case (size_t)SuggestedContourGBufferOutput::CURV2 : return m_outputFrameTextureId[5];
            case (size_t)SuggestedContourGBufferOutput::DCURV : return m_outputFrameTextureId[6];
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    enum class SuggestedContourGBufferOutput {
        POSITION=0,
        NORMAL=1,
        PDIR1=2,
        PDIR2=3,
        CURV1=4,
        CURV2=5,
        DCURV=6,
    };

private:
    uint32_t m_outputFrameTextureId[7];
    uint32_t m_outputFrameDepthStencilBufferId;
};

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

    virtual void specifyInput(size_t inputId, uint32_t textureId) {
        switch(inputId) {
            case (size_t)SuggestedContourShadingInput::POSITION : m_inputFrameTextureId[0] = textureId; break;
            case (size_t)SuggestedContourShadingInput::NORMAL : m_inputFrameTextureId[1] = textureId; break;
            case (size_t)SuggestedContourShadingInput::PDIR1 : m_inputFrameTextureId[2] = textureId; break;
            case (size_t)SuggestedContourShadingInput::PDIR2 : m_inputFrameTextureId[3] = textureId; break;
            case (size_t)SuggestedContourShadingInput::CURV1 : m_inputFrameTextureId[4] = textureId; break;
            case (size_t)SuggestedContourShadingInput::CURV2 : m_inputFrameTextureId[5] = textureId; break;
            case (size_t)SuggestedContourShadingInput::DCURV : m_inputFrameTextureId[6] = textureId; break;
            default: {
                std::cerr << "Wrong input frame Id" << std::endl;
                exit(1);
            }
        }
    }

    enum class SuggestedContourShadingInput {
        POSITION=0,
        NORMAL=1,
        PDIR1=2,
        PDIR2=3,
        CURV1=4,
        CURV2=5,
        DCURV=6
    };

    enum class SuggestedContourShadingOutput {
        IMAGE=0
    };


private:
    uint32_t m_inputFrameTextureId[7];
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;

    uint32_t m_quadVao;
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
