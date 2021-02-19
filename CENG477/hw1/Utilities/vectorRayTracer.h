#ifndef VECTORRAYTRACER_H
#define VECTORRAYTRACER_H

#include <math.h>
#include <iostream>
#include "../parser.h"
#include "./commonDefinitions.h"

using namespace parser;

struct Ray{
    Vec3f originPosition;
    Vec3f direction;
};

Ray rayCalculator(Vec3f originPosition, Vec3f otherPosition);

Vec3f addition(Vec3f v1, Vec3f v2);

Vec3f subtraction(Vec3f v1, Vec3f v2);

Vec3f scalerProduct(Vec3f v, float c);

Vec3f vector_to_vector_Product(Vec3f v1, Vec3f v2);

float dotProduct(Vec3f v1, Vec3f v2);

Vec3f crossProduct(Vec3f v1, Vec3f v2);

float cosineCalculator(Vec3f v1, Vec3f v2);

float vectorLengthCalculator(Vec3f v);

Vec3f vectorNormalizer(Vec3f v);

Vec3f halfVectorCalculator(Vec3f v1, Vec3f v2);


Ray camera_to_plane_VectorCalculator(Vec3f cameraPosition, Vec3f pixelPosition);

Ray x_to_camera_VectorCalculator(Vec3f xPosition, Vec3f cameraPosition);

Ray x_to_lightSource_VectorCalculator(Vec3f xPosition, Vec3f lightSourcePosition);


void vectorPrint(Vec3f v);

#endif