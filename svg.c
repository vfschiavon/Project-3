#include "svg.h"

#include "forms.h"

void printSvg(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux)
{
    FILE* svg = aux;
    switch (getFormType(i))
    {
        case CIRCLE:
            fprintf(svg, CIRC_SVG, getFormId(i), getFormX(i), getFormY(i), getFormR(i), getFormCorb(i), getFormCorp(i));
            break;
        case RECTANGLE:
            fprintf(svg, RECT_SVG, getFormId(i), getFormX(i), getFormY(i), getFormW(i), getFormH(i), getFormCorb(i), getFormCorp(i));
            break;
        case LINE:
            fprintf(svg, LINE_SVG, getFormId(i), getFormX(i), getFormY(i), getFormX2(i), getFormY2(i), getFormCorb(i));
            break;
        case TEXT:
            fprintf(svg, TEXT_SVG, getFormId(i), getFormX(i), getFormY(i), getFormCorb(i), getFormCorp(i), defineAnchor(getFormA(i)), getFormTxto(i));
            break;
        default:
            break;
    }
}
