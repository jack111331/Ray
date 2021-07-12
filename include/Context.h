//
// Created by Edge on 2021/7/12.
//

#ifndef RAY_CONTEXT_H
#define RAY_CONTEXT_H

#include "Config.h"
#include "Pipeline.h"

class Context {
public:
    static Context *getInstance();

    void loadConfig(const char *configName);

    Pipeline *getPipeline() {
        return m_pipeline;
    }
private:
    Context() : m_config(nullptr), m_pipeline(nullptr) {}
    static Context *m_instance;
    Config *m_config;
    Pipeline *m_pipeline;
};

#endif //RAY_CONTEXT_H
