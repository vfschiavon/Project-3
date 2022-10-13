#include "forms.h"
#include "libs.h"
#include "list.h"
#include "paths.h"
#include "qry.h"
#include "SRbTree.h"
#include "svg.h"
#include "system.h"

int main(int argc, char** argv)
{
    SRbTree tree = createSRb(0.5);

    void* paths = createAllPaths();

    readParam(argc, argv, paths);

    readPrintGeo(tree, paths);

    // if (getQryArq(paths))
    // {
    //     readQry(tree, paths);
    // }
    
    killSRb(tree);
    freeAllPaths(paths);
}
