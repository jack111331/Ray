//
// Created by Edge on 2020/7/8.
//

#ifndef RAY_CAMERA_H
#define RAY_CAMERA_H


#include "Utility.h"
#include <yaml-cpp/yaml.h>

class Camera {
public:
    Camera() : m_screen(nullptr), m_width(0), m_height(0) {}
    Camera(int width, int height, Vec3f eyeCoord, double fov, Vec3f direction, Vec3f up);
    void toPpm(const std::string &filename) const;
    void initializeScreen();
    void bufferToTexture(uint32_t bufferId) const;

    // Need more abstraction, better use adapter to adapt different type of file
    bool readCameraInfo(const YAML::Node &node);

    int m_width, m_height;
    Vec3f **m_screen;

    Vec3f m_eyeCoord;
    float m_fov;
    Vec3f m_direction, m_up;

};


#endif //RAY_CAMERA_H
