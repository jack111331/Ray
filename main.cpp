#include <iostream>

#include "Sphere.h"
#include "Scene.h"
#include "Config.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstring>

#include "HittableList.h"
#include <vector>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <IllumModelFactory.h>
#include <sys/time.h>
#include <Lambertian.h>
#include <Pipeline.h>

static bool localRendering = false;

using namespace std;

static Shader *objectShader;

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}


int main(int argc, char **argv) {
    Config *config = new Config();
    Pipeline *pipeline = nullptr;
    if (argc > 1) {
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
