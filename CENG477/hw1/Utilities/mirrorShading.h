#ifndef MIRRORSHADING_H
#define MIRRORSHADING_H

#include <math.h>
#include "../parser.h"
#include "./vectorRayTracer.h"
#include "./triangleRayTracer.h"
#include "./sphereRayTracer.h"
#include "./commonDefinitions.h"

using namespace parser;

Ray mirrorRayCalculator(Vec3f* pointX ,Vec3f* vectorWo, Vec3f* normalVector);

#endif