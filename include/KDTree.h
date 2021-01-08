//
// Created by Edge on 2020/11/20.
//

#ifndef RAY_KDTREE_H
#define RAY_KDTREE_H

#include <queue>
#include "utility.h"
#include <cmath>

class CoordData {
public:
    CoordData(const Vec3f &origin): m_origin(origin) {}
    Vec3f m_origin;
};

template<typename T>
struct CoordDataElement {
    T *m_photon;
    float m_distance;
    bool operator <(const CoordDataElement &rhs) const {
        return m_distance < rhs.m_distance;
    }
};

template<typename T>
struct KDTreeNode {
    KDTreeNode(const T& data): m_data(data) {}
    T m_data;
    bool m_isLeaf = true;
    KDTreeNode *m_left = nullptr, *m_right = nullptr;
};

template<typename T>
class KDTree {
public:
    void insert(const T &photon) {
        if(!m_root) {
            m_root = new KDTreeNode<T>(photon);
        } else {
            insert(m_root, photon, 0);
        }
    }

    void search(const Vec3f &min, const Vec3f &max, std::vector<T *> &photonList) {
        search(m_root, min, max, 0, photonList);
    }

    void nearestSearch(const Vec3f &point, int k, std::priority_queue<CoordDataElement<T>> &photonHeap) {
        Vec3f min = {-1e9, -1e9, -1e9};
        Vec3f max = {1e9, 1e9, 1e9};
        nearestSearch(m_root, point, k, 0, photonHeap);
    }

    void nearestSearchBF(const Vec3f &point, int k, std::priority_queue<CoordDataElement<T>> &photonHeap) {
        nearestSearchBF(m_root, point, k, 0, photonHeap);
    }

private:
    void insert(KDTreeNode<T> *node, const T &photon, int depth){
        int dim = depth % DIMENSION;
        if (node->m_isLeaf) {
            if (photon.m_origin[dim] < node->m_data.m_origin[dim]) {
                node->m_left = new KDTreeNode<T>(photon);
            } else {
                node->m_right = new KDTreeNode<T>(photon);
            }
            node->m_isLeaf = false;
        } else {
            if (photon.m_origin[dim] < node->m_data.m_origin[dim]) {
                if (!node->m_left) {
                    node->m_left = new KDTreeNode<T>(photon);
                } else {
                    insert(node->m_left, photon, depth + 1);
                }
            } else {
                if (!node->m_right) {
                    node->m_right = new KDTreeNode<T>(photon);
                } else {
                    insert(node->m_right, photon, depth + 1);
                }
            }
        }
    }

    void search(KDTreeNode<T> *node, const Vec3f &min, const Vec3f &max, int depth, std::vector<T *> &photonList){
        if (node->m_data.m_origin >= min && node->m_data.m_origin <= max) {
            photonList.push_back(&node->m_data);
        }
        int dim = depth % DIMENSION;
        if (node->m_left && node->m_data.m_origin[dim] >= min[dim]) {
            search(node->m_left, min, max, depth + 1, photonList);
        }
        if (node->m_right && node->m_data.m_origin[dim] <= max[dim]) {
            search(node->m_right, min, max, depth + 1, photonList);
        }
    }

    void nearestSearch(KDTreeNode<T> *node, const Vec3f &point, int k, int depth, std::priority_queue<CoordDataElement<T>> &photonHeap){
        // update distance
        float nodeToPointDistance = (node->m_data.m_origin - point).lengthWithoutSquare();
        if (photonHeap.size() < k || nodeToPointDistance < photonHeap.top().m_distance) {
            CoordDataElement<T> pe = {&node->m_data, nodeToPointDistance};
            photonHeap.push(pe);
            if (photonHeap.size() > k) {
                photonHeap.pop();
            }
        }
        int dim = depth % DIMENSION;
        float dx = point[dim] - node->m_data.m_origin[dim];
        KDTreeNode<T> *nearNode = dx < 0 ? node->m_left : node->m_right;
        KDTreeNode<T> *farNode = dx < 0 ? node->m_right : node->m_left;
        if (nearNode) {
            nearestSearch(nearNode, point, k, depth + 1,
                          photonHeap);
        }
        if (dx * dx >= photonHeap.top().m_distance) {
            return;
        }
        if (farNode) {
            nearestSearch(farNode, point, k, depth + 1,
                          photonHeap);
        }
    }

    void nearestSearchBF(KDTreeNode<T> *node, const Vec3f &point, int k, int depth, std::priority_queue<CoordDataElement<T>> &photonHeap) {
        // update distance
        float nodeToPointDistance = (node->m_data.m_origin - point).length();
        if (photonHeap.size() < k || nodeToPointDistance < photonHeap.top().m_distance) {
            CoordDataElement<T> pe = {&node->m_data, nodeToPointDistance};
            photonHeap.push(pe);
            while (!photonHeap.empty() && photonHeap.size() > k) {
                photonHeap.pop();
            }
        }
        if (node->m_left) {
            nearestSearchBF(node->m_left, point, k, depth + 1,
                            photonHeap);
        }
        if (node->m_right) {
            nearestSearchBF(node->m_right, point, k, depth + 1,
                            photonHeap);
        }
    }

    float pointToNearestBoundingBoxDistance(const Vec3f &boundingBoxMin, const Vec3f &boundingBoxMax, const Vec3f &point) {
        float distance = 0.0;
        for (int dim = 0; dim < DIMENSION; ++dim) {
            if (point[dim] < boundingBoxMin[dim] || point[dim] > boundingBoxMax[dim]) {
                float dimDistance = std::min(fabsf(point[dim] - boundingBoxMin[dim]), fabsf(point[dim] - boundingBoxMax[dim]));
                distance += dimDistance * dimDistance;
            }
        }
        return sqrtf(distance);
    }

    KDTreeNode<T> *m_root = nullptr;

    static const int DIMENSION = 3;
};


#endif //RAY_KDTREE_H
