#include "libs.h"

#include "SRbTree.h"

#ifndef FORMS_H
#define FORMS_H

enum forms {CIRCLE, RECTANGLE, LINE, TEXT};

#define CIRC_SVG "<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" fill-opacity=\"75%%\"/>\n"

#define RECT_SVG "<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" fill-opacity=\"75%%\"/>\n"

#define LINE_SVG "<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"1\"/>\n"

#define TEXT_SVG "<text id=\"%d\" x=\"%lf\" y=\"%lf\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" text-anchor=\"%s\"><![CDATA[%s]]></text>\n"

void* createForm();

char* defineAnchor(char a);

void defineLineAnchor(double* xref, double* yref, double x1, double y1, double x2, double y2);

char* formTypeToString(int type);

/*>>>>>>>>>>All sets<<<<<<<<<<*/

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

void setFormCorb(void* formtoset, char* corb);

void setFormCorp(void* formtoset, char* corp);

/*>>>>>>>>>>All gets<<<<<<<<<<*/

int getFormType(void* formtoget);

int getFormId(void* formtoget);

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

bool formFullInside(Info i, double x1, double y1 , double x2, double y2);

bool isAnchorAndMbbEqual(void* orignial, void* tocompare);

#endif