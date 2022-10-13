#include "libs.h"

#ifndef PATHS_H
#define PATHS_H

void* createAllPaths();

void freeAllPaths(void* aptofree);

char* prepareDir(char* dir);

void prepareDoc(char* asread, char* onlyname);

/*>>>>>>>>>>All sets<<<<<<<<<<*/

void setBed(void* aptoset, char* bed);

void setBsd(void* aptoset, char* bsd);

void setBedGeo(void* aptoset, char* bedgeo);

void setGeoArq(void* aptoset, char* geoarq);

void setGeoName(void* aptoset, char* geoname);

void setBedQry(void* aptoset, char* bedqry);

void setQryArq(void* aptoset, char* qryarq);

void setQryName(void* aptoset, char* qryname);

void setBsdGeoSvg(void* aptoset, char* bsdgeosvg);

void setBsdGeoDot(void* aptoset, char* bsdgeodot);

void setBsdGeoQrySvg(void* aptoset, char* bsdgeoqrysvg);

void setBsdGeoQryTxt(void* aptoset, char* bsdgeoqrytxt);

void setBsdGeoQryDot(void* aptoset, char* bsdgeoqrydot);

/*>>>>>>>>>>All gets<<<<<<<<<<*/

char* getBed(void* aptoget);

char* getBsd(void* aptoget);

char* getBedGeo(void* aptoget);

char* getGeoArq(void* aptoget);

char* getGeoName(void* aptoget);

char* getBedQry(void* aptoget);

char* getQryArq(void* aptoget);

char* getQryName(void* aptoget);

char* getBsdGeoSvg(void* aptoget);

char* getBsdGeoDot(void* aptoget);

char* getBsdGeoQrySvg(void* aptoget);

char* getBsdGeoQryTxt(void* aptoget);

char* getBsdGeoQryDot(void* aptoget);

#endif