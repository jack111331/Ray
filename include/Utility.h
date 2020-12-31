//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_UTILITY_H
#define RAY_UTILITY_H

#include <cmath>
#include <iostream>
#include <vector>
#include <stdlib.h>

class Velocity {
public:
    float x, y, z;

    Velocity() : x(0.0f), y(0.0f), z(0.0f) {}
    Velocity(float x, float y, float z) : x(x), y(y), z(z) {}

    friend std::istream &operator>>(std::istream &is, Velocity &velocity) {
        is >> velocity.x >> velocity.y >> velocity.z;
    }

    friend std::ostream &operator<<(std::ostream &os, const Velocity &velocity) {
        os << "(" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")";
    }


    Velocity &operator-=(const Velocity &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Velocity operator-() const {
        return {-x, -y, -z};
    }

    Velocity operator-(const Velocity &rhs) const {
        Velocity copy(*this);
        copy -= rhs;
        return copy;
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    float lengthWithoutSquare() const {
        return x * x + y * y + z * z;
    }

    float dot(const Velocity &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Velocity cross(const Velocity &rhs) const {
        return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
    }

    Velocity reflect(const Velocity &normal) const {
        Velocity reflectedVelocity(*this);
        float dot = reflectedVelocity.dot(normal);
        if (dot > 0.0) {
            return reflectedVelocity + 2.0 * dot * normal;
        }
        return reflectedVelocity - 2.0 * dot * normal;
    }

    bool refract(const Velocity &normal, float niOverNt, Velocity &refracted) const {
        Velocity unitVector = (*this);
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

    Velocity normalize() const {
        Velocity normalizedVelocity(*this);
        return normalizedVelocity.normalize();
    }

    Velocity &normalize() {
        float velocityLength = length();
        x /= velocityLength;
        y /= velocityLength;
        z /= velocityLength;
        return *this;
    }

    Velocity operator+(const Velocity &rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Velocity operator*(float rhs) const {
        return {rhs * x, rhs * y, rhs * z};
    }

    Velocity &operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    friend Velocity operator*(float lhs, const Velocity &rhs) {
        return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
    }

    static Velocity toVelocity(const std::vector<float> &floatList) {
        if (floatList.size() == 3) {
            return {floatList[0], floatList[1], floatList[2]};
        } else {
            // TODO error handler
            return Velocity(0, 0, 0);
        }
    }

};

class Coord {
public:
    float x, y, z;

    Coord() : x(0.0f), y(0.0f), z(0.0f) {}
    Coord(float x, float y, float z) : x(x), y(y), z(z) {}

    friend std::istream &operator>>(std::istream &is, Coord &coord) {
        is >> coord.x >> coord.y >> coord.z;
    }

    friend std::ostream &operator<<(std::ostream &os, const Coord &coord) {
        os << "(" << coord.x << ", " << coord.y << ", " << coord.z << ")";
    }

    float operator[](int dim) const {
        return dim == 0 ? x : dim == 1 ? y : z;
    };

    float &operator[](int dim) {
        return dim == 0 ? x : dim == 1 ? y : z;
    };

    Coord operator-() const {
        return {-x, -y, -z};
    }

    Velocity operator-(const Coord &rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Coord operator+(const Coord &rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Coord operator*(const float rhs) const {
        return {x * rhs, y * rhs, z * rhs};
    }

    Coord operator/(const Coord &rhs) const {
        return {x / rhs.x, y / rhs.y, z / rhs.z};
    }

    Coord operator/(float rhs) const {
        return {x / rhs, y / rhs, z / rhs};
    }

    Coord operator+(const Velocity &rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Coord operator-(const Velocity &rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    bool operator>=(const Coord &rhs) const {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    bool operator<=(const Coord &rhs) const {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }

    static Coord toCoord(const std::vector<float> &floatList) {
        if (floatList.size() == 3) {
            return {floatList[0], floatList[1], floatList[2]};
        } else {
            // TODO error handler
            return Coord(0.0f, 0.0f, 0.0f);
        }
    }

};

class Coord2D {
public:
    float x, y;

    friend Coord2D operator*(float lhs, const Coord2D &rhs) {
        return {lhs * rhs.x, lhs * rhs.y};
    }

    Coord2D operator-(const Coord2D &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Coord2D operator+(const Coord2D &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }
};

class Coord2Di {
public:
    int x, y;
};


class Color {
public:
    float r, g, b;

    friend std::istream &operator>>(std::istream &is, Color &color) {
        is >> color.r >> color.g >> color.b;
    }

    friend std::ostream &operator<<(std::ostream &os, const Color &color) {
        os << (uint8_t) (color.r > 1.0 ? 255 : (255.99 * color.r))
           << (uint8_t) (color.g > 1.0 ? 255 : (255.99 * color.g))
           << (uint8_t) (color.b > 1.0 ? 255 : (255.99 * color.b));
        return os;
    }

    void clamp() {
        r = r > 1.0f ? 1.0f : r;
        g = g > 1.0f ? 1.0f : g;
        b = b > 1.0f ? 1.0f : b;
    }

    friend Color operator*(float lhs, const Color &rhs) {
        return {lhs * rhs.r, lhs * rhs.g, lhs * rhs.b};
    }

    friend Color operator*(const Color &lhs, const Color &rhs) {
        return {lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b};
    }

    Color &operator+=(const Color &rhs) {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    Color &operator/=(float divider) {
        r /= divider;
        g /= divider;
        b /= divider;
        return *this;
    }

    Color operator/(float divider) {
        Color result(*this);
        result.r /= divider;
        result.g /= divider;
        result.b /= divider;
        return result;
    }

    Color operator+(const Color &rhs) const {
        return {r + rhs.r, g + rhs.g, b + rhs.b};
    }

    static Color toColor(const std::vector<float> &floatList) {
        if (floatList.size() == 3) {
            return {floatList[0], floatList[1], floatList[2]};
        } else {
            // TODO error handler
            return {};
        }
    }

};

struct Vec3i {
    int x, y, z;
};

class Material;

class HitRecord {
public:
    float t = -1.0f;
    Coord point;
    Velocity normal;
    Material *material;
    Coord2D texCoord;
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
    Color emit = {.0f, .0f, .0f};
    Color attenuation = {.0f, .0f, .0f};

    bool isHasAttenuation() const {
        return attenuation.r >= 1e-6 && attenuation.g >= 1e-6 && attenuation.b >= 1e-6;
    }
};

enum EmitType {
    ABSORBED, REFLECTED, TRANSMITTED
};

namespace Util {
    float schlickApprox(float cosine, float referenceIndex);

    float randomInUnit();

    Velocity randomSphere();

    EmitType russianRoulette(float reflectivity, float refractivity);

};


#endif //RAY_UTILITY_H
