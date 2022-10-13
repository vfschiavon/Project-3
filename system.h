#include "libs.h"

#include "SRbTree.h"

#ifndef SYSTEM_H
#define SYSTEM_H

void readParam(int argc, char** argv, void* paths);

void readPrintGeo(SRbTree tree, void* paths);

void readQry(SRbTree tree, void* paths);

#endif