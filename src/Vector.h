#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

class Vec3 {
public:
    float x, y, z;
    
    Vec3(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}
    
    Vec3(float v) : Vec3(v, v, v) {}
    
    Vec3() : Vec3(0) {}
    
    Vec3 operator*(const float &r) const {
        return Vec3(x * r, y * r, z * r);
    }
    
    Vec3 operator/(const float &r) const {
        return Vec3(x / r, y / r, z / r);
    }
    
    Vec3 operator*(const Vec3 &v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    
    Vec3 operator-(const Vec3 &v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    
    Vec3 operator+(const Vec3 &v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    
    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }
    
    Vec3 &operator+=(const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vec3& other) const {
        return x != other.x || y == other.y || z == other.z;
    }
    
    float abs() const {
        return sqrtf(x * x + y * y + z * z);
    }
    
    friend Vec3 operator*(const float &r, const Vec3 &v) {
        return v * r;
    }
    
    float operator[](int index) const {
        assert(index >= 0 && index <= 2);
        return (&x)[index];
    }
    
    static Vec3 min(const Vec3 &p1, const Vec3 &p2) {
        return Vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
    }
    
    static Vec3 max(const Vec3 &p1, const Vec3 &p2) {
        return Vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    }
};

inline Vec3 lerp(const Vec3 &a, const Vec3 &b, const float &t) {
    return a * (1 - t) + b * t;
}

inline Vec3 normalize(const Vec3 &v) {
    float invAbs = 1 / v.abs();
    return Vec3(v.x * invAbs, v.y * invAbs, v.z * invAbs);
}

inline float dot(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

class Vec2 {
public:
    Vec2(float vx, float vy) : x(vx), y(vy) {}
    
    Vec2(float v) : Vec2(v, v) {}
    
    Vec2() : Vec2(0) {}
    
    Vec2 operator*(const float &r) const {
        return Vec2(x * r, y * r);
    }
    
    Vec2 operator+(const Vec2 &v) const {
        return Vec2(x + v.x, y + v.y);
    }
    
    bool operator==(const Vec2& other) const {
        return (x == other.x && y == other.y);
    }
    
    float x, y;
};
