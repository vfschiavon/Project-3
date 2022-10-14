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
    fprintf(e->qrytxt, "Energized %s id %d with energy %.2lf\n", formTypeToString(getFormType(i)), getFormId(i), e->v);
    setNauEnergy(i, e->v);
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
void mv(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{

}

/* Functions and structs for LR */
void lr(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{

}

/* Functions and structs for D */
void d(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{

}

/* Functions and structs for MC */
void mc(SRbTree tree, FILE* qry, FILE* qrytxt, FILE* qrysvg)
{

}
