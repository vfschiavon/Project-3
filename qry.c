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
    if (getFormType(i) == RECTANGLE)
    {
        struct e* e = aux;
        fprintf(e->qrytxt, "Energized boat %d with energy %.2lf\n", getFormId(i), e->v);
        setNauEnergy(i, e->v);
    }
}

void e(SRbTree tree, FILE* qry, FILE* qrytxt)
{
    struct e* aux = calloc(1, sizeof(struct e));
    aux->qrytxt = qrytxt;
    fscanf(qry, "%lf", &aux->v);
    fprintf(qrytxt, "\n>Energizing all boats with %.2lf:\n", aux->v);
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
            if (getNauEnergy(i) >= sqrt(pow(dx, 2) + pow(dy, 2)) / 5)
            {
                setNauEnergy(tempInfo, getNauEnergy(tempInfo) - (sqrt(pow(dx, 2) + pow(dy, 2)) / 5));
                insertSRb(tree, getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo), getFormY(tempInfo), getFormX(tempInfo) + getFormW(tempInfo), getFormY(tempInfo) + getFormH(tempInfo), tempInfo);
                fprintf(qrytxt, "Moved %s %d from (%.2lf, %.2lf) to (%.2lf, %.2lf)\n", formTypeToString(getFormType(tempInfo)), getFormId(tempInfo), getFormX(tempInfo) - dx, getFormY(tempInfo) - dy, getFormX(tempInfo), getFormY(tempInfo));
            }
            else
            {
                fprintf(qrytxt, "This boat has no energy to move.\n");
            }
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
    else
    {
        fprintf(qrytxt, "Form %d not found.\n", id);
    }
}

/* Functions and structs for LR */
struct lr
{
    double throwX;
    double throwY;
    double w;
    double h;
    double total;
    bool captured;
    FILE* qrytxt;
};

void verifyNet(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    if (getFormType(i) != RECTANGLE)
    {
        struct lr* lr = aux;
        if (formFullInside(i, lr->throwX, lr->throwY, lr->throwX + lr->w, lr->throwY + lr->h))
        {
            lr->captured = true;
            if (getFormType(i) == CIRCLE)
            {
                fprintf(lr->qrytxt, "Captured fish %d (%.2lf, %.2lf) worth M$ 5,00\n", getFormId(i), x, y);
                lr->total += 5;
            }
            else if (getFormType(i) == LINE)
            {
                fprintf(lr->qrytxt, "Captured shrimp %d (%.2lf, %.2lf, %.2lf, %.2lf) worth M$ 1,00\n", getFormId(i), getFormX(i), getFormY(i), getFormX2(i), getFormY2(i));
                lr->total += 1;
            }
            else
            {
                if (!strcmp(getFormTxto(i), ">-|-<"))
                {
                    fprintf(lr->qrytxt, "Captured lobster %d (%.2lf, %.2lf) worth M$ 20,00\n", getFormId(i), x, y);
                }
                else if (!strcmp(getFormTxto(i), "$"))
                {
                    fprintf(lr->qrytxt, "Captured coin %d (%.2lf, %.2lf) worth 0.5 energy.\n", getFormId(i), x, y);
                    setNauEnergy(i, getNauEnergy(i) + 0.5);
                }
                else
                {
                    fprintf(lr->qrytxt, "Captured algae or debris %d (%.2lf, %.2lf) worth nothing.\n", getFormId(i), x, y);
                }
            }
        }
    }
}

double throwNet(Info i, char* side, double d, double w, double h, void* tree, FILE* qrytxt, FILE* qrysvg)
{
    if (getFormType(i) == RECTANGLE)
    {
        if (getNauEnergy(i) >= w * h * d / 125)
        {
            double throwX, throwY;
            struct lr* aux = calloc(1, sizeof(struct lr));
            aux->w = w;
            aux->h = h;
            aux->qrytxt = qrytxt;
            fprintf(qrytxt, "Energy before throw: %.2lf\n", getNauEnergy(i));
            setNauEnergy(i, getNauEnergy(i) - w * h * d / 125);
            fprintf(qrytxt, "Energy after throw: %.2lf\n", getNauEnergy(i));
            calculatePos(side, d, getFormX(i), getFormY(i), getFormW(i), getFormH(i), &throwX, &throwY);
            aux->throwX = throwX;
            aux->throwY = throwY;
            fprintf(qrytxt, "Net thrown at (%.2lf, %.2lf, %.2lf, %.2lf)\n", throwX, throwY, w, h);
            fprintf(qrysvg, RECT_SVG, -1, throwX, throwY, w, h, "black", "none");
            percursoProfundidade(tree, verifyNet, aux);
            setNauBalance(i, getNauBalance(i) + aux->total);
            if (!aux->captured)
            {
                fprintf(qrytxt, "Didn't capture anything.\n");
            }
            double total = aux->total;
            free(aux);
            return total;
        }
        else
        {
            fprintf(qrytxt, "This boat has no energy to throw a net.\n");
        }
    }
    return 0;
}

double lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    int id;
    char side[3];
    double d, w, h;
    fscanf(qry, "%d %s %lf %lf %lf", &id, side, &d, &w, &h);
    fprintf(qrytxt, "\n>The boat %d will throw a net in the %s side with a distance of %.2lf, width of %.2lf and height of %.2lf:\n", id, side, d, w, h);
    void* pArray[2] = {&id, NULL};
    percursoProfundidade(tree, searchId, pArray);
    if (pArray[1])
    {
        double total = throwNet(pArray[1], side, d, w, h, tree, qrytxt, qrysvg);
        fprintf(qrytxt, "Total captured: M$ %.2lf\n", total);
        return total;
    }
    else
    {
        fprintf(qrytxt, "Boat %d not found.\n", id);
    }
    return 0;
}

/* Functions and structs for D */
struct d
{
    double shootX;
    double shootY;
    bool hit;
    void* shooter;
    void* tree;
    FILE* qrytxt;
};

void shotHit(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    if (getFormType(i) == RECTANGLE)
    {
        struct d* d = aux;
        if (d->shootX >= x && d->shootY >= y && d->shootX <= x + getFormW(i) && d->shootY <= y + getFormH(i))
        {
            d->hit = true;
            fprintf(d->qrytxt, "Shot hit boat %d (%.2lf, %.2lf, %.2lf, %.2lf)\n", getFormId(i), x, y, getFormW(i), getFormH(i));
            fprintf(d->qrytxt, "The attacking boat captured M$ %.2lf from this destroyed boat.\n", getNauBalance(i));
            setNauBalance(d->shooter, getNauBalance(d->shooter) + getNauBalance(i));
            removeSRb(d->tree, x, y, 0, 0, 0, 0);
        }
    }
}

void shoot(Info i, char* side, double d, void* tree, FILE* qrytxt, FILE* qrysvg)
{
    if (getFormType(i) == RECTANGLE)
    {
        if (getNauEnergy(i) >= d)
        {
            double shootX, shootY;
            struct d* aux = calloc(1, sizeof(struct d));
            aux->shooter = i;
            aux->tree = tree;
            aux->qrytxt = qrytxt;
            setNauEnergy(i, getNauEnergy(i) - d);
            calculatePos(side, d, getFormX(i), getFormY(i), getFormW(i), getFormH(i), &shootX, &shootY);
            aux->shootX = shootX;
            aux->shootY = shootY;
            fprintf(qrytxt, "Shot at (%.2lf, %.2lf)\n", shootX, shootY);
            fprintf(qrysvg, TEXT_SVG, -1, shootX, shootY, "black", "black", "middle", "*");
            percursoProfundidade(tree, shotHit, aux);
            if (!aux->hit)
            {
                fprintf(qrytxt, "Shot missed.\n");
            }
            free(aux);
        }
        else
        {
            fprintf(qrytxt, "This boat has no energy to shoot.\n");
        }
    }
}

void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{
    int id;
    char side[3];
    double d;
    fscanf(qry, "%d %s %lf", &id, side, &d);
    fprintf(qrytxt, "\n>The boat %d will shoot in the %s side with a distance of %.2lf:\n", id, side, d);
    void* pArray[2] = {&id, NULL};
    percursoProfundidade(tree, searchId, pArray);
    if (pArray[1])
    {
        shoot(pArray[1], side, d, tree, qrytxt, qrysvg);
    }
    else
    {
        fprintf(qrytxt, "Boat %d not found.\n", id);
    }
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
    if (getFormType(i) == CIRCLE)
    {
        struct mc* mc = aux;
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
