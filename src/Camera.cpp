//
// Created by Edge on 2020/7/8.
//

#include <cstdint>
#include <fstream>
#include "glad/glad.h"
#include "Camera.h"

using std::ofstream;
using std::ios;

Camera::Camera(int width, int height, Vec3f eyeCoord, double fov, Vec3f direction, Vec3f up) : m_width(width),
                                                                                               m_height(height),
                                                                                               m_eyeCoord(
                                                                                                             eyeCoord),
                                                                                               m_fov(fov),
                                                                                               m_direction(
                                                                                                             direction),
                                                                                               m_up(up) {
    m_screen = new Vec3f *[height];
    for (int i = 0; i < height; ++i) {
        m_screen[i] = new Vec3f[width];
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
            ofs << (uint8_t) (m_screen[i][j].x > 1.0 ? 255 : (255.99 * m_screen[i][j].x))
                << (uint8_t) (m_screen[i][j].y > 1.0 ? 255 : (255.99 * m_screen[i][j].y))
                << (uint8_t) (m_screen[i][j].z > 1.0 ? 255 : (255.99 * m_screen[i][j].z));
        }
    }

}

void Camera::initializeScreen() {
    if(!m_width || !m_height) {
        std::cerr << "No camera width or height provided" << std::endl;
        exit(1);
    }
    m_screen = new Vec3f *[m_height];
    for (int i = 0; i < m_height; ++i) {
        m_screen[i] = new Vec3f[m_width];
    }
}

void Camera::bufferToTexture(uint32_t bufferId) const {
    glBindTexture(GL_TEXTURE_2D, bufferId);
    uint8_t *buffer = new uint8_t[3 * m_height * m_width];
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            buffer[3 * (i * m_width + j)] = (uint8_t) (m_screen[i][j].x > 1.0 ? 255 : (255.99 * m_screen[i][j].x));
            buffer[3 * (i * m_width + j) + 1] = (uint8_t) (m_screen[i][j].y > 1.0 ? 255 : (255.99 * m_screen[i][j].y));
            buffer[3 * (i * m_width + j) + 2] = (uint8_t) (m_screen[i][j].z > 1.0 ? 255 : (255.99 * m_screen[i][j].z));
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    delete[] buffer;
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Camera::readCameraInfo(const YAML::Node &node) {
    if (!node["position"] || !node["direction"] || !node["up"] || !node["FOV"] || !node["resolution"]) {
        return false;
    }
    m_eyeCoord = Vec3f::toVec3f(node["position"].as<std::vector<float>>());
    m_direction = Vec3f::toVec3f(node["direction"].as<std::vector<float>>());
    m_up = Vec3f::toVec3f(node["up"].as<std::vector<float>>());
    m_fov = node["FOV"].as<float>();

    const std::vector<float> &resolutionList = node["resolution"].as<std::vector<float>>();
    m_width = resolutionList[0];
    m_height = resolutionList[1];
    return true;
}
