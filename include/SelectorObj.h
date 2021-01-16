//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_SELECTOROBJ_H
#define RAY_SELECTOROBJ_H


#include "ObjectNode.h"
#include "TLASNode.h"

class SelectorObj : public TLASNode {
public:
    SelectorObj() : TLASNode(), m_selectMemberEnableList() {}

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin = 0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const ;

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const {
        ObjectBoundingBox boundingBox;
        for (uint32_t memberId = 0; memberId < m_groupMemberList.size();++memberId) {
            if(m_selectMemberEnableList[memberId]) {
                boundingBox.updateBoundingBox(m_groupMemberList[memberId]->getBoundingBox());
            }
        }
        return boundingBox;
    }

    virtual TLASNodeType getTlasNodeType() {
        return TLASNodeType::SELECTOR;
    }

private:
    std::vector<bool> m_selectMemberEnableList;
    // TODO we need cache.....
};


#endif //RAY_SELECTOROBJ_H
