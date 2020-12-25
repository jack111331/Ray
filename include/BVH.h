//
// Created by Edge on 2020/10/17.
//

#ifndef RAY_BVH_H
#define RAY_BVH_H

#include "Utility.h"
#include "Hittable.h"

template<typename T>
struct OctreeNode {
    OctreeNode *m_child[8] = {};
    std::vector<T *> m_data;
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

template<typename T>
class Octree {
public:
    Octree(const ObjectBoundingBox &boundingBox);

    void insert(const T *object) {
        insert(m_root, object, m_bound[0], m_bound[1], 0);
    }

    void build() {
        build(m_root, m_bound[0], m_bound[1]);
    }

    struct QueueElement {
        const OctreeNode<T> *node;
        float t;

        QueueElement(const OctreeNode<T> *node, float tHit) : node(node), t(tHit) {}

        friend bool operator<(const QueueElement &a, const QueueElement &b) { return a.t > b.t; };
    };

    ~Octree() {
        if (m_root) {
            delete m_root;
        }
    }

private:
    void insert(OctreeNode<T> *node, const T *object, const Coord &boundMin, const Coord &boundMax, int depth);

    void computeChildBound(uint32_t index, const Coord &nodeCentroid, const Coord &boundMin, const Coord &boundMax,
                           Coord &pMin, Coord &pMax);

    void build(OctreeNode<T> *node, const Coord &boundMin, const Coord &boundMax);

public:
    Coord m_bound[2]; // for compute node centroid for later space partition
    OctreeNode<T> *m_root = nullptr;
};

#endif //RAY_BVH_H
