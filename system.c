#include "system.h"

#include "forms.h"
#include "paths.h"
#include "qry.h"
#include "svg.h"

/*>>>>>>>>>>readParam<<<<<<<<<<*/
void flagE(char** argv, int i, void* paths)
{
    char* bed = calloc(strlen(argv[i]) + 1, sizeof(char));
    strcpy(bed, argv[i]);
    bed = prepareDir(bed);
    setBed(paths, bed);
}

void flagF(char** argv, int i, void* paths)
{
    char* geofull = calloc(strlen(argv[i]) + 1, sizeof(char));
    char* geoname = calloc(strlen(argv[i]) + 1, sizeof(char));
    strcpy(geofull, argv[i]);
    prepareDoc(geofull, geoname);
    setGeoArq(paths, geofull);
    setGeoName(paths, geoname);
}

void flagO(char** argv, int i, void* paths)
{
    char* bsd = calloc(strlen(argv[i]) + 1, sizeof(char));
    strcpy(bsd, argv[i]);
    bsd = prepareDir(bsd);
    setBsd(paths, bsd);
}

void flagQ(char** argv, int i, void* paths)
{
    char* qryfull = calloc(strlen(argv[i]) + 1, sizeof(char));
    char* qryname = calloc(strlen(argv[i]) + 1, sizeof(char));
    strcpy(qryfull, argv[i]);
    prepareDoc(qryfull, qryname);
    setQryArq(paths, qryfull);
    setQryName(paths, qryname);
}

void bedAndGeo(void* paths)
{
    char* bedgeo = calloc(strlen(getBed(paths)) + strlen(getGeoArq(paths)) + 1, sizeof(char));
    strcpy(bedgeo, getBed(paths));
    strcat(bedgeo, getGeoArq(paths));
    setBedGeo(paths, bedgeo);
}

void bedAndQry(void* paths)
{
    char* bedqry = calloc(strlen(getBed(paths)) + strlen(getQryArq(paths)) + 1, sizeof(char));
    strcpy(bedqry, getBed(paths));
    strcat(bedqry, getQryArq(paths));
    setBedQry(paths, bedqry);
}

void bsdAndGeo(void* paths)
{
    char* bsdgeosvg = calloc(strlen(getBsd(paths)) + strlen(getGeoName(paths)) + 5, sizeof(char));
    strcpy(bsdgeosvg, getBsd(paths));
    strcat(bsdgeosvg, getGeoName(paths));
    strcat(bsdgeosvg, ".svg");
    setBsdGeoSvg(paths, bsdgeosvg);

    char* bsdgeodot = calloc(strlen(getBsd(paths)) + strlen(getGeoName(paths)) + 5, sizeof(char));
    strcpy(bsdgeodot, getBsd(paths));
    strcat(bsdgeodot, getGeoName(paths));
    strcat(bsdgeodot, ".dot");
    setBsdGeoDot(paths, bsdgeodot);
}

void bsdAndQry(void* paths)
{
    char* bsdgeoqrysvg = calloc(strlen(getBsd(paths)) + strlen(getGeoName(paths)) + strlen(getQryName(paths)) + 6, sizeof(char));
    char* bsdgeoqrytxt = calloc(strlen(getBsd(paths)) + strlen(getGeoName(paths)) + strlen(getQryName(paths)) + 6, sizeof(char));
    char* bsdgeoqrydot = calloc(strlen(getBsd(paths)) + strlen(getGeoName(paths)) + strlen(getQryName(paths)) + 6, sizeof(char));
    strcpy(bsdgeoqrysvg, getBsd(paths));
    strcat(bsdgeoqrysvg, getGeoName(paths));
    strcat(bsdgeoqrysvg, "_");
    strcat(bsdgeoqrysvg, getQryName(paths));
    strcpy(bsdgeoqrytxt, bsdgeoqrysvg);
    strcpy(bsdgeoqrydot, bsdgeoqrysvg);
    strcat(bsdgeoqrysvg, ".svg");
    strcat(bsdgeoqrytxt, ".txt");
    strcat(bsdgeoqrydot, ".dot");
    setBsdGeoQrySvg(paths, bsdgeoqrysvg);
    setBsdGeoQryTxt(paths, bsdgeoqrytxt);
    setBsdGeoQryDot(paths, bsdgeoqrydot);
}

void readParam(int argc, char** argv, void* paths)
{
    bool readbed = false, readgeo = false, readbsd = false, readqry = false;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-e")) //path - diretŕoio base de entrada (BED)
        {
            i++;
            readbed = true;
            flagE(argv, i, paths);
        }
        else if (!strcmp(argv[i], "-f")) //arq.geo
        {   
            i++;
            readgeo = true;
            flagF(argv, i, paths);
        }
        else if (!strcmp(argv[i], "-o")) //path - diretório base de saída (BSD)
        {
            i++;
            readbsd = true;
            flagO(argv, i, paths);
        }
        else if (!strcmp(argv[i], "-q")) //paste/arq.qry
        {
            i++;
            readqry = true;
            flagQ(argv, i, paths);
        }
    }

    if (readbed && readgeo) // bed/arq.geo
    {
        bedAndGeo(paths);
    }

    if (readbed && readqry) // bed/paste/arq.qry
    {
        bedAndQry(paths);
    }

    if (readbsd && readgeo) // bsd/geoname.svg
    {
        bsdAndGeo(paths);
    }

    if (readbsd && readqry) // bsd/geoname_qryname.svg and bsd/geoname_qryname.txt
    {
        bsdAndQry(paths);
    }
}

