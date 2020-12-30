#include <iostream>

#include <Pipeline.h>
#include "Config.h"

using namespace std;

int main(int argc, char **argv) {
    Config *config = new Config();
    Pipeline *pipeline = nullptr;
    if (argc > 1) {
        cout << "Reading config: " << argv[1] << endl;
        pipeline = config->loadConfig(argv[1]);
    } else {
        cout << "[ERROR] No valid argument" << endl;
        exit(1);
    }
    if (!pipeline) {
        cout << "[ERROR] Create pipeline failed" << endl;
        exit(1);
    }

    // Environment preparation
    pipeline->setupEnvironment();

    pipeline->setupPipeline();

    pipeline->pipelineLoop();

    return 0;
}
