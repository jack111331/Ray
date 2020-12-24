//
// Created by Edge on 2020/12/25.
//

#ifndef RAY_TRIANGLEGROUP_H
#define RAY_TRIANGLEGROUP_H

#include "Hittable.h"
#include "OBJ_Loader.h"

class TriangleGroup : public Hittable {
public:
    bool fromObj(const objl::Mesh &mesh, const Scene *scene);
    bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);
    bool isHitSingle(float tmin, uint32_t index, const Ray &ray, HitRecord &record) const;

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material);

    virtual ObjectBoundingBox getBoundingBox();

public:
    std::vector<Coord> m_vertices;
    std::vector<Coord2D> m_texCoords;
    std::vector<Velocity> m_normals;
    std::vector<uint32_t> m_indices;
    ObjectBoundingBox m_boundingBox;
};


#endif //RAY_TRIANGLEGROUP_H
