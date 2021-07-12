//
// Created by Edge on 2021/7/12.
//

#include <wx/wx.h>
#include "Context.h"

Context *Context::m_instance = nullptr;

Context *Context::getInstance() {
    if (Context::m_instance == nullptr) {
        Context::m_instance = new Context();

    }
    return Context::m_instance;
}

void Context::loadConfig(const char *configName) {
    m_config = new Config();
    wxLogMessage("Reading config: %s\n", configName);
    m_pipeline = m_config->loadConfig(configName);
    if (!m_pipeline) {
        wxLogMessage("[ERROR] Create pipeline failed\n");
        exit(1);
    }
    m_pipeline->setupEnvironment();
    m_pipeline->setupPipeline();
    wxLogMessage("Pipeline setup\n");
}