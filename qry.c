#include "qry.h"

#include "forms.h"

/* Functions and structs for E */
struct e
{
    double v;
    FILE* qrytxt;
};

void energize(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    struct e* e = aux;
    if (getFormType(i) == RECTANGLE)
    {
        fprintf(e->qrytxt, "Energized %s id %d with energy %.2lf\n", formTypeToString(getFormType(i)), getFormId(i), e->v);
        setNauEnergy(i, e->v);
    }
}

void e(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    struct e* aux = calloc(1, sizeof(struct e));
    aux->qrytxt = qrytxt;
    fscanf(qry, "%lf", &aux->v);
    fprintf(qrytxt, "\n>Energizing all vessels with %.2lf:\n", aux->v);
    percursoProfundidade(tree, energize, aux);
    free(aux);
}

/* Functions and structs for MV */
struct mv
{
    int id;
    double dx;
    double dy;
    void* info;
    void* tree;
    FILE* qrytxt;
};

void searchId(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    struct mv* mv = aux;
    if (getFormId(i) == mv->id)
    {
        mv->info = i;
    }
}

void move(Info i, void* aux)
{
    struct mv* mv = aux;
    void* tempInfo = NULL;
    if (getFormType(i) != LINE)
    {
        tempInfo = removeSRb(mv->tree, getFormX(i), getFormY(i), 0, 0, 0, 0);
        setFormX(tempInfo, getFormX(tempInfo) + mv->dx);
        setFormY(tempInfo, getFormY(tempInfo) + mv->dy);
    }
    else
    {
        double xanchor, yanchor;
        defineLineAnchor(&xanchor, &yanchor, getFormX(i), getFormY(i), getFormX2(i), getFormY2(i));
        tempInfo = removeSRb(mv->tree, xanchor, yanchor, 0, 0, 0, 0);
        setFormX(tempInfo, getFormX(tempInfo) + mv->dx);
        setFormY(tempInfo, getFormY(tempInfo) + mv->dy);
        setFormX2(tempInfo, getFormX2(tempInfo) + mv->dx);
        setFormY2(tempInfo, getFormY2(tempInfo) + mv->dy);
    }
    switch (getFormType(tempInfo))
    {
        case CIRCLE:
            insertSRb(mv->tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo) - getFormR(tempInfo), getFormY(tempInfo) - getFormR(tempInfo), getFormX(tempInfo) + getFormR(tempInfo), getFormY(tempInfo) + getFormR(tempInfo), tempInfo);
            fprintf(mv->qrytxt, "Moved %s id %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - mv->dx, getFormY(tempInfo) - mv->dy, getFormX(tempInfo), getFormY(tempInfo));
            break;
        case RECTANGLE:
            insertSRb(mv->tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo) + getFormW(tempInfo), getFormY(tempInfo) + getFormH(tempInfo), tempInfo);
            fprintf(mv->qrytxt, "Moved %s id %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - mv->dx, getFormY(tempInfo) - mv->dy, getFormX(tempInfo), getFormY(tempInfo));
            break;
        case LINE:;
            double xanchor, yanchor;
            defineLineAnchor(&xanchor, &yanchor, getFormX(tempInfo), getFormY(tempInfo), getFormX2(tempInfo), getFormY2(tempInfo));
            insertSRb(mv->tree, xanchor, yanchor, xanchor, yanchor, xanchor + fabs(getFormX2(tempInfo) - getFormX(tempInfo)), yanchor + fabs(getFormY2(tempInfo) - getFormY(tempInfo)), tempInfo);
            fprintf(mv->qrytxt, "Moved %s id %d from (%.2lf, %.2lf, %.2lf, %.2lf) to (%.2lf, %.2lf, %.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - mv->dx, getFormY(tempInfo) - mv->dy, getFormX2(tempInfo) - mv->dx, getFormY2(tempInfo) - mv->dy, getFormX(tempInfo), getFormY(tempInfo), getFormX2(tempInfo), getFormY2(tempInfo));
            break;
        case TEXT:
            insertSRb(mv->tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), tempInfo);
            fprintf(mv->qrytxt, "Moved %s id %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - mv->dx, getFormY(tempInfo) - mv->dy, getFormX(tempInfo), getFormY(tempInfo));
            break;
        default:
            break;
    }
}

void mv(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    struct mv* aux = calloc(1, sizeof(struct mv));
    aux->qrytxt = qrytxt;
    aux->tree = tree;
    fscanf(qry, "%d %lf %lf", &aux->id, &aux->dx, &aux->dy);
    fprintf(qrytxt, "\n>Moving form id %d by (%.2lf, %.2lf):\n", aux->id, aux->dx, aux->dy);
    percursoProfundidade(tree, searchId, aux);
    if (aux->info)
    {
        move(aux->info, aux);
    }
    free(aux);
}

/* Functions and structs for LR */
void lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    int id;
    char* lado;
    double d, w, h;
    fscanf(qry, "%d %s %lf %lf %lf", &id, lado, &d, &w, &h);
}

/* Functions and structs for D */
void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    int id;
    double d;
    char* lado;
    fscanf(qry, "%d %s %lf", &id, lado, &d);
}

/* Functions and structs for MC */
void mc(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    double dx, dy, x, y, w, h;
    fscanf(qry, "%lf %lf %lf %lf %lf %lf", &dx, &dy, &x, &y, &w, &h);
}
