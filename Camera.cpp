//
// Created by Edge on 2020/7/8.
//

#include <cstdint>
#include <fstream>
#include <GL/glew.h>
#include "Camera.h"

using std::ofstream;
using std::ios;

Camera::Camera(int width, int height, Coord eyeCoord, double fov, Velocity direction, Velocity up) : m_width(width),
                                                                                                    m_height(height),
                                                                                                    m_eyeCoord(
                                                                                                            eyeCoord),
                                                                                                    m_fov(fov),
                                                                                                    m_direction(
                                                                                                            direction),
                                                                                                    m_up(up) {
    m_screen = new Color *[height];
    for (int i = 0; i < height; ++i) {
        m_screen[i] = new Color[width];
    }
}

void Camera::toPpm(const std::string &filename) const {
    ofstream ofs;
    ofs.open(filename, ios::binary);
    ofs << "P6\n";
    ofs << std::to_string(m_width) << " " << std::to_string(m_height) << std::endl;
    ofs << std::to_string(255) << std::endl;


    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            ofs << m_screen[i][j];
        }
    }

}
void Camera::bufferToTexture(uint32_t bufferId) const {
    glBindTexture(GL_TEXTURE_2D, bufferId);
    uint8_t *buffer = new uint8_t[3 * m_height * m_width];
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            buffer[3 * (i*m_width + j)] = (uint8_t)(255.99 * m_screen[i][j].r);
            buffer[3 * (i*m_width + j)+1] = (uint8_t)(255.99 * m_screen[i][j].g);
            buffer[3 * (i*m_width + j)+2] = (uint8_t)(255.99 * m_screen[i][j].b);
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    delete[] buffer;
    glBindTexture(GL_TEXTURE_2D, 0);
}
