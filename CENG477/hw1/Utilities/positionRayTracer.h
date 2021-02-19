#ifndef POSITIONRAYTRACER_H
#define POSITIONRAYTRACER_H

#include <math.h>
#include <iostream>
#include "../parser.h"
#include "./commonDefinitions.h"

float pixelLengthCalculator(float firstCoordinate, float secondCoordinate, int noPixels);

void positionPrint(parser::Vec3f position);

#endif
