#include "libs.h"

#include "SRbTree.h"

#ifndef QRY_H
#define QRY_H

void funcE(SRbTree tree, FILE* qry, FILE* qrytxt);

void funcMV(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

void funcLR(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

void funcD(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

void funcMC(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

#endif
