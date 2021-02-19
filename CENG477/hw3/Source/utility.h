#ifndef __utility_h__
#define __utility_h__
#include "parser.h"
#include <math.h>

using namespace parser;

static Vec3f operator / (const Vec3f& vec, float scalar){
    return Vec3f(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

static float length(const Vec3f& vec){
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static Vec3f cross(const Vec3f& vec1, const Vec3f& vec2){
    return Vec3f(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}

static Vec3f normalize(const Vec3f& vec){
    return vec / length(vec);
}

Vec3f normalVectorCalculator(Vec3f vertex0, Vec3f vertex1, Vec3f vertex2);

#endif