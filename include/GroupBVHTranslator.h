//
// Created by Edge on 2021/1/9.
//

#ifndef RAY_GROUPBVHTRANSLATOR_H
#define RAY_GROUPBVHTRANSLATOR_H


#include "GroupObj.h"

class GroupBVHTranslator {
    // TODO flatten Group
    // flatten transform to texture
    // flatten material to texture
public:
    GroupBVHTranslator(GroupObj *group);
private:
    GroupObj * m_group;
    std::set<Hittable *> m_meshPool;
};


#endif //RAY_GROUPBVHTRANSLATOR_H
