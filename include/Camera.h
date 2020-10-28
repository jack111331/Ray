//
// Created by Edge on 2020/7/8.
//

#ifndef ADVANCED_COMPUTER_GRAPH_CAMERA_H
#define ADVANCED_COMPUTER_GRAPH_CAMERA_H


#include "Utility.h"

class Camera {
public:
    Camera(int width, int height, Coord eyeCoord, double fov, Velocity direction, Velocity up);
    void toPpm(const std::string &filename) const;
    void bufferToTexture(uint32_t bufferId) const;

    int m_width, m_height;
    Color **m_screen;

    Coord m_eyeCoord;
    float m_fov;
    Velocity m_direction, m_up;
};


#endif //ADVANCED_COMPUTER_GRAPH_CAMERA_H