/*>>>>>>>>>>readPrintGeo<<<<<<<<<<*/
void savePrintCirc(SRbTree tree, FILE* geo, FILE* svg)
{
    int id;
    double x, y, r;
    char corb[20], corp[20];
    void* newform = createForm();
    fscanf(geo, "%d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);
    setFullCirc(newform, id, x, y, r, corb, corp);
    insertSRb(tree, x, y, x - r, y - r, x + r, y + r, newform);
    fprintf(svg, CIRC_SVG, id, x, y, r, corb, corp);
}

void savePrintRect(SRbTree tree, FILE* geo, FILE* svg)
{
    int id;
    double x, y, w, h;
    char corb[20], corp[20];
    void* newform = createForm();
    fscanf(geo, "%d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp);
    setFullRect(newform, id, x, y, w, h, corb, corp);
    insertSRb(tree, x, y, x, y, x + w, y + h, newform);
    fprintf(svg, RECT_SVG, id, x, y, w, h, corb, corp);
}

void savePrintLine(SRbTree tree, FILE* geo, FILE* svg)
{
    int id;
    double x, y, x2, y2, xanchor, yanchor;
    char cor[20];
    void* newform = createForm();
    fscanf(geo, "%d %lf %lf %lf %lf %s", &id, &x, &y, &x2, &y2, cor);
    setFullLine(newform, id, x, y, x2, y2, cor);
    defineLineAnchor(&xanchor, &yanchor, x, y, x2, y2);
    insertSRb(tree, xanchor, yanchor, xanchor, yanchor, xanchor + fabs(x2 - x), yanchor + fabs(y2 - y), newform);
    fprintf(svg, LINE_SVG, id, x, y, x2, y2, cor);
}

void savePrintText(SRbTree tree, FILE* geo, FILE* svg)
{
    int id;
    double x, y;
    char corb[20], corp[20], a, txto[100];
    void* newform = createForm();
    fscanf(geo, "%d %lf %lf %s %s %c %[^\n]s", &id, &x, &y, corb, corp, &a, txto);
    setFullText(newform, id, x, y, corb, defineAnchor(a), txto);
    insertSRb(tree, x, y, x, y, x, y, newform);
    fprintf(svg, TEXT_SVG, id, x, y, corb, corp, defineAnchor(a), txto);
}

void readPrintGeo(SRbTree tree, void* paths)
{
    FILE* geo = fopen(getBedGeo(paths), "r");
    if (geo)
    {
        FILE* svg = fopen(getBsdGeoSvg(paths), "w");
        fprintf(svg, "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");
        if (svg)
        {
            char type[2];
            while (!feof(geo))
            {
                fscanf(geo, "%s", type);
                if (!strcmp(type, "c")) // Circle
                {
                    savePrintCirc(tree, geo, svg);
                }
                else if (!strcmp(type, "r")) // Rectangle
                {
                    savePrintRect(tree, geo, svg);
                }
                else if (!strcmp(type, "l")) // Line
                {
                    savePrintLine(tree, geo, svg);
                }
                else if (!strcmp(type, "t")) // Text
                {
                    savePrintText(tree, geo, svg);
                }
                strcpy(type, " ");
            }
            fprintf(svg, "</svg>\n");
            fclose(svg);
            printSRb(tree, getBsdGeoDot(paths));
        }
        else
        {
            printf("Error opening the first .svg file.\n");
            exit(EXIT_FAILURE);
        }
        fclose(geo);
    }
    else
    {
        printf("Error opening the .geo file.\n");
        exit(EXIT_FAILURE);
    }
}

/*>>>>>>>>>>readQry<<<<<<<<<<*/
void readQry(SRbTree tree, void* paths)
{
    char func[3];

    FILE* qry = fopen(getBedQry(paths), "r");

    if (qry)
    {
        FILE* qrytxt = fopen(getBsdGeoQryTxt(paths), "w");
        FILE* qrysvg = fopen(getBsdGeoQrySvg(paths), "w");
        fprintf(qrysvg, "<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");

        if (qrytxt && qrysvg)
        {
            double absTotal = 0;
            fprintf(qrytxt, ">>Start of pirate fishing:\n");

            while (!feof(qry))
            {
                fscanf(qry, "%s", func);

                if (!strcmp(func, "e")) // Energize
                {   
                    e(tree, qry, qrytxt);
                }
                else if (!strcmp(func, "mv")) // Move
                {
                    mv(tree, qry, qrytxt);
                }
                else if (!strcmp(func, "lr")) // Launch net
                {
                    absTotal += lr(tree, qry, qrytxt, qrysvg);
                }
                else if (!strcmp(func, "d")) // Shoot
                {
                    d(tree, qry, qrytxt, qrysvg);
                }
                else if (!strcmp(func, "mc")) // Move shoal
                {
                    mc(tree, qry, qrytxt);
                }
                strcpy(func, " ");
            }
            fprintf(qrytxt, "\n>Sum of all captures: M$ %.2lf\n", absTotal);
            void* files[2] = {qrytxt, qrysvg};
            fprintf(qrytxt, "\n>Results:\n");
            percursoProfundidade(tree, printSvg, files);
            fprintf(qrytxt, "\n>>End of pirate fishing.\n");
            fclose(qrytxt);
            fprintf(qrysvg, "</svg>\n");
            fclose(qrysvg);
            printSRb(tree, getBsdGeoQryDot(paths));
        }
        else
        {
            printf("Error opening the .txt or .svg file.\n");
            exit(EXIT_FAILURE);
        }
        fclose(qry);
    }
    else
    {
        printf("Error opening the .qry file.\n");
        exit(EXIT_FAILURE);
    }
}
