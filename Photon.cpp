//
// Created by Edge on 2020/10/21.
//

#include <Photon.h>
#include <Dielectric.h>
#include "HittableList.h"

using namespace std;


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