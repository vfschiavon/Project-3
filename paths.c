#include "paths.h"

typedef struct all_paths
{
    char* bed; // Input directory
    char* bsd; // Output directory
    char* bedgeo; // bed/arq.geo
    char* geoarq; // arq.geo
    char* geoname; // arq (geo)
    char* bedqry; // bed/arq.qry
    char* qryarq; // arq.qry
    char* qryname; // arq (qry)
    char* bsdgeosvg; // bsd/nomegeo.svg
    char* bsdgeodot; // bsd/nomegeo.dot ----------
    char* bsdgeoqrysvg; // bsd/nomegeo_qry.svg
    char* bsdgeoqrytxt; // bsd/nomegeo_qry.txt
    char* bsdgeoqrydot; // bsd/nomegeo_qry.dot ------------
} all_paths;

void* createAllPaths()
{
    all_paths* ap = calloc(1, sizeof(all_paths));
    return ap;
}

void freeAllPaths(void* aptofree)
{
    all_paths* ap = aptofree;
    free(ap->bed);
    free(ap->bsd);
    free(ap->bedgeo);
    free(ap->geoarq);
    free(ap->geoname);
    free(ap->bedqry);
    free(ap->qryarq);
    free(ap->qryname);
    free(ap->bsdgeosvg);
    free(ap->bsdgeodot);
    free(ap->bsdgeoqrysvg);
    free(ap->bsdgeoqrytxt);
    free(ap->bsdgeoqrydot);
    free(ap);
}

char* prepareDir(char* dir)
{
    if (dir[strlen(dir) - 1] != '/')
    {
        dir = realloc(dir, strlen(dir) + 2);
        strcat(dir, "/");
    }
    return dir;
}

void prepareDoc(char* asread, char* onlyname)
{
    char* bar = strrchr(asread, '/');

    if (bar)
    {
        bar++;
        strcpy(onlyname, bar);
        strtok(onlyname, ".");
    }
    else
    {
        strcpy(onlyname, asread);
        strtok(onlyname, ".");
    }
}

/*>>>>>>>>>>All sets<<<<<<<<<<*/

void setBed(void* aptoset, char* bed)
{
    all_paths* ap = aptoset;
    ap->bed = bed;
}

void setBsd(void* aptoset, char* bsd)
{
    all_paths* ap = aptoset;
    ap->bsd = bsd;
}

void setBedGeo(void* aptoset, char* bedgeo)
{
    all_paths* ap = aptoset;
    ap->bedgeo = bedgeo;
}

void setGeoArq(void* aptoset, char* geoarq)
{
    all_paths* ap = aptoset;
    ap->geoarq = geoarq;
}

void setGeoName(void* aptoset, char* geoname)
{
    all_paths* ap = aptoset;
    ap->geoname = geoname;
}

void setBedQry(void* aptoset, char* bedqry)
{
    all_paths* ap = aptoset;
    ap->bedqry = bedqry;
}

void setQryArq(void* aptoset, char* qryarq)
{
    all_paths* ap = aptoset;
    ap->qryarq = qryarq;
}

void setQryName(void* aptoset, char* qryname)
{
    all_paths* ap = aptoset;
    ap->qryname = qryname;
}

void setBsdGeoSvg(void* aptoset, char* bsdgeosvg)
{
    all_paths* ap = aptoset;
    ap->bsdgeosvg = bsdgeosvg;
}

void setBsdGeoDot(void* aptoset, char* bsdgeodot)
{
    all_paths* ap = aptoset;
    ap->bsdgeodot = bsdgeodot;
}

void setBsdGeoQrySvg(void* aptoset, char* bsdgeoqrysvg)
{
    all_paths* ap = aptoset;
    ap->bsdgeoqrysvg = bsdgeoqrysvg;
}

void setBsdGeoQryTxt(void* aptoset, char* bsdgeoqrytxt)
{
    all_paths* ap = aptoset;
    ap->bsdgeoqrytxt = bsdgeoqrytxt;
}

void setBsdGeoQryDot(void* aptoset, char* bsdgeoqrydot)
{
    all_paths* ap = aptoset;
    ap->bsdgeoqrydot = bsdgeoqrydot;
}

/*>>>>>>>>>>All gets<<<<<<<<<<*/

char* getBed(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bed;
}

char* getBsd(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bsd;
}

char* getBedGeo(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bedgeo;
}

char* getGeoArq(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->geoarq;
}

char* getGeoName(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->geoname;
}

char* getBedQry(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bedqry;
}

char* getQryArq(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->qryarq;
}

char* getQryName(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->qryname;
}

char* getBsdGeoSvg(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bsdgeosvg;
}

char* getBsdGeoDot(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bsdgeodot;
}

char* getBsdGeoQrySvg(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bsdgeoqrysvg;
}

char* getBsdGeoQryTxt(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bsdgeoqrytxt;
}

char* getBsdGeoQryDot(void* aptoget)
{
    all_paths* ap = aptoget;
    return ap->bsdgeoqrydot;
}
