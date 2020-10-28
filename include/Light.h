//
// Created by Edge on 2020/7/13.
//

#ifndef ADVANCED_COMPUTER_GRAPH_LIGHT_H
#define ADVANCED_COMPUTER_GRAPH_LIGHT_H


#include "Utility.h"

class Light {
public:
    Light(Coord origin) : m_origin(origin) {}

    Coord m_origin;
};

#endif //ADVANCED_COMPUTER_GRAPH_LIGHT_H
