#pragma once

#include <cmath>
#include <iostream>
#include <random>


const float EPSILON = 0.0001;

inline float deg2rad(const float& deg) {
    return deg * M_PI / 180.0;
}

inline float clamp(const float &lo, const float &hi, const float &v) {
    return std::max(lo, std::min(hi, v));
}

inline bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1)
        std::swap(x0, x1);
    return true;
}

inline float rand_n1_1() {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1, 1);
    return dis(e);
}

inline void updateProgress(float progress) {
    int barWidth = 50;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        std::cout << (i <= pos ? '=' : ' ');
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};
