//
// Created by Edge on 2020/10/17.
//
#include "BVH.h"

void Octree::insert(OctreeNode *node, const Hittable *object, const Coord &boundMin, const Coord &boundMax, int depth) {
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
        Coord boundingBoxCentroid = (objectBoundingBox.m_bounding[0] + objectBoundingBox.m_bounding[1]) * 0.5f;
        Coord nodeCentroid = (boundMin + boundMax) * 0.5f;
        uint32_t childIndex = 0;
        if (boundingBoxCentroid.x > nodeCentroid.x) childIndex += 4;
        if (boundingBoxCentroid.y > nodeCentroid.y) childIndex += 2;
        if (boundingBoxCentroid.z > nodeCentroid.z) childIndex += 1;
        Coord childBoundMin, childBoundMax;
        // compute child bound and pass down
        computeChildBound(childIndex, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
        if (node->m_child[childIndex] == nullptr) node->m_child[childIndex] = new OctreeNode;
        insert(node->m_child[childIndex], object, childBoundMin, childBoundMax, depth + 1);
    }
}

void Octree::computeChildBound(uint32_t index, const Coord &nodeCentroid, const Coord &boundMin, const Coord &boundMax,
                               Coord &pMin, Coord &pMax) {
    pMin.x = (index & 4) ? nodeCentroid.x : boundMin.x;
    pMin.y = (index & 2) ? nodeCentroid.y : boundMin.y;
    pMin.z = (index & 1) ? nodeCentroid.z : boundMin.z;
    pMax.x = (index & 4) ? boundMax.x : nodeCentroid.x;
    pMax.y = (index & 2) ? boundMax.y : nodeCentroid.y;
    pMax.z = (index & 1) ? boundMax.z : nodeCentroid.z;
}

void Octree::build(OctreeNode *node, const Coord &boundMin, const Coord &boundMax) {
    if (node->m_isLeaf) {
        // update node bounding box
        for (auto object: node->m_data) {
            node->m_nodeBoundingBox.updateBoundingBox(object->getBoundingBox());
        }
    } else {
        for (uint32_t i = 0; i < 8; ++i) {
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