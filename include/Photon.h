//
// Created by Edge on 2020/10/21.
//

#ifndef RAY_PHOTON_H
#define RAY_PHOTON_H

#include <queue>
#include "Utility.h"
#include "IllumModel.h"

class Photon {
public:
    enum Flag {
        NORMAL, CAUSTIC
    };
    Coord m_origin;
    float m_power;
    Color m_color;
    Velocity m_incident;
    short m_flag;
};

struct PhotonElement {
    Photon *m_photon;
    float m_distance;
    bool operator <(const PhotonElement &rhs) const {
        return m_distance < rhs.m_distance;
    }
//    bool operator ()(const PhotonElement &lhs, const PhotonElement &rhs) const {
//        return lhs.m_distance < rhs.m_distance;
//    }
};

struct KDTreeNode {
    Photon m_data;
    bool m_isLeaf = true;
    KDTreeNode *m_left = nullptr, *m_right = nullptr;
};

class KDTree {
public:
    void insert(const Photon &photon) {
        if(!m_root) {
            m_root = new KDTreeNode;
            m_root->m_data = photon;
        } else {
            insert(m_root, photon, 0);
        }
    }

    void search(const Coord &min, const Coord &max, std::vector<Photon *> &photonList) {
        search(m_root, min, max, 0, photonList);
    }

    void nearestSearch(const Coord &point, int k, std::priority_queue<PhotonElement> &photonHeap) {
        Coord min = {-1e9, -1e9, -1e9};
        Coord max = {1e9, 1e9, 1e9};
        nearestSearch(m_root, point, k, 0, photonHeap);
    }

    void nearestSearchBF(const Coord &point, int k, std::priority_queue<PhotonElement> &photonHeap) {
        nearestSearchBF(m_root, point, k, 0, photonHeap);
    }

private:
    void insert(KDTreeNode *node, const Photon &photon, int depth);

    void search(KDTreeNode *node, const Coord &min, const Coord &max, int depth, std::vector<Photon *> &photonList);

    void nearestSearch(KDTreeNode *node, const Coord &point, int k, int depth, std::priority_queue<PhotonElement> &photonHeap);

    void nearestSearchBF(KDTreeNode *node, const Coord &point, int k, int depth, std::priority_queue<PhotonElement> &photonHeap);

    float pointToNearestBoundingBoxDistance(const Coord &boundingBoxMin, const Coord &boundingBoxMax, const Coord &point);

    KDTreeNode *m_root = nullptr;

    static const int DIMENSION = 3;
};

class HittableList;

class PhotonMappingModel : public IlluminationModel {
public:
    PhotonMappingModel() {
        m_kdTree = new KDTree;
    }
    virtual Color castRay(const Scene *scene, Ray &ray, double intensity, Color &color, int depth);

    KDTree *m_kdTree;
    // FIXME
    float m_radius = 2.0;
};

#endif //RAY_PHOTON_H
