#ifndef AMBIENTSHADING_H
#define AMBIENTSHADING_H

#include "../parser.h"
#include "./vectorRayTracer.h"
#include "./commonDefinitions.h"

using namespace parser;

Vec3f* ambient(Vec3f ambientConstant, Vec3f ambientLight);

#endif