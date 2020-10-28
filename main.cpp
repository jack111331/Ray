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

static bool localRendering = false;
static bool whitted = false;
static bool photonMapping = false;

using namespace std;
struct Object {
    ObjectInfo m_objectInfo;
    Material *m_material;
};

static vector<Object> objectList;
static Shader *objectShader;

void printGraphicCardInfo() {
    const GLubyte *vendor = glGetString(GL_VENDOR); // Returns the vendor
    const GLubyte *renderer = glGetString(GL_RENDERER);
    cout << "Current Vendor: " << (const char *) vendor << endl;
    cout << "Current Renderer: " << (const char *) renderer << endl;
}


void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void initLocalRendering(const Scene *scene) {
    fflush(stdout);
    auto hittableList = scene->m_hittableList->m_hittableList;
    for (auto hittable : hittableList) {
        std::vector<ObjectInfo> objectInfoList = hittable->createVAO(hittable->m_material);
        for(auto objectInfo: objectInfoList) {
            objectList.push_back({objectInfo, hittable->m_material});
        }
    }
    objectShader = new Shader("shader/mesh.vs", "shader/mesh.fs");
    objectShader->buildShader();

}

void localRender(vector<Object> &objectList, const Camera *camera, const vector<Light *> &lightList) {
    for (auto object: objectList) {
        // shader binding
        objectShader->bind();
        // object binding
        glBindVertexArray(object.m_objectInfo.m_vao);
        // uniform
        const Material *material = object.m_material;
        glm::mat4 projection = glm::perspective(glm::radians(camera->m_fov), (float) camera->m_width / (float) camera->m_height,
                                                0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(camera->m_eyeCoord.x, camera->m_eyeCoord.y, camera->m_eyeCoord.z),
                                     glm::vec3(camera->m_direction.x, camera->m_direction.y, camera->m_direction.z),
                                     glm::vec3(camera->m_up.x, camera->m_up.y, camera->m_up.z));
        objectShader->uniformMat4f("projection", projection);
        objectShader->uniformMat4f("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        objectShader->uniformMat4f("model", model);

        objectShader->uniform3f("lightPos", lightList[0]->m_origin.x, lightList[0]->m_origin.y,
                                lightList[0]->m_origin.z);
        objectShader->uniform3f("viewPos", camera->m_eyeCoord.x, camera->m_eyeCoord.y, camera->m_eyeCoord.z);
        objectShader->uniform3f("lightColor", 1.0, 1.0, 1.0);
        // TODO add bvh support
        objectShader->uniform3f("objectColor", material->m_color.r, material->m_color.g, material->m_color.b);
        // draw call
        glDrawElements(GL_TRIANGLES, object.m_objectInfo.m_indicesAmount, GL_UNSIGNED_INT, 0);
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {
        if (!strcmp(argv[1], "LR")) {
            localRendering = true;
        } else if(!strcmp(argv[1], "WT")) {
            whitted = true;
        } else if(!strcmp(argv[1], "PM")) {
            photonMapping = true;
        }
    }
    IlluminationModel *model;
    if(whitted) {
        model = IlluminationModelFactory::createModel("Whitted");
    } else if(photonMapping) {
        model = IlluminationModelFactory::createModel("PhotonMapping");
    } else {
        model = IlluminationModelFactory::createModel("Phong");
    }
    Scene *scene = new Scene(model);
    Config *config = new Config();

    if(!whitted && !photonMapping) {
//        config->loadConfig("config/hw2_input.txt", scene);
        config->loadConfig("config/input_cornell.txt", scene);
//        config->loadConfig("config/input_sibenik.txt", scene);
    } else if (whitted){
//        config->loadConfig("config/input_whitted.txt", scene);
//        config->loadConfig("config/input_with_model.txt", scene);
//        config->loadConfig("config/input_with_model_monkey.txt", scene);
//        config->loadConfig("config/input_with_model_bv.txt", scene);
//        config->loadConfig("config/input_with_model_bvh.txt", scene);
//        config->loadConfig("config/input_with_model_bvh_monkey.txt", scene);
        config->loadConfig("config/input_cornell.txt", scene);
//        config->loadConfig("config/input_sibenik.txt", scene);
    } else {
//        config->loadConfig("config/input_with_model_pm.txt", scene);
        config->loadConfig("config/input_cornell_pm.txt", scene);
    }
    const Camera *camera = scene->m_camera;
    if (!localRendering) {
        if(!photonMapping) {
            scene->displayScene();
        } else {
            scene->displayPhotonMappingScene();
        }
        camera->toPpm("output.ppm");
    }

    // Environment preparation
    if (!glfwInit()) {
        cout << "[GLFW] init Failed" << endl;
        return 0;
    }

    if (localRendering) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    GLFWwindow *window = glfwCreateWindow(scene->m_camera->m_width, scene->m_camera->m_height, "Ray", NULL,
                                          NULL);
    if (window == nullptr) {
        std::cout << "[GLFW] failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewInit();

    if (localRendering) {
        initLocalRendering(scene);
    }

    printGraphicCardInfo();

    uint32_t frameTextureId;
    glGenTextures(1, &frameTextureId);
    glBindTexture(GL_TEXTURE_2D, frameTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    while (!glfwWindowShouldClose(window)) {
        // Ray tracing
        if (!localRendering) {
            // Setup GL
            glViewport(0, 0, camera->m_width, camera->m_height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            float ratio = camera->m_width / (float) camera->m_height;
            glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            camera->bufferToTexture(frameTextureId);
//        glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, frameTextureId);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, 0.0);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-1.0, 1.0, 0.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, 0.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, -1.0, 0.0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFlush();
        } else {
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            localRender(objectList, scene->m_camera, scene->m_lightList);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
