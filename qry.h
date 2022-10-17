#include "libs.h"

#include "SRbTree.h"

#ifndef QRY_H
#define QRY_H

void e(SRbTree tree, FILE* qry, FILE* qrytxt);

void mv(SRbTree tree, FILE* qry, FILE* qrytxt);

double lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

void mc(SRbTree tree, FILE* qry, FILE* qrytxt);

#endif
