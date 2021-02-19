#ifndef SPECULARSHADING_H
#define SPECULARSHADING_H

#include <math.h>
#include "../parser.h"
#include "./vectorRayTracer.h"
#include "./triangleRayTracer.h"
#include "./sphereRayTracer.h"
#include "./commonDefinitions.h"

using namespace parser;

//Always call after diffuse shading
Vec3f* specularForTriangle(Vec3f specularConstant, float phongExponent, float distanceRSquare , Vec3f vectorWi, Vec3f vectorWo , Vec3f pointLightIntensity ,Vec3f* normalVector);

Vec3f* specularForSphere(Vec3f specularConstant, float phongExponent, float distanceRSquare ,Vec3f pointX, Vec3f vectorWi, Vec3f vectorWo , Vec3f pointLightIntensity, Vec3f* center, float radius);

#endif