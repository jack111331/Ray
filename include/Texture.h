//
// Created by Edge on 2020/11/2.
//

#ifndef RAY_TEXTURE_H
#define RAY_TEXTURE_H

#include <string>
#include "Utility.h"

class Texture {
public:
    Texture(const std::string &filename);
    Color getColor(const Coord2D &texCoord);
    uint8_t *m_image;
    int m_width, m_height, m_channel;
};


#endif //RAY_TEXTURE_H
