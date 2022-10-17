#include "qry.h"

#include "forms.h"

// Used in MV, LR and D
void searchId(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    void** returnArray = aux;
    if (getFormId(i) == *(int*)returnArray[0])
    {
        returnArray[1] = i;
    }
}

// Used in LR and D
void calculatePos(char* side, double d, double x, double y, double w, double h, double* xref, double* yref)
{
    if (!strcmp(side, "PP")) // Popa
    {
        *xref = x + (w / 2);
        *yref = y - d;
    }
    else if (!strcmp(side, "BB")) // Bombordo
    {
        *xref = x + w + d;
        *yref = y + (h / 2);
    }
    else if (!strcmp(side, "PR")) // Proa
    {
        *xref = x + (w / 2);
        *yref = y + h + d;
    }
    else if (!strcmp(side, "EB")) // Estibordo
    {
        *xref = x - d;
        *yref = y + (h / 2);
    }
}

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
        fprintf(e->qrytxt, "Energized ship %d with energy %.2lf\n", getFormId(i), e->v);
        setNauEnergy(i, e->v);
    }
}

void e(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    struct e* aux = calloc(1, sizeof(struct e));
    aux->qrytxt = qrytxt;
    fscanf(qry, "%lf", &aux->v);
    fprintf(qrytxt, "\n>Energizing all ships with %.2lf:\n", aux->v);
    percursoProfundidade(tree, energize, aux);
    free(aux);
}

/* Functions for MV */
void moveMV(Info i, double dx, double dy, void* tree, FILE* qrytxt)
{
    void* tempInfo = NULL;
    if (getFormType(i) != LINE)
    {
        tempInfo = removeSRb(tree, getFormX(i), getFormY(i), 0, 0, 0, 0);
        setFormX(tempInfo, getFormX(tempInfo) + dx);
        setFormY(tempInfo, getFormY(tempInfo) + dy);
    }
    else
    {
        double xanchor, yanchor;
        defineLineAnchor(&xanchor, &yanchor, getFormX(i), getFormY(i), getFormX2(i), getFormY2(i));
        tempInfo = removeSRb(tree, xanchor, yanchor, 0, 0, 0, 0);
        setFormX(tempInfo, getFormX(tempInfo) + dx);
        setFormY(tempInfo, getFormY(tempInfo) + dy);
        setFormX2(tempInfo, getFormX2(tempInfo) + dx);
        setFormY2(tempInfo, getFormY2(tempInfo) + dy);
    }
    switch (getFormType(tempInfo))
    {
        case CIRCLE:
            insertSRb(tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo) - getFormR(tempInfo), getFormY(tempInfo) - getFormR(tempInfo), getFormX(tempInfo) + getFormR(tempInfo), getFormY(tempInfo) + getFormR(tempInfo), tempInfo);
            fprintf(qrytxt, "Moved %s %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - dx, getFormY(tempInfo) - dy, getFormX(tempInfo), getFormY(tempInfo));
            break;
        case RECTANGLE:
            setNauEnergy(tempInfo, getNauEnergy(tempInfo) - (sqrt(pow(dx, 2) + pow(dy, 2)) / 5));
            insertSRb(tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo) + getFormW(tempInfo), getFormY(tempInfo) + getFormH(tempInfo), tempInfo);
            fprintf(qrytxt, "Moved %s %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - dx, getFormY(tempInfo) - dy, getFormX(tempInfo), getFormY(tempInfo));
            break;
        case LINE:;
            double xanchor, yanchor;
            defineLineAnchor(&xanchor, &yanchor, getFormX(tempInfo), getFormY(tempInfo), getFormX2(tempInfo), getFormY2(tempInfo));
            insertSRb(tree, xanchor, yanchor, xanchor, yanchor, xanchor + fabs(getFormX2(tempInfo) - getFormX(tempInfo)), yanchor + fabs(getFormY2(tempInfo) - getFormY(tempInfo)), tempInfo);
            fprintf(qrytxt, "Moved %s %d from (%.2lf, %.2lf, %.2lf, %.2lf) to (%.2lf, %.2lf, %.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - dx, getFormY(tempInfo) - dy, getFormX2(tempInfo) - dx, getFormY2(tempInfo) - dy, getFormX(tempInfo), getFormY(tempInfo), getFormX2(tempInfo), getFormY2(tempInfo));
            break;
        case TEXT:
            insertSRb(tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), tempInfo);
            fprintf(qrytxt, "Moved %s %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - dx, getFormY(tempInfo) - dy, getFormX(tempInfo), getFormY(tempInfo));
            break;
        default:
            break;
    }
}

void mv(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    int id;
    double dx, dy;
    fscanf(qry, "%d %lf %lf", &id, &dx, &dy);
    fprintf(qrytxt, "\n>Moving form %d by (%.2lf, %.2lf):\n", id, dx, dy);
    void* pArray[2] = {&id, NULL};
    percursoProfundidade(tree, searchId, pArray);
    if (pArray[1])
    {
        moveMV(pArray[1], dx, dy, tree, qrytxt);
    }
}

/* Functions and structs for LR */
struct lr
{
    int id;
    char side[3];
    double d;
    double w;
    double h;
    bool hit;
    void* tree;
    FILE* qrytxt;
    FILE* qrysvg;
};

void lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    struct lr* aux = calloc(1, sizeof(struct lr));
    aux->tree = tree;
    aux->qrytxt = qrytxt;
    aux->qrysvg = qrysvg;
    aux->hit = false;
    fscanf(qry, "%d %s %lf %lf %lf", &aux->id, aux->side, &aux->d, &aux->w, &aux->h);
    fprintf(qrytxt, "\n>The ship %d will throw a net in the %s side with a distance of %.2lf, width of %.2lf and height of %.2lf:\n", aux->id, aux->side, aux->d, aux->w, aux->h);
    percursoProfundidade(tree, searchId, aux);
    free(aux);
}

