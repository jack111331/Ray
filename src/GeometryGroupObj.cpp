//
// Created by Edge on 2020/7/15.
//

#include <Sphere.h>
#include <TriangleGroup.h>
#include "GeometryGroupObj.h"

using namespace std;

bool ObjectBoundingBox::isHit(const Ray &ray, float &tNear, float &tFar) {
    float tMin = (m_bounding[0].x - ray.origin.x) / ray.velocity.x;
    float tMax = (m_bounding[1].x - ray.origin.x) / ray.velocity.x;

    if (tMin > tMax) swap(tMin, tMax);

    float tyMin = (m_bounding[0].y - ray.origin.y) / ray.velocity.y;
    float tyMax = (m_bounding[1].y - ray.origin.y) / ray.velocity.y;

    if (tyMin > tyMax) swap(tyMin, tyMax);

    // Test if it intersect in 2d
    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (m_bounding[0].z - ray.origin.z) / ray.velocity.z;
    float tzMax = (m_bounding[1].z - ray.origin.z) / ray.velocity.z;

    if (tzMin > tzMax) swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    if (tzMin > tMin)
        tMin = tzMin;

    if (tzMax < tMax)
        tMax = tzMax;

    tNear = std::min(tNear, tMin);
    tFar = std::max(tFar, tMax);

    return true;
}

bool GeometryGroupObj::isHit(const Ray &ray, IntersectionRecord &record, float tmin, const glm::mat4 &transformMat) const {
    if(m_accel) {
        return m_accel->isHit(ray, record, tmin, transformMat);
    }
    bool isHit = false;
    for(auto member: m_groupMemberList) {
        isHit = std::max(isHit, member->isHit(ray, record, tmin, transformMat));
    }
    return isHit;
}

ObjectBoundingBox GeometryGroupObj::getBoundingBox() const {
    return m_boundingBox;
}

bool GeometryGroupObj::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if (!node["geometry"]) {
        std::cerr << "No geometry setting" << std::endl;
        exit(1);
    }
    auto geometryNode = node["geometry"];
    for (uint32_t i = 0; i < geometryNode.size(); ++i) {
        auto meshIt = scene->m_meshTable.find(geometryNode[i]["instance-name"].as<std::string>());
        if(meshIt != scene->m_meshTable.end()) {
            addHittable(meshIt->second);
        } else {
            std::cerr << "Can't load mesh instance " << geometryNode[i]["instance-name"].as<std::string>() << std::endl;
            exit(1);
        }
        std::cout << geometryNode[i]["instance-name"].as<std::string>() << std::endl;
    }
    m_accel = new BVH(this);
    return true;
}

void GeometryGroupObj::addHittable(ObjectNode *mesh) {
    m_groupMemberList.push_back(mesh);
    m_boundingBox.updateBoundingBox(mesh->getBoundingBox());
}