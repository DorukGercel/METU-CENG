#ifndef SPHERERAYTRACER_H
#define SPHERERAYTRACER_H

#include <math.h>
#include "../parser.h"
#include "./vectorRayTracer.h"
#include "./commonDefinitions.h"

using namespace parser;

float t_FinderForSphere(Vec3f center, float radius, Ray ray);

Vec3f sphereNormalVectorCalculator(Vec3f* center, float radius, Vec3f* intersectPoint);

#endif
