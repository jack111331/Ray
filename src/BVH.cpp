//
// Created by Edge on 2020/10/17.
//
#include "BVH.h"
#include "GroupObj.h"
#include "GeometryGroupObj.h"
#include "TriangleGroup.h"
#include <queue>

using namespace std;

void Octree::insert(OctreeNode *node, const ObjectNode *object, const Vec3f &boundMin, const Vec3f &boundMax, int depth) {
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
        Vec3f boundingBoxCentroid = (objectBoundingBox.m_bounding[0] + objectBoundingBox.m_bounding[1]) * 0.5f;
        Vec3f nodeCentroid = (boundMin + boundMax) * 0.5f;
        uint32_t childIndex = 0;
        if (boundingBoxCentroid.x > nodeCentroid.x) childIndex += 4;
        if (boundingBoxCentroid.y > nodeCentroid.y) childIndex += 2;
        if (boundingBoxCentroid.z > nodeCentroid.z) childIndex += 1;
        Vec3f childBoundMin, childBoundMax;
        // compute child bound and pass down
        computeChildBound(childIndex, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
        if (node->m_child[childIndex] == nullptr) node->m_child[childIndex] = new OctreeNode;
        insert(node->m_child[childIndex], object, childBoundMin, childBoundMax, depth + 1);
    }
}

void Octree::computeChildBound(uint32_t index, const Vec3f &nodeCentroid, const Vec3f &boundMin, const Vec3f &boundMax,
                               Vec3f &pMin, Vec3f &pMax) {
    pMin.x = (index & 4) ? nodeCentroid.x : boundMin.x;
    pMin.y = (index & 2) ? nodeCentroid.y : boundMin.y;
    pMin.z = (index & 1) ? nodeCentroid.z : boundMin.z;
    pMax.x = (index & 4) ? boundMax.x : nodeCentroid.x;
    pMax.y = (index & 2) ? boundMax.y : nodeCentroid.y;
    pMax.z = (index & 1) ? boundMax.z : nodeCentroid.z;
}

void Octree::build(OctreeNode *node, const Vec3f &boundMin, const Vec3f &boundMax) {
    m_nodeCount++;
    if (node->m_isLeaf) {
        // update node bounding box
        for (auto object: node->m_data) {
            node->m_nodeBoundingBox.updateBoundingBox(object->getBoundingBox());
        }
    } else {
        for (uint32_t i = 0; i < 8; ++i) {
            if (node->m_child[i]) {
                Vec3f nodeCentroid = (boundMin + boundMax) * 0.5f;
                Vec3f childBoundMin, childBoundMax;
                // compute child bound and pass down
                computeChildBound(i, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
                build(node->m_child[i], childBoundMin, childBoundMax);
                // collect child extent compute result
                node->m_nodeBoundingBox.updateBoundingBox(node->m_child[i]->m_nodeBoundingBox);
            }
        }
    }
}

BVH::BVH(TriangleGroup *triangleGroup) : m_octree(nullptr) {

    updateBVH(triangleGroup);
}

bool BVH::isHit(const Ray &ray, IntersectionRecord &record, float tmin, const glm::mat4 &transformMat) {
    float tNear = 1e9, tFar = -1e9;
    if (!m_octree->m_root->m_nodeBoundingBox.isHit(ray, tNear, tFar)) {
        return false;
    }
    priority_queue<Octree::QueueElement> pq;
    pq.push({Octree::QueueElement(m_octree->m_root, 0)});
    bool isHitted = false;
    while (!pq.empty() && (pq.top().t < record.t || record.t < 0)) {
        const OctreeNode *node = pq.top().node;
        pq.pop();
        if (node->m_isLeaf) {
            for (uint32_t i = 0; i < node->m_data.size(); ++i) {
                if (node->m_data[i]->isHit(ray, record, tmin, transformMat)) {
                    isHitted = true;
                }
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                if (node->m_child[i]) {
                    float tNearChild = 0, tFarChild = tFar;
                    if (node->m_child[i]->m_nodeBoundingBox.isHit(ray, tNearChild, tFarChild)) {
                        float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
                        pq.push(Octree::QueueElement(node->m_child[i], t));
                    }
                }
            }
        }
    }
    return isHitted;
}

void BVH::updateBVH(TriangleGroup *triangleGroup) {
    if (m_octree) {
        delete m_octree;
    }
    m_octree = new Octree(triangleGroup->getBoundingBox());
    for (auto triangle: triangleGroup->m_triangles) {
        m_octree->insert(triangle);
    }
    m_octree->build();
}

BVH::BVH(GroupObj *group) : m_octree(nullptr) {
    updateBVH(group);
}

void BVH::updateBVH(GroupObj *group) {
    if (m_octree) {
        delete m_octree;
    }
    m_octree = new Octree(group->getBoundingBox());
    for (auto member: group->m_groupMemberList) {
        m_octree->insert(member);
    }
    m_octree->build();
}

BVH::BVH(GeometryGroupObj *group) : m_octree(nullptr) {
    updateBVH(group);
}

void BVH::updateBVH(GeometryGroupObj *group) {
    if (m_octree) {
        delete m_octree;
    }
    m_octree = new Octree(group->getBoundingBox());
    for (auto member: group->m_groupMemberList) {
        m_octree->insert(member);
    }
    m_octree->build();
}

void BVH::flattenBVH(Octree *tree, OctreeNode *node) {
    if (node->m_isLeaf) {
        node->m_startIdx = tree->m_packedIndices.size();
        for(auto mesh: node->m_data) {
            Triangle *triangle = (Triangle *)mesh;
            tree->m_packedIndices.push_back(triangle->m_indices[0]);
            tree->m_packedIndices.push_back(triangle->m_indices[1]);
            tree->m_packedIndices.push_back(triangle->m_indices[2]);
        }
    } else {
        for(int i = 0;i < 8;++i) {
            if(node->m_child[i]) {
                flattenBVH(tree, node->m_child[i]);
            }
        }
    }
}