/* Functions and structs for D */
struct d
{
    int id;
    char side[3];
    double d;
    double shootX;
    double shootY;
    bool hit;
    void* tree;
    FILE* qrytxt;
    FILE* qrysvg;
};

void shotHit(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    struct d* d = aux;
    if (getFormType(i) == RECTANGLE)
    {
        if (d->shootX >= x && d->shootY >= y && d->shootX <= x + getFormW(i) && d->shootY <= y + getFormH(i))
        {
            d->hit = true;
            fprintf(d->qrytxt, "Shot hit ship %d (%.2lf, %.2lf, %.2lf, %.2lf)\n", getFormId(i), x, y, getFormW(i), getFormH(i));
            removeSRb(d->tree, x, y, 0, 0, 0, 0);
        }
    }
}

void shoot(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    struct d* d = aux;
    if (getFormId(i) == d->id)
    {
        if (getFormType(i) == RECTANGLE)
        {
            if (getNauEnergy(i) >= d->d)
            {
                setNauEnergy(i, getNauEnergy(i) - d->d);
                calculatePos(d->side, d->d, x, y, getFormW(i), getFormH(i), &d->shootX, &d->shootY);
                fprintf(d->qrytxt, "Shot at (%.2lf, %.2lf)\n", d->shootX, d->shootY);
                fprintf(d->qrysvg, TEXT_SVG, -1, d->shootX, d->shootY, "black", "black", "middle", "*");
                percursoProfundidade(d->tree, shotHit, aux);
                if (!d->hit)
                {
                    fprintf(d->qrytxt, "Shot missed.\n");
                }
            }
            else
            {
                fprintf(d->qrytxt, "This ship has no energy to shoot.\n");
            }
        }
    }
}

void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    struct d* aux = calloc(1, sizeof(struct d));
    aux->tree = tree;
    aux->qrytxt = qrytxt;
    aux->qrysvg = qrysvg;
    aux->hit = false;
    fscanf(qry, "%d %s %lf", &aux->id, aux->side, &aux->d);
    fprintf(qrytxt, "\n>The ship %d will shoot in the %s side with a distance of %.2lf:\n", aux->id, aux->side, aux->d);
    percursoProfundidade(tree, shoot, aux);
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
    fprintf(mc->qrytxt, "Moved %s %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - mc->dx, getFormY(tempInfo) - mc->dy, getFormX(tempInfo), getFormY(tempInfo));
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
