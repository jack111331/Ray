//
// Created by Edge on 2020/10/21.
//

#include <Photon.h>
#include <Dielectric.h>
#include "HittableList.h"

using namespace std;

void KDTree::insert(KDTreeNode *node, const Photon &photon, int depth) {
    int dim = depth % DIMENSION;
    if (node->m_isLeaf) {
        if (photon.m_origin[dim] < node->m_data.m_origin[dim]) {
            node->m_left = new KDTreeNode;
            node->m_left->m_data = photon;
        } else {
            node->m_right = new KDTreeNode;
            node->m_right->m_data = photon;
        }
        node->m_isLeaf = false;
    } else {
        if (photon.m_origin[dim] < node->m_data.m_origin[dim]) {
            if (!node->m_left) {
                node->m_left = new KDTreeNode;
                node->m_left->m_data = photon;
            } else {
                insert(node->m_left, photon, depth + 1);
            }
        } else {
            if (!node->m_right) {
                node->m_right = new KDTreeNode;
                node->m_right->m_data = photon;
            } else {
                insert(node->m_right, photon, depth + 1);
            }
        }
    }
}

void
KDTree::search(KDTreeNode *node, const Coord &min, const Coord &max, int depth, std::vector<Photon *> &photonList) {
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

void KDTree::nearestSearch(KDTreeNode *node, const Coord &point, int k, int depth,
                           std::priority_queue<PhotonElement> &photonHeap) {
    // update distance
    float nodeToPointDistance = (node->m_data.m_origin - point).lengthWithoutSquare();
    if (photonHeap.size() < k || nodeToPointDistance < photonHeap.top().m_distance) {
        PhotonElement pe = {&node->m_data, nodeToPointDistance};
        photonHeap.push(pe);
        if (photonHeap.size() > k) {
            photonHeap.pop();
        }
    }
    int dim = depth % DIMENSION;
    float dx = point[dim] - node->m_data.m_origin[dim];
    KDTreeNode *nearNode = dx < 0 ? node->m_left : node->m_right;
    KDTreeNode *farNode = dx < 0 ? node->m_right : node->m_left;
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

float KDTree::pointToNearestBoundingBoxDistance(const Coord &boundingBoxMin, const Coord &boundingBoxMax,
                                                const Coord &point) {
    float distance = 0.0;
    for (int dim = 0; dim < DIMENSION; ++dim) {
        if (point[dim] < boundingBoxMin[dim] || point[dim] > boundingBoxMax[dim]) {
            float dimDistance = min(fabsf(point[dim] - boundingBoxMin[dim]), fabsf(point[dim] - boundingBoxMax[dim]));
            distance += dimDistance * dimDistance;
        }
    }
    return sqrtf(distance);
}

void KDTree::nearestSearchBF(KDTreeNode *node, const Coord &point, int k, int depth,
                             std::priority_queue<PhotonElement> &photonHeap) {
    // update distance
    float nodeToPointDistance = (node->m_data.m_origin - point).length();
    if (photonHeap.size() < k || nodeToPointDistance < photonHeap.top().m_distance) {
        PhotonElement pe = {&node->m_data, nodeToPointDistance};
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

Color PhotonMappingModel::castRay(const Scene *scene, Ray &ray, int depth) {
    HitRecord record;
    if (scene->m_hittableList->isHit(0.00001, ray, record)) {
        ShadeRecord shadeRecord;
        record.material->calculatePhotonMapping(scene, *this, ray, record, shadeRecord);
        if(depth > 4 || !shadeRecord.isHasAttenuation()) {
            return shadeRecord.emit + shadeRecord.attenuation;
        }
        return shadeRecord.emit + shadeRecord.attenuation * castRay(scene, ray, depth + 1);
    }
    return scene->m_backgroundColor;
}