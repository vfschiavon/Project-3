#include "forms.h"

typedef struct form
{
    int type;
    int id;
    double energy; // Exclusive for naus (rectagles)
    double balance; // Exclusive for naus (rectagles)
    double x;
    double x2;
    double y;
    double y2;
    double r;
    double w;
    double h;
    char corb[20];
    char corp[20];
    char a[10];
    char txto[100];
} form;

void* createForm()
{
    form* f = calloc(1, sizeof(form));
    return f;
}

char* defineAnchor(char a)
{
    char* anchor;
    if (a == 'i')
    {
        anchor = "start";
    }
    else if (a == 'm')
    {
        anchor = "middle";
    }
    else if (a == 'f')
    {
        anchor = "end";
    }
    return anchor;
}

void defineLineAnchor(double* xref, double* yref, double x1, double y1, double x2, double y2)
{
    if (x1 == x2)
    {
        if (y1 < y2)
        {
            *xref = x1;
            *yref = y1;
        }
        else
        {
            *xref = x2;
            *yref = y2;
        }
    }
    else
    {
        if (x1 < x2)
        {
            *xref = x1;
            *yref = y1;
        }
        else
        {
            *xref = x2;
            *yref = y2;
        }
    }
}

char* formTypeToString(int type)
{
    char* typeString;
    switch(type)
    {
        case CIRCLE:
            typeString = "fish";
            break;
        case RECTANGLE:
            typeString = "ship";
            break;
        case LINE:
            typeString = "shrimp";
            break;
        case TEXT:
            typeString = "lobster or coin or algae or debris";
            break;
        default:
            break;
    }
    return typeString;
}

/*>>>>>>>>>>All sets<<<<<<<<<<*/

void setFullCirc(void* formtoset, int id, double x, double y, double r, char* corb, char* corp)
{
    form* f = formtoset;
    f->type = CIRCLE;
    f->id = id;
    f->x = x;
    f->y = y;
    f->r = r;
    strcpy(f->corb, corb);
    strcpy(f->corp, corp);
}

void setFullRect(void* formtoset, int id, double x, double y, double w, double h, char* corb, char* corp)
{
    form* f = formtoset;
    f->type = RECTANGLE;
    f->id = id;
    f->x = x;
    f->y = y;
    f->w = w;
    f->h = h;
    strcpy(f->corb, corb);
    strcpy(f->corp, corp);
}

void setFullLine(void* formtoset, int id, double x, double y, double x2, double y2, char* corb)
{
    form* f = formtoset;
    f->type = LINE;
    f->id = id;
    f->x = x;
    f->y = y;
    f->x2 = x2;
    f->y2 = y2;
    strcpy(f->corb, corb);
}

void setFullText(void* formtoset, int id, double x, double y, char* corb, char* a, char* txto)
{
    form* f = formtoset;
    f->type = TEXT;
    f->id = id;
    f->x = x;
    f->y = y;
    strcpy(f->corb, corb);
    strcpy(f->a, a);
    strcpy(f->txto, txto);
}

void setNauEnergy(void* formtoset, double energy)
{
    form* f = formtoset;
    if (f->type == RECTANGLE)
    {
        f->energy = energy;
    }
}

void setNauBalance(void* formtoset, double balance)
{
    form* f = formtoset;
    if (f->type == RECTANGLE)
    {
        f->balance = balance;
    }
}

void setFormX(void* formtoset, double x)
{
    form* f = formtoset;
    f->x = x;
}

void setFormX2(void* formtoset, double x2)
{
    form* f = formtoset;
    f->x2 = x2;
}

void setFormY(void* formtoset, double y)
{
    form* f = formtoset;
    f->y = y;
}

void setFormY2(void* formtoset, double y2)
{
    form* f = formtoset;
    f->y2 = y2;
}

void setFormCorb(void* formtoset, char* corb)
{
    form* f = formtoset;
    strcpy(f->corb, corb);
}

void setFormCorp(void* formtoset, char* corp)
{
    form* f = formtoset;
    strcpy(f->corp, corp);
}

/*>>>>>>>>>>All gets<<<<<<<<<<*/

int getFormType(void* formtoget)
{
    form* f = formtoget;
    return f->type;
}

int getFormId(void* formtoget)
{
    form* f = formtoget;
    return f->id;
}

