//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_SELECTOROBJ_H
#define RAY_SELECTOROBJ_H


#include "Hittable.h"

class SelectorObj : public Hittable {
public:
    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const ;

    // For local shading
    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList) {
        for (auto member: m_selectMemberList) {
            member->createVAO(shadeObjectList);
        }
    };

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const {
        ObjectBoundingBox boundingBox;
        for (uint32_t memberId = 0; memberId < m_selectMemberList.size();++memberId) {
            if(m_selectMemberEnableList[memberId]) {
                boundingBox.updateBoundingBox(m_selectMemberList[memberId]->getBoundingBox());
            }
        }
        return boundingBox;
    }
private:
    std::vector<Hittable *> m_selectMemberList;
    std::vector<bool> m_selectMemberEnableList;
    // TODO we need cache.....
};


#endif //RAY_SELECTOROBJ_H
