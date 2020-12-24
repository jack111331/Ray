//
// Created by Edge on 2020/7/8.
//

#ifndef RAY_CAMERA_H
#define RAY_CAMERA_H


#include "Utility.h"
#include <yaml-cpp/yaml.h>

class Camera {
public:
    Camera() : m_screen(nullptr) {}
    Camera(int width, int height, Coord eyeCoord, double fov, Velocity direction, Velocity up);
    void toPpm(const std::string &filename) const;
    void bufferToTexture(uint32_t bufferId) const;

    // Need more abstraction, better use adapter to adapt different type of file
    bool readCameraInfo(const YAML::Node &node);

    int m_width, m_height;
    Color **m_screen;

    Coord m_eyeCoord;
    float m_fov;
    Velocity m_direction, m_up;
};


#endif //RAY_CAMERA_H
