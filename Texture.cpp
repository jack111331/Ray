//
// Created by Edge on 2020/11/2.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

Texture::Texture(const std::string &filename) {
    m_image = stbi_load(filename.c_str(), &m_width, &m_height, &m_channel, 3);
    if(!m_image) {
        std::cerr << "Texture load fail, filename:" << filename << std::endl;
        exit(1);
    }
}

Color Texture::getColor(const Coord2D &texCoord) {
    float width = m_width, height = m_height;
    Coord2D texCoordFlip = {texCoord.x, 1.0f - texCoord.y};
    Coord2Di cornerCoord[4] = {{(int) floor(texCoordFlip.x * width),     (int) floor(texCoordFlip.y * height-1)},
                               {(int) floor(texCoordFlip.x * width),     (int) floor(texCoordFlip.y * height)},
                               {(int) floor(texCoordFlip.x * width + 1), (int) floor(texCoordFlip.y * height-1)},
                               {(int) floor(texCoordFlip.x * width + 1), (int) floor(texCoordFlip.y * height)}};

    Color cornerColor[4];
    for (int i = 0; i < 4; ++i) {
        int index = 3 * (cornerCoord[i].y * m_width + cornerCoord[i].x);
        cornerColor[i].r = (float) m_image[index] / 255.99f;
        cornerColor[i].g = (float) m_image[index + 1] / 255.99f;
        cornerColor[i].b = (float) m_image[index + 2] / 255.99f;
    }
    float alpha = texCoordFlip.x * width - cornerCoord[0].x;
    float beta = texCoordFlip.y * height - cornerCoord[0].y;
    return (1 - alpha) * ((1 - beta) * cornerColor[0] + beta * cornerColor[1]) +
           alpha * ((1 - beta) * cornerColor[2] + beta * cornerColor[3]);
}

