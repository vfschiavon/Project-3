#include "libs.h"

#include "SRbTree.h"

#ifndef FORMS_H
#define FORMS_H

// This enum is used to identify the type of form in a more readable way
enum forms {CIRCLE, RECTANGLE, LINE, TEXT};

// These defines are used to clearify the print on the .svg file
#define CIRC_SVG "<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" fill-opacity=\"75%%\"/>\n"

#define RECT_SVG "<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" fill-opacity=\"75%%\"/>\n"

#define LINE_SVG "<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"1\"/>\n"

#define TEXT_SVG "<text id=\"%d\" x=\"%lf\" y=\"%lf\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" text-anchor=\"%s\"><![CDATA[%s]]></text>\n"

// Create a struct of the type form and returns it
void* createForm();

// Receives a char anchor (i, m, f) and returns the string that represents it on the svg pattern
char* defineAnchor(char a);

// Receives ddata from a line and define its anchors (by reference)
void defineLineAnchor(double* xref, double* yref, double x1, double y1, double x2, double y2);

// Receives a number indentifier and returns the name of the specified figure
char* formTypeToString(int type);

/*>>>>>>>>>>All sets<<<<<<<<<<*/
/*Receives the necessary data from each form and set on the passed struct*/

void setFullCirc(void* formtoset, int id, double x, double y, double r, char* corb, char* corp);

void setFullRect(void* formtoset, int id, double x, double y, double w, double h, char* corb, char* corp);

void setFullLine(void* formtoset, int id, double x, double y, double x2, double y2, char* corb);

void setFullText(void* formtoset, int id, double x, double y, char* corb, char* a, char* txto);

void setNauEnergy(void* formtoset, double energy);

void setNauBalance(void* formtoset, double balance);

void setFormX(void* formtoset, double x);

void setFormX2(void* formtoset, double x2);

void setFormY(void* formtoset, double y);

void setFormY2(void* formtoset, double y2);

/*>>>>>>>>>>All gets<<<<<<<<<<*/
/*Returns the specified data from the passed struct*/

int getFormType(void* formtoget);

int getFormId(void* formtoget);

double getNauEnergy(void* formtoget);

double getNauBalance(void* formtoget);

double getFormX(void* formtoget);

double getFormX2(void* formtoget);

double getFormY(void* formtoget);

double getFormY2(void* formtoget);

double getFormR(void* formtoget);

double getFormW(void* formtoget);

double getFormH(void* formtoget);

char* getFormCorb(void* formtoget);

char* getFormCorp(void* formtoget);

char* getFormA(void* formtoget);

char* getFormTxto(void* formtoget);

/*>>>>>>>>>>Verifiers<<<<<<<<<<*/

// Verifies if the form passed is full inside of the rectangle passed
bool formFullInside(Info i, double x1, double y1 , double x2, double y2);

// Verifies if the forms passed have the same anchor
bool isAnchorAndMbbEqual(void* orignial, void* tocompare);

#endif