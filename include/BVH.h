//
// Created by Edge on 2020/10/17.
//

#ifndef RAY_BVH_H
#define RAY_BVH_H

#include "Utility.h"
#include "Hittable.h"

struct OctreeNode {
    OctreeNode *m_child[8] = {};
    std::vector<const Hittable *> m_data;
    ObjectBoundingBox m_nodeBoundingBox;
    bool m_isLeaf = true;

    ~OctreeNode() {
        for (int i = 0; i < 8; ++i) {
            if (m_child[i]) {
                delete m_child[i];
            }
        }
    }
};

class Octree {
public:
    Octree(const ObjectBoundingBox &boundingBox) {
        // constructor compute bound by first computing centroid and then add dimension to form bound
        Velocity diff = boundingBox.m_bounding[1] - boundingBox.m_bounding[0];
        float dim = std::max(diff.x, std::max(diff.y, diff.z)) / 2;
        // technically, this is not true centroid, just use this for computational efficiency
        Coord centroid = (boundingBox.m_bounding[1] + boundingBox.m_bounding[0]) / 2.0f;
        Velocity radius = {dim, dim, dim};
        m_bound[0] = (centroid - radius);
        m_bound[1] = (centroid + radius);
        m_root = new OctreeNode;
    }

    void insert(const Hittable *object) {
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
    void insert(OctreeNode *node, const Hittable *object, const Coord &boundMin, const Coord &boundMax, int depth);

    void computeChildBound(uint32_t index, const Coord &nodeCentroid, const Coord &boundMin, const Coord &boundMax,
                           Coord &pMin, Coord &pMax);

    void build(OctreeNode *node, const Coord &boundMin, const Coord &boundMax);


public:
    Coord m_bound[2]; // for compute node centroid for later space partition
    OctreeNode *m_root = nullptr;
};

#endif //RAY_BVH_H
