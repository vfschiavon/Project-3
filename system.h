#include "libs.h"

#include "SRbTree.h"

#ifndef SYSTEM_H
#define SYSTEM_H

// Read all parameters passed by argv and stores it on a path struct
void readParam(int argc, char** argv, void* paths);

// Read the geo file, print all forms in the first svg and stores all the forms in a tree
void readPrintGeo(SRbTree tree, void* paths);

// Read the qry file, execute it's functions and print all results in the final svg and txt
void readQry(SRbTree tree, void* paths);

#endif