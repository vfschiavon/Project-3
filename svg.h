#include "libs.h"
#include "SRbTree.h"

#ifndef SVG_H
#define SVG_H

// Receives the data from a node and some auxiliar data to print all the results in the svg file, based on each form separately
void printSvg(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux);

#endif