double getNauEnergy(void* formtoget)
{
    form* f = formtoget;
    if (f->type == RECTANGLE)
    {
        return f->energy;
    }
    return 0;
}

double getNauBalance(void* formtoget)
{
    form* f = formtoget;
    if (f->type == RECTANGLE)
    {
        return f->balance;
    }
    return 0;
}

double getFormX(void* formtoget)
{
    form* f = formtoget;
    return f->x;
}

double getFormX2(void* formtoget)
{
    form* f = formtoget;
    return f->x2;
}

double getFormY(void* formtoget)
{
    form* f = formtoget;
    return f->y;
}

double getFormY2(void* formtoget)
{
    form* f = formtoget;
    return f->y2;
}

double getFormR(void* formtoget)
{
    form* f = formtoget;
    return f->r;
}

double getFormW(void* formtoget)
{
    form* f = formtoget;
    return f->w;
}

double getFormH(void* formtoget)
{
    form* f = formtoget;
    return f->h;
}

char* getFormCorb(void* formtoget)
{
    form* f = formtoget;
    return f->corb;
}

char* getFormCorp(void* formtoget)
{
    form* f = formtoget;
    return f->corp;
}

char* getFormA(void* formtoget)
{
    form* f = formtoget;
    return f->a;
}

char* getFormTxto(void* formtoget)
{
    form* f = formtoget;
    return f->txto;
}

/*>>>>>>>>>>Verifiers<<<<<<<<<<*/

bool circleFullInside(double cx, double cy, double r, double x1, double y1, double x2, double y2)
{
    if (cx - r >= x1 && cx + r <= x2 && cy - r >= y1 && cy + r <= y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rectangleFullInside(double rx, double ry, double w, double h, double x1, double y1, double x2, double y2)
{
    if (rx >= x1 && rx + w <= x2 && ry >= y1 && ry + h <= y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool lineFullInside(double lx1, double ly1, double lx2, double ly2, double x1, double y1, double x2, double y2)
{
    if (lx1 >= x1 && lx1 <= x2 && ly1 >= y1 && ly1 <= y2 && lx2 >= x1 && lx2 <= x2 && ly2 >= y1 && ly2 <= y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool textFullInside(double tx, double ty, double x1, double y1, double x2, double y2)
{
    if (tx >= x1 && tx <= x2 && ty >= y1 && ty <= y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool formFullInside(Info i, double x1, double y1 , double x2, double y2)
{
    switch (getFormType(i))
    {
        case CIRCLE:
            return circleFullInside(getFormX(i), getFormY(i), getFormR(i), x1, y1, x2, y2);
            break;
        case RECTANGLE:
            return rectangleFullInside(getFormX(i), getFormY(i), getFormW(i), getFormH(i), x1, y1, x2, y2);
            break;
        case LINE:
            return lineFullInside(getFormX(i), getFormY(i), getFormX2(i), getFormY2(i), x1, y1, x2, y2);
            break;
        case TEXT:
            return textFullInside(getFormX(i), getFormY(i), x1, y1, x2, y2);
            break;
        default:
            break;
    }
    return false;
}

bool isAnchorAndMbbEqual(void* orignial, void* tocompare)
{
    form* orig = orignial;
    form* comp = tocompare;

    switch (orig->type)
    {
        case CIRCLE:
            if (orig->x == comp->x && orig->y == comp->y && orig->r == comp->r)
            {
                return true;
            }
            break;
        case RECTANGLE:
            if (orig->x == comp->x && orig->y == comp->y && orig->w == comp->w && orig->h == comp->h)
            {
                return true;
            }
            break;
        case LINE:;
            double ancOrigX, ancOrigY, ancCompX, ancCompY;
            defineLineAnchor(&ancOrigX, &ancOrigY, orig->x, orig->y, orig->x2, orig->y2);
            defineLineAnchor(&ancCompX, &ancCompY, comp->x, comp->y, comp->x2, comp->y2);
            if (ancOrigX == ancCompX && ancOrigY == ancCompY)
            {
                if (fabs(orig->x2 - orig->x) == fabs(comp->x2 - comp->x) && fabs(orig->y2 - orig->y) == fabs(comp->y2 - comp->y))
                {
                    return true;
                }
            }
            break;
        case TEXT:
            if (orig->x == comp->x && orig->y == comp->y)
            {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}