#include "libs.h"

#include "SRbTree.h"

#ifndef QRY_H
#define QRY_H

// Execute all the main operations from funtion e (qry)
// Receives the tree where the forms are stored, the qry file to read the function data and the file of the txt report
void e(SRbTree tree, FILE* qry, FILE* qrytxt);

// Execute all the main operations from funtion mv (qry)
// Receives the tree where the forms are stored, the qry file to read the function data and the file of the txt report
void mv(SRbTree tree, FILE* qry, FILE* qrytxt);

// Execute all the main operations from funtion lr (qry)
// Receives the tree where the forms are stored, the qry file to read the function data, the file of the txt report and the final svg
// Return the total M$ collected
double lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

// Execute all the main operations from funtion d (qry)
// Receives the tree where the forms are stored, the qry file to read the function data , the file of the txt report and the final svg
void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg);

// Execute all the main operations from funtion mc (qry)
// Receives the tree where the forms are stored, the qry file to read the function data and the file of the txt report
void mc(SRbTree tree, FILE* qry, FILE* qrytxt);

#endif
