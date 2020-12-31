//
// Created by Edge on 2020/12/31.
//

#ifndef RAY_DEFFEREDSHADINGPIPELINE_H
#define RAY_DEFFEREDSHADINGPIPELINE_H

#include "Pipeline.h"

class DefferedGBufferPass : public Pass {
public:
    explicit DefferedGBufferPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "DefferedGBufferPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)DefferedGBufferOutput::POSITION : return m_outputFrameTextureId[0];
            case (size_t)DefferedGBufferOutput::NORMAL: return m_outputFrameTextureId[1];
            case (size_t)DefferedGBufferOutput::COLOR : return m_outputFrameTextureId[2];
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    enum class DefferedGBufferOutput {
        POSITION=0,
        NORMAL=1,
        COLOR=2
    };

private:
    uint32_t m_outputFrameTextureId[3];
    uint32_t m_outputFrameDepthStencilBufferId;
};

class DefferedShadingSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};

class DefferedShadingPass : public Pass {
public:
    explicit DefferedShadingPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "DefferedShadingPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)DefferedShadingOutput::IMAGE : return m_outputFrameTextureId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    virtual void specifyInput(size_t inputId, uint32_t textureId) {
        switch(inputId) {
            case (size_t)DefferedShadingInput::POSITION : m_inputFrameTextureId[0] = textureId; break;
            case (size_t)DefferedShadingInput::NORMAL : m_inputFrameTextureId[1] = textureId; break;
            case (size_t)DefferedShadingInput::COLOR : m_inputFrameTextureId[2] = textureId; break;
            default: {
                std::cerr << "Wrong input frame Id" << std::endl;
                exit(1);
            }
        }
    }

    enum class DefferedShadingInput {
        POSITION=0,
        NORMAL=1,
        COLOR=2 // currently not support ..
    };

    enum class DefferedShadingUBOInput {
        LIGHT=1
    };

    enum class DefferedShadingOutput {
        IMAGE=0
    };


private:
    uint32_t m_inputFrameTextureId[3];
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;

    uint32_t m_quadVao;
};


class DefferedShadingPipeline : public LocalRenderingPipeline {
public:
    virtual void setupPipeline();

    virtual void blitFrameBuffer();

protected:
    virtual void renderAllPass();

private:
    DefferedShadingSetting *shadingSetting;
};


#endif //RAY_DEFFEREDSHADINGPIPELINE_H
