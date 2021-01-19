//
// Created by Edge on 2020/10/17.
//

#ifndef RAY_BVH_H
#define RAY_BVH_H

#include "Utility.h"
#include "ObjectNode.h"

struct OctreeNode {
    OctreeNode *m_child[8] = {};
    std::vector<const ObjectNode *> m_data;
    ObjectBoundingBox m_nodeBoundingBox;
    int m_inTwoLevelBVHId;
    bool m_isLeaf = true;
    int m_startIdx;
    int m_intermediateNodeIdx = -1;

    ~OctreeNode() {
        for (uint32_t i = 0; i < 8; ++i) {
            if (m_child[i]) {
                delete m_child[i];
            }
        }
    }
};

class Octree {
public:
    Octree(const ObjectBoundingBox &boundingBox) {
        Vec3f diff = boundingBox.m_bounding[1] - boundingBox.m_bounding[0];
        float dim = std::max(diff.x, std::max(diff.y, diff.z));
        Vec3f centroid = (boundingBox.m_bounding[1] + boundingBox.m_bounding[0]);
        Vec3f radius = {dim, dim, dim};
        m_bound[0] = (centroid - radius) * 0.5f;
        m_bound[1] = (centroid + radius) * 0.5f;
        m_root = new OctreeNode;
    }

    void insert(const ObjectNode *object) {
        insert(m_root, object, m_bound[0], m_bound[1], 0);
    }

    void build() {
        build(m_root, m_bound[0], m_bound[1]);
    }

    struct QueueElement {
        const OctreeNode *node;
        float t;

        QueueElement(const OctreeNode *node, float tHit) : node(node), t(tHit) {}

        friend bool operator<(const QueueElement &a, const QueueElement &b) { return a.t > b.t; };
    };

    ~Octree() {
        if (m_root) {
            delete m_root;
        }
    }

private:
    void insert(OctreeNode *node, const ObjectNode *object, const Vec3f &boundMin, const Vec3f &boundMax, int depth);

    void computeChildBound(uint32_t index, const Vec3f &nodeCentroid, const Vec3f &boundMin, const Vec3f &boundMax,
                           Vec3f &pMin, Vec3f &pMax);

    void build(OctreeNode *node, const Vec3f &boundMin, const Vec3f &boundMax);


public:
    Vec3f m_bound[2]; // for compute node centroid for later space partition
    OctreeNode *m_root = nullptr;
    int m_nodeCount = 0;
    std::vector<uint32_t> m_packedIndices;

};

class TriangleGroup;
class GroupObj;
class GeometryGroupObj;

class BVH {
public:
    BVH(TriangleGroup *triangleGroup);

    void updateBVH(TriangleGroup *triangleGroup);

    // TODO we need refactor
    BVH(GroupObj *group);

    void updateBVH(GroupObj *group);

    BVH(GeometryGroupObj *group);

    void updateBVH(GeometryGroupObj *group);

    bool isHit(const Ray &ray, IntersectionRecord &record, float tmin=0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0));

    void flattenBVH(int offsetVertices) {
        flattenBVH(m_octree, m_octree->m_root, offsetVertices);
    }

    Octree *m_octree;
private:
    void flattenBVH(Octree *tree, OctreeNode *node, int offsetVertices);



};

#endif //RAY_BVH_H
