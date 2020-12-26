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
    Octree(const ObjectBoundingBox &boundingBox) {
        // constructor compute bound by first computing centroid and then add dimension to form bound
        Velocity diff = boundingBox.m_bounding[1] - boundingBox.m_bounding[0];
        float dim = std::max(diff.x, std::max(diff.y, diff.z)) / 2;
        // technically, this is not true centroid, just use this for computational efficiency
        Coord centroid = (boundingBox.m_bounding[1] + boundingBox.m_bounding[0]) / 2.0f;
        m_bound[0] = (centroid - (Velocity) {dim, dim, dim});
        m_bound[1] = (centroid + (Velocity) {dim, dim, dim});
        m_root = new OctreeNode<T>;
    }

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
    void insert(OctreeNode<T> *node, const T *object, const Coord &boundMin, const Coord &boundMax, int depth){
        if (node->m_isLeaf) {
            // insert extent into node or reallocate node extents' to child node
            if (node->m_data.empty() || depth == 16) {
                node->m_data.push_back(object);
            } else {
                node->m_isLeaf = false;
                while (!node->m_data.empty()) {
                    // recursion and go to non leaf handling part
                    insert(node, node->m_data.back(), boundMin, boundMax, depth);
                    node->m_data.pop_back();
                }
                insert(node, object, boundMin, boundMax, depth);
            }
        } else {
            // compute centroid and determine which child this extent should go
            const ObjectBoundingBox &objectBoundingBox = object->getBoundingBox();
            Coord boundingBoxCentroid = (objectBoundingBox.m_bounding[0] + objectBoundingBox.m_bounding[1]) / 2.0;
            Coord nodeCentroid = (boundMin + boundMax) * 0.5f;
            uint32_t childIndex = 0;
            if (boundingBoxCentroid.x > nodeCentroid.x) childIndex += 4;
            if (boundingBoxCentroid.y > nodeCentroid.y) childIndex += 2;
            if (boundingBoxCentroid.z > nodeCentroid.z) childIndex += 1;
            Coord childBoundMin, childBoundMax;
            // compute child bound and pass down
            computeChildBound(childIndex, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
            if (node->m_child[childIndex] == nullptr) node->m_child[childIndex] = new OctreeNode<T>;
            insert(node->m_child[childIndex], object, childBoundMin, childBoundMax, depth + 1);
        }
    }

    void computeChildBound(uint32_t index, const Coord &nodeCentroid, const Coord &boundMin, const Coord &boundMax,
                           Coord &pMin, Coord &pMax){
        pMin.x = (index & 4) ? nodeCentroid.x : boundMin.x;
        pMin.y = (index & 2) ? nodeCentroid.y : boundMin.y;
        pMin.z = (index & 1) ? nodeCentroid.z : boundMin.z;
        pMax.x = (index & 4) ? boundMax.x : nodeCentroid.x;
        pMax.y = (index & 2) ? boundMax.y : nodeCentroid.y;
        pMax.z = (index & 1) ? boundMax.z : nodeCentroid.z;
    }

    void build(OctreeNode<T> *node, const Coord &boundMin, const Coord &boundMax){
        if (node->m_isLeaf) {
            // update node bounding box
            for (auto object:node->m_data) {
                node->m_nodeBoundingBox.updateBoundingBox(object->m_boundingBox);
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                if (node->m_child[i]) {
                    Coord nodeCentroid = (boundMin + boundMax) * 0.5f;
                    Coord childBoundMin, childBoundMax;
                    // compute child bound and pass down
                    computeChildBound(i, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
                    build(node->m_child[i], childBoundMin, childBoundMax);
                    // collect child extent compute result
                    node->m_nodeBoundingBox.updateBoundingBox(node->m_child[i]->m_nodeBoundingBox);
                }
            }
        }
    }


public:
    Coord m_bound[2]; // for compute node centroid for later space partition
    OctreeNode<T> *m_root = nullptr;
};

#endif //RAY_BVH_H
