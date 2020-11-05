//
// Created by Edge on 2020/10/17.
//

#ifndef RAY_BVH_H
#define RAY_BVH_H

#include "Utility.h"
#include "Hittable.h"

namespace objl {
    class Mesh;
    class Material;
}

class Object : public Hittable {
public:
    Object(const objl::Mesh &mesh);

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);
    bool isHitSingle(float tmin, uint32_t index, const Ray &ray, HitRecord &record) const;

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material);

public:
    std::vector<Coord> m_vertices;
    std::vector<Coord2D> m_texCoords;
    std::vector<Velocity> m_normals;
    std::vector<uint32_t> m_indices;
};

class BoundingBox : public Hittable {
public:
    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    void assign(const Coord &coord);

    void extendBy(const Coord &coord);

    Coord m_bounding[2];

};

class Cell {
public:
    struct TriangleDesc {
        TriangleDesc(const Object *object, uint32_t index) : object(object), index(index) {};
        const Object *object;
        uint32_t index;
    };
    void insert(Object *object, uint32_t index);
    bool intersect(float tmin, const Ray &ray, HitRecord &record) const;
    std::vector<TriangleDesc> m_triangles;
};

class Grid : public Hittable {
public:
    Grid(const objl::Mesh &mesh);

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material);

public:
    std::vector<Coord> m_vertices;
    std::vector<Velocity> m_normals;
    std::vector<uint32_t> m_indices;

    Coord m_gridMin;
    Coord m_gridRes;
    Vec3i m_gridDim;
    std::vector<std::vector<std::vector<std::vector<uint32_t>>>> m_cell;

    BoundingBox m_aabb;
private:
    Vec3i calculateCoordInCell(const Coord &coord);
};

class Extent {
public:
    Extent() : m_object(nullptr) {
        for (int i = 0; i < 7; ++i) {
            m_d[i][0] = 1e9;
            m_d[i][1] = -1e9;
        }
    }

    void extendBy(const Extent &extent);

    bool isHit(const Ray &ray, float precomputedNumerator[], float precomputeDenominator[], float &tNear, float &tFar,
               uint8_t &planeIndex);

    float m_d[7][2];
    static const Velocity m_planeSetNormals[7];
    Object *m_object;
    uint32_t m_triangleIndex;
};

struct OctreeNode {
    OctreeNode *m_child[8] = {};
    std::vector<const Extent *> m_data;
    Extent m_nodeExtent;
    bool m_isLeaf = true;
    ~OctreeNode() {
        for(int i = 0;i < 8;++i) {
            if(m_child[i]) {
                delete m_child[i];
            }
        }
    }
};

class Octree {
public:
    Octree(const Extent &extent);

    void insert(const Extent *extent) {
        insert(m_root, extent, m_bound[0], m_bound[1], 0);
    }

    void build() {
        build(m_root, m_bound[0], m_bound[1]);
    }

    struct QueueElement {
        const OctreeNode *node;
        float t;
        QueueElement(const OctreeNode *node, float tHit) : node(node), t(tHit) {}
        friend bool operator < (const QueueElement &a, const QueueElement &b) {return a.t>b.t;};
    };
    ~Octree() {
        if(m_root) {
            delete m_root;
        }
    }

private:
    void insert(OctreeNode *node, const Extent *extent, const Coord &boundMin, const Coord &boundMax, int depth);

    void computeChildBound(uint32_t index, const Coord &nodeCentroid, const Coord &boundMin, const Coord &boundMax, Coord &pMin, Coord &pMax);

    void build(OctreeNode *node, const Coord &boundMin, const Coord &boundMax);

public:
    Coord m_bound[2]; // for compute node centroid for later space partition
    OctreeNode *m_root = nullptr;
};

class PolygonMeshBVH : public Hittable {
public:
    PolygonMeshBVH(const std::string &objPath, Material *material);

    void computeTriangleBound(const Velocity &planeNormal, uint32_t objectIndex, uint32_t triangleIndex, float &dNear, float &dFar);

    void computeBounds(const Velocity &planeNormal, const Object *object, float &dNear, float &dFar);

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material);

    ~PolygonMeshBVH() {
        for(int i = 0;i < m_objects.size();++i) {
            delete[] m_extents[i];
        }
        delete[] m_extents;
        delete m_octree;
    }
public:
    std::vector<Object *> m_objects;

    Extent **m_extents;

    Octree *m_octree;
};

class PolygonMeshBV : public Hittable {
public:
    PolygonMeshBV(const std::string &objPath);

    void computeBound(const Velocity &planeNormal, const Object &object, float &dNear, float &dFar);

    bool intersect(uint32_t index, const Ray &ray);

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material);

public:
    uint32_t m_objectSize;
    Object *m_objects;

    Extent *m_extent;
};

class PolygonMesh : public Hittable {
public:
    PolygonMesh(const std::string &objPath);

    void buildBoundingBox();

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material);

    ~PolygonMesh() {
        for(uint32_t i = 0;i < m_objectSize;++i) {
            delete[] m_objects;
        }
    }

public:
    Object *m_objects;
    BoundingBox *m_aabb;
    uint32_t m_objectSize;
};


#endif //RAY_BVH_H
