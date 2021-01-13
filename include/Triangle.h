//
// Created by Edge on 2020/7/9.
//

#ifndef RAY_TRIANGLE_H
#define RAY_TRIANGLE_H


#include "Utility.h"
#include "Hittable.h"
#include "Material.h"
#include "Scene.h"
#include "BLASNode.h"

class TriangleNode {
public:
    TriangleNode() : m_coord(Vec3f(0, 0, 0)),
                     m_texCoord(
                             (Vec2f{0, 0})),
                     m_normal(Vec3f(0, 0, 0)),
                     m_hasOther(
                             false) {}

    TriangleNode(const Vec3f &coord, const Vec2f &texCoord, const Vec3f &normal, bool hasOther) : m_coord(coord),
                                                                                                  m_texCoord(
                                                                                                               texCoord),
                                                                                                  m_normal(normal),
                                                                                                  m_hasOther(
                                                                                                               hasOther) {}

    Vec3f m_coord;
    Vec2f m_texCoord;
    Vec3f m_normal;
    bool m_hasOther = false;
};

class Triangle : public BLASNode {
public:
    Triangle() : BLASNode(), m_point{} {}

    Triangle(Vec3f (*coord)[3]) : m_point{} {
        for (int i = 0; i < 3; ++i) {
            m_point[i] = new TriangleNode;
            m_point[i]->m_coord = (*coord)[i];
            m_boundingBox.updateBoundingBox(m_point[i]->m_coord);
        }
    }

    void fromTriangleNode(Material *material, const Scene *scene, TriangleNode *firstNode,
                          TriangleNode *secondNode, TriangleNode *thirdNode) {
        m_point[0] = firstNode;
        m_point[1] = secondNode;
        m_point[2] = thirdNode;

        setMaterial(material);
        m_boundingBox.updateBoundingBox(m_point[0]->m_coord);
        m_boundingBox.updateBoundingBox(m_point[1]->m_coord);
        m_boundingBox.updateBoundingBox(m_point[2]->m_coord);
    }

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    virtual  void createVAO(std::vector<ShadeObject *> &shadeObjectList);

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const;

    TriangleNode *m_point[3];
};


#endif //RAY_TRIANGLE_H
