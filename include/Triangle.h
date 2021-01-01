//
// Created by Edge on 2020/7/9.
//

#ifndef RAY_TRIANGLE_H
#define RAY_TRIANGLE_H


#include "Utility.h"
#include "Hittable.h"
#include "Material.h"

class TriangleNode {
public:
    Coord m_coord;
    Coord2D m_texCoord;
    Velocity m_normal;
};

class Triangle : public Hittable {
public:
    Triangle(): m_point{} {}

    Triangle(Coord (*coord)[3]): m_point{} {
        for (int i = 0; i < 3; ++i) {
            m_point[i] = new TriangleNode;
            m_point[i]->m_coord = (*coord)[i];
            m_boundingBox.updateBoundingBox(m_point[i]->m_coord);
        }
    }

    void fromTriangleNode(const std::string &materialName, const Scene *scene, TriangleNode *firstNode, TriangleNode *secondNode, TriangleNode *thirdNode) {
        auto it = scene->m_materialTable.find(materialName);
        if(it == scene->m_materialTable.end()) {
            // TODO better handler require
            exit(1);
        }
        setMaterial(it->second);
        m_point[0] = firstNode;
        m_point[1] = secondNode;
        m_point[2] = thirdNode;
    }

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record) const;

    virtual std::vector<ShadeObject *> createVAO();

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const;

    TriangleNode *m_point[3];

    ObjectBoundingBox m_boundingBox;
};


#endif //RAY_TRIANGLE_H
