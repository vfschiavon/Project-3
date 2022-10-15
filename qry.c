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

void moveMV(Info i, void* aux)
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
            setNauEnergy(tempInfo, getNauEnergy(tempInfo) - (sqrt(pow(mv->dx, 2) + pow(mv->dy, 2)) / 5));
            updateEnergyByMove(tempInfo, mv);
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
        moveMV(aux->info, aux);
    }
    free(aux);
}

/* Functions and structs for LR */
struct lr
{
    int id;
    double d;
    double w;
    double h;
    char side[3];
    FILE* qrysvg;
    FILE* qrytxt;
};

void lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    struct lr* aux = calloc(1, sizeof(struct lr));
    aux->qrysvg = qrysvg;
    aux->qrytxt = qrytxt;
    fscanf(qry, "%d %s %lf %lf %lf", &aux->id, aux->side, &aux->d, &aux->w, &aux->h);

    free(aux);
}

/* Functions and structs for D */
struct d
{
    int id;
    char side[3];
    double d;
    FILE* qrysvg;
    FILE* qrytxt;
};

void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    struct d* aux = calloc(1, sizeof(struct d));
    aux->qrysvg = qrysvg;
    aux->qrytxt = qrytxt;
    fscanf(qry, "%d %s %lf", &aux->id, aux->side, &aux->d);
    
    free(aux);
}

/* Functions and structs for MC */
struct mc
{
    double dx;
    double dy;
    double x;
    double y;
    double w;
    double h;
    void* tree;
    FILE* qrytxt;
};

void moveMC(Info i, void* aux)
{
    struct mc* mc = aux;
    void* tempInfo = NULL;
    tempInfo = removeSRb(mc->tree, getFormX(i), getFormY(i), 0, 0, 0, 0);
    setFormX(tempInfo, getFormX(tempInfo) + mc->dx);
    setFormY(tempInfo, getFormY(tempInfo) + mc->dy);
    insertSRb(mc->tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo) - getFormR(tempInfo), getFormY(tempInfo) - getFormR(tempInfo), getFormX(tempInfo) + getFormR(tempInfo), getFormY(tempInfo) + getFormR(tempInfo), tempInfo);
    fprintf(mc->qrytxt, "Moved %s id %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - mc->dx, getFormY(tempInfo) - mc->dy, getFormX(tempInfo), getFormY(tempInfo));
}

void verifyInside(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    struct mc* mc = aux;
    if (getFormType(i) == CIRCLE)
    {
        if (formFullInside(i, mc->x, mc->y, mc->x + mc->w, mc->y + mc->h))
        {
            moveMC(i, aux);
        }
    }
}

void mc(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    struct mc* aux = calloc(1, sizeof(struct mc));
    aux->tree = tree;
    aux->qrytxt = qrytxt;
    fscanf(qry, "%lf %lf %lf %lf %lf %lf", &aux->dx, &aux->dy, &aux->x, &aux->y, &aux->w, &aux->h);
    fprintf(qrytxt, "\n>Moving forms inside rectangle (%.2lf, %.2lf, %.2lf, %.2lf) by (%.2lf, %.2lf):\n", aux->x, aux->y, aux->w, aux->h, aux->dx, aux->dy);
    percursoProfundidade(tree, verifyInside, aux);
    free(aux);
}
