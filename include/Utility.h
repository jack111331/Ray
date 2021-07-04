//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_UTILITY_H
#define RAY_UTILITY_H

#include <cmath>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <glm/ext/matrix_float4x4.hpp>

class Vec3f {
public:
    float x, y, z;

    Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

    friend std::istream &operator>>(std::istream &is, Vec3f &velocity) {
        is >> velocity.x >> velocity.y >> velocity.z;
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vec3f &velocity) {
        os << "(" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")";
        return os;
    }

    Vec3f &operator+=(const Vec3f &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3f operator+(const Vec3f &rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vec3f &operator-=(const Vec3f &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3f operator-(const Vec3f &rhs) const {
        Vec3f copy(*this);
        copy -= rhs;
        return copy;
    }

    Vec3f operator-() const {
        return {-x, -y, -z};
    }

    float operator[](int dim) const {
        return dim == 0 ? x : dim == 1 ? y : z;
    };

    float &operator[](int dim) {
        return dim == 0 ? x : dim == 1 ? y : z;
    };

    Vec3f &operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vec3f operator*(float rhs) const {
        return {x * rhs, y * rhs, z * rhs};
    }

    Vec3f operator*(const Vec3f &rhs) const {
        return {x * rhs.x, y * rhs.y, z * rhs.z};
    }

    friend Vec3f operator*(const glm::mat3 &lhs, const Vec3f &vec) {
        glm::vec3 tmp(vec.x, vec.y, vec.z);
        tmp = lhs * tmp;
        return {tmp.x, tmp.y, tmp.z};
    }

    friend Vec3f operator*(const glm::mat4 &lhs, const Vec3f &vec) {
        glm::vec4 tmp(vec.x, vec.y, vec.z, 1.0f);
        tmp = lhs * tmp;
        return {tmp.x, tmp.y, tmp.z};
    }

    Vec3f operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    Vec3f operator/(const Vec3f &rhs) const {
        return {x / rhs.x, y / rhs.y, z / rhs.z};
    }

    Vec3f operator/(float rhs) const {
        return {x / rhs, y / rhs, z / rhs};
    }

    bool operator>=(const Vec3f &rhs) const {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    bool operator<=(const Vec3f &rhs) const {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    float lengthWithoutSquare() const {
        return x * x + y * y + z * z;
    }

    float dot(const Vec3f &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Vec3f cross(const Vec3f &rhs) const {
        return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
    }

    Vec3f reflect(const Vec3f &normal) const {
        Vec3f reflectedVelocity(*this);
        float dot = reflectedVelocity.dot(normal);
        if (dot > 0.0) {
            return reflectedVelocity + 2.0 * dot * normal;
        }
        return reflectedVelocity - 2.0 * dot * normal;
    }

    bool refract(const Vec3f &normal, float niOverNt, Vec3f &refracted) const {
        Vec3f unitVector = (*this);
        unitVector = unitVector.normalize();
        float dt = unitVector.dot(normal);
        float discriminant = 1.0 - niOverNt * niOverNt * (1.0 - dt * dt);
        if (discriminant > 0.0) {
            refracted = niOverNt * (unitVector - normal * dt) - normal * sqrt(discriminant);
            return true;
        } else {
            return false;
        }
    }

    Vec3f normalize() const {
        Vec3f normalizedVelocity(*this);
        return normalizedVelocity.normalize();
    }

    Vec3f &normalize() {
        float velocityLength = length();
        x /= velocityLength;
        y /= velocityLength;
        z /= velocityLength;
        return *this;
    }

    friend Vec3f operator*(float lhs, const Vec3f &rhs) {
        return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
    }

    bool nearZero() const {
        return x <= 1e-6 && y <= 1e-6 && z <= 1e-6;
    }

    static Vec3f toVec3f(const std::vector<float> &floatList) {
        if (floatList.size() == 3) {
            return {floatList[0], floatList[1], floatList[2]};
        } else {
            // TODO error handler
            return Vec3f(0, 0, 0);
        }
    }

    void clamp(float minVal, float maxVal) {
        x = x > maxVal ? maxVal : x < minVal ? minVal : x;
        y = y > maxVal ? maxVal : y < minVal ? minVal : y;
        z = z > maxVal ? maxVal : z < minVal ? minVal : z;
    }

};

class Vec2f {
public:
    float x, y;

    friend Vec2f operator*(float lhs, const Vec2f &rhs) {
        return {lhs * rhs.x, lhs * rhs.y};
    }

    Vec2f operator-(const Vec2f &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Vec2f operator+(const Vec2f &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }
};

class Vec2i {
public:
    int x, y;
};

struct Vec3i {
    Vec3i(): x(0), y(0), z(0) {}
    Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
    int x, y, z;
};

struct Vec4f {
    Vec4f(): x(0), y(0), z(0), w(0) {}
    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4f(const Vec3f &v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    Vec4f &min(const Vec4f &rhs) {
        x = std::min(x, rhs.x);
        y = std::min(y, rhs.y);
        z = std::min(z, rhs.z);
        w = std::min(w, rhs.w);
        return *this;
    }
    Vec4f &max(const Vec4f &rhs) {
        x = std::max(x, rhs.x);
        y = std::max(y, rhs.y);
        z = std::max(z, rhs.z);
        w = std::max(w, rhs.w);
        return *this;
    }
    float x, y, z, w;
};

struct Vec4i {
    Vec4i(): x(0), y(0), z(0), w(0) {}
    Vec4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
    int x, y, z, w;
};

class Material;

class IntersectionRecord {
public:
    float t = -1.0f;
    Vec3f point;
    Vec3f normal;
    Vec3f ffNormal;
    Material *material;
    Vec2f texCoord;
};

class LightRecord {
public:
    void addShadedLight(bool isShadedLight) {
        isShadedLightList.push_back(isShadedLight);
    }

    std::vector<bool> isShadedLightList;
};

class ShadeRecord {
public:
    Vec3f emit = {.0f, .0f, .0f};
    Vec3f attenuation = {.0f, .0f, .0f};

    bool isHasAttenuation() const {
        return !attenuation.nearZero();
    }
};

enum EmitType {
    ABSORBED, REFLECTED, TRANSMITTED
};

namespace Util {
    float schlickApprox(float cosine, float referenceIndex);

    float randomInUnit();

    Vec3f randomInHemisphere();

    Vec3f randomCosineDirection();

    float randomIn(float min, float max);

    Vec3f randomSphere();

    EmitType russianRoulette(float reflectivity, float refractivity);

    inline Vec3f randomInUnitSphere() {
        while (true) {
            Vec3f p(randomIn(-1.0, 1.0f), randomIn(-1.0, 1.0f), randomIn(-1.0, 1.0f));
            if (p.lengthWithoutSquare() >= 1) continue;
            return p;
        }
    }

    inline Vec3f randomUnitVector() {
        return randomInUnitSphere().normalize();
    }

};


#endif //RAY_UTILITY_H
