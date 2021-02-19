#ifndef TRIANGLERAYTRACER_H
#define TRIANGLERAYTRACER_H

#include <math.h>
#include "mathHelper.h"
#include "vectorRayTracer.h"
#include "../parser.h"
#include "./commonDefinitions.h"

#include <mutex>

using namespace parser;

float isPointInTriangle(Vec3f *vertices, Ray ray, Vec3f* &triangularMatrix, Vec3f* &cameraSolution,std::mutex* triangleMatrixMutex, bool isFromCamera);

//float t_FinderForTriangle(float** A, float* solution);

Vec3f triangleNormalVectorCalculator(Vec3f *vertices);

#endif