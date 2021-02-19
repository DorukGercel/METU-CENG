#ifndef DIFUSESHADING_H
#define DIFUSESHADING_H

#include <math.h>
#include "../parser.h"
#include "./vectorRayTracer.h"
#include "./triangleRayTracer.h"
#include "./sphereRayTracer.h"
#include "./commonDefinitions.h"

#include <mutex>

using namespace parser;

Vec3f* diffuseForTriangle(Vec3f diffuseConstant, float distanceRSquare, Vec3f pointLightIntensity, Vec3f vectorWi, Vec3f* vertices, Vec3f*& normalVector, std::mutex* normalVectorMutex);

Vec3f* diffuseForSphere(Vec3f diffuseConstant, float distanceRSquare, Vec3f pointX, Vec3f pointLightIntensity, Vec3f vectorWi,Vec3f* center, float radius);

#endif