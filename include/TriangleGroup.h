//
// Created by Edge on 2020/12/25.
//

#ifndef RAY_TRIANGLEGROUP_H
#define RAY_TRIANGLEGROUP_H

#include "ObjectNode.h"
#include "Triangle.h"
#include "BVH.h"

class TriangleGroup;


namespace objl {
    class Mesh;
}


class TriangleGroup : public BLASNode {
public:
    TriangleGroup() : BLASNode(), m_individualTriangle(false) {}

    static std::map<std::string, ObjectNode *> fromObj(const YAML::Node &node, const Scene *scene);

    bool fromObj(const objl::Mesh &mesh, const Scene *scene, const std::string &materialName = "");

    bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f,
                       const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    // For local shading
    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList, const glm::mat4 &transformMat = glm::mat4(1.0f));

    virtual ObjectBoundingBox getBoundingBox() const;

    virtual BLASNodeType getBLASNodeType() {
        return BLASNodeType::TRIANGLE_GROUP;
    }


public:
    std::vector<Triangle *> m_triangles;

    bool m_individualTriangle;

    std::vector<TriangleNode *> m_nodeList;

    std::string m_objFilename;
};


#endif //RAY_TRIANGLEGROUP_H
