//
// Created by Edge on 2020/10/14.
//

#include "Utility.h"

namespace Util {
    float schlickApprox(float cosine, float referenceIndex) {
        float r0 = (1 - referenceIndex) / (1 + referenceIndex);
        r0 *= r0;
        return r0 + (1 - r0) * pow((1.0 - cosine), 5);
    }
    float randomInUnit() {
        return rand() / (RAND_MAX+1.0f);
    }

    Velocity randomSphere() {
        Velocity result = {2.0f * randomInUnit() - 1.0f, 2.0f * randomInUnit() - 1.0f, 2.0f * randomInUnit() - 1.0f};
        result.normalize();
        return result;
    }

    EmitType russianRoulette(float reflectivity, float refractivity) {
        float randomValue = Util::randomInUnit();
        if (randomValue < reflectivity) {
            return EmitType::REFLECTED;
        } else if (randomValue < reflectivity + refractivity) {
            return EmitType::TRANSMITTED;
        } else {
            return EmitType::ABSORBED;
        }
    }

};