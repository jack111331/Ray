//
// Created by Edge on 2020/12/31.
//

#ifndef RAY_AMBIENTOCCLUSIONVOLUMESHADINGPIPELINE_H
#define RAY_AMBIENTOCCLUSIONVOLUMESHADINGPIPELINE_H

#include "Pipeline.h"

class AmbientOcclusionVolumeGBufferPass : public Pass {
public:
    explicit AmbientOcclusionVolumeGBufferPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "AmbientOcclusionVolumeGBufferPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)AmbientOcclusionVolumeGBufferOutput::POSITION : return m_outputFrameTextureId[0];
            case (size_t)AmbientOcclusionVolumeGBufferOutput::NORMAL: return m_outputFrameTextureId[1];
            case (size_t)AmbientOcclusionVolumeGBufferOutput::COLOR : return m_outputFrameTextureId[2];
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    enum class AmbientOcclusionVolumeGBufferOutput {
        POSITION=0,
        NORMAL=1,
        COLOR=2
    };

private:
    uint32_t m_outputFrameTextureId[3];
    uint32_t m_outputFrameDepthStencilBufferId;
};

class AmbientOcclusionVolumeBoundingPass : public Pass {
public:
    explicit AmbientOcclusionVolumeBoundingPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "AmbientOcclusionVolumeBoundingPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)AmbientOcclusionVolumeBoundingOutput::OCCLUSION : return m_outputFrameTextureId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    virtual void specifyInput(size_t inputId, uint32_t textureId) {
        switch(inputId) {
            case (size_t)AmbientOcclusionVolumeBoundingInput::POSITION : m_inputFrameTextureId[0] = textureId; break;
            case (size_t)AmbientOcclusionVolumeBoundingInput::NORMAL : m_inputFrameTextureId[1] = textureId; break;
            default: {
                std::cerr << "Wrong input frame Id" << std::endl;
                exit(1);
            }
        }
    }
    enum class AmbientOcclusionVolumeBoundingInput {
        POSITION=0,
        NORMAL=1
    };

    enum class AmbientOcclusionVolumeBoundingOutput {
        OCCLUSION=0
    };

private:
    uint32_t m_inputFrameTextureId[2];
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;
};

class AmbientOcclusionVolumeShadingSetting : public PassSetting {
public:
    std::vector<Light *> m_lightList;
    uint32_t m_lightUBO;
};

class AmbientOcclusionVolumeShadingPass : public Pass {
public:
    explicit AmbientOcclusionVolumeShadingPass(PassSetting *passSetting);

    virtual void renderPass(const std::vector<ShadeObject *> &shadingList);

    virtual std::string getType() {
        return "AmbientOcclusionVolumeShadingPass";
    }

    virtual uint32_t getOutputFrameTexture(size_t frameId) {
        switch(frameId) {
            case (size_t)AmbientOcclusionVolumeShadingOutput::IMAGE : return m_outputFrameTextureId;
            default: {
                std::cerr << "Wrong output frame Id" << std::endl;
                exit(1);
            }
        }
        return 0;
    }

    virtual void specifyInput(size_t inputId, uint32_t textureId) {
        switch(inputId) {
            case (size_t)AmbientOcclusionVolumeShadingInput::POSITION : m_inputFrameTextureId[0] = textureId; break;
            case (size_t)AmbientOcclusionVolumeShadingInput::NORMAL : m_inputFrameTextureId[1] = textureId; break;
            case (size_t)AmbientOcclusionVolumeShadingInput::COLOR : m_inputFrameTextureId[2] = textureId; break;
            case (size_t)AmbientOcclusionVolumeShadingInput::OCCLUSION : m_inputFrameTextureId[3] = textureId; break;
            default: {
                std::cerr << "Wrong input frame Id" << std::endl;
                exit(1);
            }
        }
    }

    enum class AmbientOcclusionVolumeShadingInput {
        POSITION=0,
        NORMAL=1,
        COLOR=2,
        OCCLUSION=3
    };

    enum class AmbientOcclusionVolumeShadingUBOInput {
        LIGHT=1
    };

    enum class AmbientOcclusionVolumeShadingOutput {
        IMAGE=0
    };


private:
    uint32_t m_inputFrameTextureId[4];
    uint32_t m_outputFrameTextureId;
    uint32_t m_outputFrameDepthStencilBufferId;

    uint32_t m_quadVao;
};


class AmbientOcclusionVolumeShadingPipeline : public LocalRenderingPipeline {
public:
    virtual void setupPipeline();

    virtual void blitFrameBuffer();

protected:
    virtual void renderAllPass();

private:
    AmbientOcclusionVolumeShadingSetting *shadingSetting;
};


#endif //RAY_AMBIENTOCCLUSIONVOLUMESHADINGPIPELINE_H
