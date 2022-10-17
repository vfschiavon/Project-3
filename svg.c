#include "svg.h"

#include "forms.h"

void boatCases(Info i, double x, double y, void** files)
{
    fprintf((FILE*) files[0], "Boat %d ended with M$ %.2lf\n", getFormId(i), getNauBalance(i));
    if (getNauEnergy(i) == 0)
    {
        fprintf((FILE*) files[1], RECT_SVG, getFormId(i), x, y, getFormW(i), getFormH(i), "#484537", getFormCorp(i));
    }
    else if (getNauEnergy(i) < 10)
    {
        fprintf((FILE*) files[1], RECT_SVG, getFormId(i), x, y, getFormW(i), getFormH(i), "#FFCC00", getFormCorp(i));
    }
    else if (getNauEnergy(i) < 30)
    {
        fprintf((FILE*) files[1], RECT_SVG, getFormId(i), x, y, getFormW(i), getFormH(i), "#217821", getFormCorp(i));
    }
    else
    {
        fprintf((FILE*) files[1], RECT_SVG, getFormId(i), x, y, getFormW(i), getFormH(i), "#800066", getFormCorp(i));
    }
}

void printSvg(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    void** files = aux;
    switch (getFormType(i))
    {
        case CIRCLE:
            fprintf((FILE*) files[1], CIRC_SVG, getFormId(i), x, y, getFormR(i), getFormCorb(i), getFormCorp(i));
            break;
        case RECTANGLE:
            boatCases(i, x, y, files);
            break;
        case LINE:
            fprintf((FILE*) files[1], LINE_SVG, getFormId(i), getFormX(i), getFormY(i), getFormX2(i), getFormY2(i), getFormCorb(i));
            break;
        case TEXT:
            fprintf((FILE*) files[1], TEXT_SVG, getFormId(i), x, y, getFormCorb(i), getFormCorp(i), getFormA(i), getFormTxto(i));
            break;
        default:
            break;
    }
}
