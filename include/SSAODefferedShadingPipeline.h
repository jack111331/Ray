//
// Created by Edge on 2020/12/31.
//

#ifndef RAY_SSAODEFFEREDSHADINGPIPELINE_H
#define RAY_SSAODEFFEREDSHADINGPIPELINE_H

#include "Pipeline.h"

class SSAODefferedGBufferPass : public Pass {
public:
    explicit SSAODefferedGBufferPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "SSAODefferedGBufferPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)SSAODefferedGBufferOutput::POSITION : return m_outputFrameTextureId[0];
            case (size_t)SSAODefferedGBufferOutput::NORMAL: return m_outputFrameTextureId[1];
            case (size_t)SSAODefferedGBufferOutput::COLOR : return m_outputFrameTextureId[2];
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    enum class SSAODefferedGBufferOutput {
        POSITION=0,
        NORMAL=1,
        COLOR=2
    };

private:
    uint32_t m_outputFrameTextureId[3];
    uint32_t m_outputFrameDepthStencilBufferId;
};

class SSAODefferedShadingSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};

class SSAODefferedAmbientOcclusionPass : public Pass {
public:
    explicit SSAODefferedAmbientOcclusionPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "SSAODefferedAmbientOcclusionPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)SSAODefferedAmbientOcclusionOutput::OCCLUSION : return m_outputFrameTextureId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    virtual void specifyInput(size_t inputId, uint32_t textureId) {
        switch(inputId) {
            case (size_t)SSAODefferedAmbientOcclusionInput::POSITION : m_inputFrameTextureId[0] = textureId; break;
            case (size_t)SSAODefferedAmbientOcclusionInput::NORMAL : m_inputFrameTextureId[1] = textureId; break;
            default: {
                std::cerr << "Wrong input frame Id" << std::endl;
                exit(1);
            }
        }
    }

    enum class SSAODefferedAmbientOcclusionInput {
        POSITION=0,
        NORMAL=1
    };

    enum class SSAODefferedAmbientOcclusionOutput {
        OCCLUSION=0
    };


private:
    void generateSampleList();
    std::vector<Vec3f> generateNoiseList();

    uint32_t m_inputFrameTextureId[2];
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;

    uint32_t m_quadVao;
    std::vector<Vec3f> m_ssaoKernel;
    uint32_t m_noiseTextureId;
};

class SSAODefferedShadingPass : public Pass {
public:
    explicit SSAODefferedShadingPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "SSAODefferedShadingPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)SSAODefferedShadingOutput::IMAGE : return m_outputFrameTextureId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    virtual void specifyInput(size_t inputId, uint32_t textureId) {
        switch(inputId) {
            case (size_t)SSAODefferedShadingInput::POSITION : m_inputFrameTextureId[0] = textureId; break;
            case (size_t)SSAODefferedShadingInput::NORMAL : m_inputFrameTextureId[1] = textureId; break;
            case (size_t)SSAODefferedShadingInput::COLOR : m_inputFrameTextureId[2] = textureId; break;
            case (size_t)SSAODefferedShadingInput::OCCLUSION : m_inputFrameTextureId[3] = textureId; break;
            default: {
                std::cerr << "Wrong input frame Id" << std::endl;
                exit(1);
            }
        }
    }

    enum class SSAODefferedShadingInput {
        POSITION=0,
        NORMAL=1,
        COLOR=2,
        OCCLUSION=3

    };

    enum class SSAODefferedShadingUBOInput {
        LIGHT=1
    };

    enum class SSAODefferedShadingOutput {
        IMAGE=0
    };


private:
    uint32_t m_inputFrameTextureId[4];
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;

    uint32_t m_quadVao;
};


class SSAODefferedShadingPipeline : public LocalRenderingPipeline {
public:
    virtual void setupPipeline();

    virtual void blitFrameBuffer();

protected:
    virtual void renderAllPass();

private:
    SSAODefferedShadingSetting *shadingSetting;
};


#endif //RAY_SSAODEFFEREDSHADINGPIPELINE_H
