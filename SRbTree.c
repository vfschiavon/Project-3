#include "libs.h"

#include "SRbTree.h"
#include "forms.h"
#include "qry.h"

typedef struct node
{
    // Anchors
    double xa;
    double ya;
    // Minimun bounding boxes of figures
    double mx1;
    double my1;
    double mx2;
    double my2; 
    // Minimun bounding boxes of subtrees
    double submx1;
    double submy1;
    double submx2;
    double submy2;
    // Info
    char color;
    Info info;
    //Pointers
    struct node* father;
    struct node* left;
    struct node* right;
} node;

typedef struct tree
{
    double epsilon;
    int size;
    node* root;
} tree;

/*Auxiliary functions*/
void leftRotate(SRbTree t, node* x)
{
    node* y = x->right;
    x->right = NULL;
    if (y)
    {
        if (y->left)
        {
            x->right = y->left;
        }
        if (y->left)
        {
            y->left->father = x;
        }
        y->father = x->father;
        if (!x->father)
        {
            tree* tre = t;
            tre->root = y;
        }
        else if (x == x->father->left)
        {
            x->father->left = y;
        }
        else
        {
            x->father->right = y;
        }
        y->left = x;
        x->father = y;
    }
}

void rightRotate(SRbTree t, node* x)
{
    node* y = x->left;
    x->left = NULL;
    if (y)
    {
        if (y->right)
        {
            x->left = y->right;
        }
        if (y->right)
        {
            y->right->father = x;
        }
        y->father = x->father;
        if (!x->father)
        {
            tree* tre = t;
            tre->root = y;
        }
        else if (x == x->father->right)
        {
            x->father->right = y;
        }
        else
        {
            x->father->left = y;
        }
        y->right = x;
        x->father = y;
    }
}   

void fixupSRb(SRbTree t, node* pZ)
{
    tree* tre = t;
    node* z = pZ;
    node* y = NULL;

    while (z->father && z->father->color == 'r')
    {
        if (z->father == z->father->father->left)
        {
            y = z->father->father->right;
            if (y && y->color == 'r')
            {
                z->father->color = 'b';
                y->color = 'b';
                z->father->father->color = 'r';
                z = z->father->father;
            }
            else
            {
                if (z == z->father->right)
                {
                    z = z->father;
                    leftRotate(t, z);
                }
                z->father->color = 'b';
                z->father->father->color = 'r';
                rightRotate(t, z->father->father);
            }
        }
        else
        {
            if (z->father == z->father->father->right) {
                y = z->father->father->left;
                if (y && y->color == 'r')
                {
                    z->father->color = 'b';
                    y->color = 'b';
                    z->father->father->color = 'r';
                    z = z->father->father;
                }
                else
                {
                    if (z == z->father->left)
                    {
                        z = z->father;
                        rightRotate(t, z);
                    }
                    z->father->color = 'b';
                    z->father->father->color = 'r';
                    leftRotate(t, z->father->father);
                }
            }
        }
    }
    tre->root->color = 'b';
}

void recursiveGetPartBbSRb(node* currentnode, double x, double y, double w, double h, Lista resultado)
{
    if (currentnode)
    {
        if ((currentnode->mx1 >= x && currentnode->my1 >= y && currentnode->mx1 <= x + w && currentnode->my1 <= y + h) ||
            (currentnode->mx2 >= x && currentnode->my2 >= y && currentnode->mx2 <= x + w && currentnode->my2 <= y + h) ||
            (currentnode->mx1 >= x && currentnode->my2 >= y && currentnode->mx1 <= x + w && currentnode->my2 <= y + h) ||
            (currentnode->mx2 >= x && currentnode->my1 >= y && currentnode->mx2 <= x + w && currentnode->my1 <= y + h))
        {
            insertAtTail(resultado, currentnode->info);
        }
        
        if ((currentnode->submx1 >= x && currentnode->submy1 >= y && currentnode->submx1 <= x + w && currentnode->submy1 <= y + h) ||
            (currentnode->submx2 >= x && currentnode->submy2 >= y && currentnode->submx2 <= x + w && currentnode->submy2 <= y + h) ||
            (currentnode->submx1 >= x && currentnode->submy2 >= y && currentnode->submx1 <= x + w && currentnode->submy2 <= y + h) ||
            (currentnode->submx2 >= x && currentnode->submy1 >= y && currentnode->submx2 <= x + w && currentnode->submy1 <= y + h))
        {
            recursiveGetPartBbSRb(currentnode->left, x, y, w, h, resultado);
            recursiveGetPartBbSRb(currentnode->right, x, y, w, h, resultado);
        }
    }
}

void recursiveGetBbSRb(node* currentnode, double x, double y, double w, double h, Lista resultado)
{
    if (currentnode)
    {
        if (currentnode->mx1 >= x && currentnode->mx2 <= x + w && currentnode->my1 >= y && currentnode->my2 <= y + h)
        {
            insertAtTail(resultado, currentnode->info);
        }
        if (currentnode->submx1 >= x && currentnode->submx1 <= x + w && currentnode->submy1 >= y && currentnode->submy1 <= y + h)
        {
            recursiveGetBbSRb(currentnode->left, x, y, w, h, resultado);
            recursiveGetBbSRb(currentnode->right, x, y, w, h, resultado);
        }
    }
}

node* searchNode(node* currentnode, double pXa, double pYa, double epsilon)
{
    if (currentnode)
    {
        node* result = NULL;
        if (fabs(currentnode->xa - pXa) <= epsilon && fabs(currentnode->ya - pYa) <= epsilon)
        {
            result = currentnode;
        }
        else
        {
            if (pXa < currentnode->xa || (pXa == currentnode->xa && pYa < currentnode->ya))
            {
                result = searchNode(currentnode->left, pXa, pYa, epsilon);
            }
            else
            {
                result = searchNode(currentnode->right, pXa, pYa, epsilon);
            }
        }
        return result;
    }
    else
    {
        return NULL;
    }
}

void transplantSRb(SRbTree t, node* u, node* v)
{
    tree* tre = t;
    if (!u->father)
    {
        tre->root = v;
    }
    else if (u == u->father->left)
    {
        u->father->left = v;
    }
    else
    {
        u->father->right = v;
    }
    if (v)
    {
        v->father = u->father;
    }
}

node* minimumNode(node* x)
{
    while (x->left)
    {
        x = x->left;
    }
    return x;
}

void deleteFixupSRb(tree* t, node* x)
{
    node* w = NULL;
    while (x != t->root && x->color == 'b')
    {
        if (x == x->father->left)
        {
            w = x->father->right;
            if (w->color == 'r')
            {
                w->color = 'b';
                x->father->color = 'r';
                leftRotate(t, x->father);
                w = x->father->right;
            }
            if (w->left && w->left->color == 'b' && w->right && w->right->color == 'b')
            {
                w->color = 'r';
                x = x->father;
            }
            else
            {
                if (w->right && w->right->color == 'b')
                {
                    if (w->left)
                    {
                        w->left->color = 'b';
                    }
                    w->color = 'r';
                    rightRotate(t, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = 'b';
                if (w->right)
                {
                    w->right->color = 'b';
                }
                leftRotate(t, x->father);
                x = t->root;
            }
        }
        else
        {
            w = x->father->left;
            if (w && w->color == 'r')
            {
                w->color = 'b';
                x->father->color = 'r';
                rightRotate(t, x->father);
                w = x->father->left;
            }
            if (w && w->right && w->right->color == 'b' && w->left && w->left->color == 'b')
            {
                w->color = 'r';
                x = x->father;
            }
            else
            {
                if (w && w->left && w->left->color == 'b')
                {
                    w->right->color = 'b';
                    w->color = 'r';
                    leftRotate(t, w);
                    w = x->father->left;
                }
                if (w)
                {
                    w->color = x->father->color;
                }
                x->father->color = 'b';
                if (w && w->left)
                {
                    w->left->color = 'b';
                }
                rightRotate(t, x->father);
                x = t->root;
            }
        }
    }
    x->color = 'b';
}

Info createSubstitute(Info original)
{
    void* copy = createForm();
    switch (getFormType(original))
    {
        case CIRCLE:
            setFullCirc(copy, getFormId(original), getFormX(original), getFormY(original), getFormR(original), getFormCorb(original), getFormCorp(original));
            break;
        case RECTANGLE:
            setFullRect(copy, getFormId(original), getFormX(original), getFormY(original), getFormW(original), getFormH(original), getFormCorb(original), getFormCorp(original));
            break;
        case LINE:
            setFullLine(copy, getFormId(original), getFormX(original), getFormY(original), getFormX2(original), getFormY2(original), getFormCorb(original));
            break;
        case TEXT:
            setFullText(copy, getFormId(original), getFormX(original), getFormY(original), getFormCorb(original), getFormA(original), getFormTxto(original));
            break;
        default:
            break;
    }
    return copy;
}

void recursivePrintNode(node* currentnode, FILE* dot)
{
    if (currentnode)
    {
        fprintf(dot, "\tnode[style=filled, color=%s]; %d;\n", currentnode->color == 'r' ? "red" : "black", getFormId(currentnode->info));
    }
    else 
    {
        return;
    }
    recursivePrintNode(currentnode->left, dot);
    recursivePrintNode(currentnode->right, dot);
}

void recursivePrintEdge(node* currentnode, FILE* dot)
{
    if (currentnode)
    {
        if (currentnode->left)
        {
            fprintf(dot, "\t%d -> %d [color=black];\n", getFormId(currentnode->info), getFormId(currentnode->left->info));
        }
        if (currentnode->right)
        {
            fprintf(dot, "\t%d -> %d [color=black];\n", getFormId(currentnode->info), getFormId(currentnode->right->info));
        }
    }
    else 
    {
        return;
    }
    recursivePrintEdge(currentnode->left, dot);
    recursivePrintEdge(currentnode->right, dot);
}

int findHeight(node* n)
{
    if (!n)
    {
        return 0;
    }
    int left = findHeight(n->left);
    int right = findHeight(n->right);
    if (left > right)
    {
        return left + 1;
    }
    else
    {
        return right + 1;
    }
}

void levelOrder(node* n, int level, FvisitaNo fVisita, void* aux)
{
    if (!n)
    {
        return;
    }
    if (level == 1)
    {
        fVisita(n->info, n->xa, n->ya, n->mx1, n->mx2, n->my1, n->my2, aux);
    }
    else if (level > 1)
    {
        levelOrder(n->left, level - 1, fVisita, aux);
        levelOrder(n->right, level - 1, fVisita, aux);
    }
}

void recursiveSimetrico(node* currentnode, FvisitaNo fVisita, void* aux)
{
    if (currentnode)
    {
        recursiveSimetrico(currentnode->left, fVisita, aux);
        fVisita(currentnode->info, currentnode->xa, currentnode->ya, currentnode->mx1, currentnode->mx2, currentnode->my1, currentnode->my2, aux);
        recursiveSimetrico(currentnode->right, fVisita, aux);
    }
    else 
    {
        return;
    }
}

void recursiveProfundidade(node* currentnode, FvisitaNo fVisita, void* aux)
{
    if (currentnode)
    {
        fVisita(currentnode->info, currentnode->xa, currentnode->ya, currentnode->mx1, currentnode->mx2, currentnode->my1, currentnode->my2, aux);
        recursiveProfundidade(currentnode->left, fVisita, aux);
        recursiveProfundidade(currentnode->right, fVisita, aux);
    }
    else 
    {
        return;
    }
}

void recursiveFree(void* currentnode)
{
    node* n = currentnode;

    if (!n) {return;}

    free(n->info);

    recursiveFree(n->left);
    recursiveFree(n->right);

    free(n);
}

/*Functions from .h*/
SRbTree createSRb(double epsilon)
{
    tree* tre = calloc(1, sizeof(tree));
    if (tre)
    {
        tre->epsilon = epsilon;
        return tre;
    }
    else
    {
        return NULL;
    }
}

Node insertSRb(SRbTree t, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, Info info)
{
    tree* tre = t;
    node* tRoot = tre->root;
    node* z = calloc(1, sizeof(node));
    z->xa = x;
    z->ya = y;
    z->mx1 = mbbX1;
    z->my1 = mbbY1;
    z->mx2 = mbbX2;
    z->my2 = mbbY2;
    z->info = info;

    node* aux = NULL;
    while (tRoot)
    {
        aux = tRoot;
        if (z->xa < tRoot->xa || (z->xa == tRoot->xa && z->ya < tRoot->ya))
        {
            tRoot = tRoot->left;
        }
        else
        {
            tRoot = tRoot->right;
        }
    }

    z->father = aux;
    if (!aux)
    {
        tre->root = z;
    }
    else if (z->xa < aux->xa || (z->xa == aux->xa && z->ya < aux->ya))
    {
        aux->left = z;
    }
    else
    {
        aux->right = z;
    }

    z->color = 'r';
    tre->size++;
    fixupSRb(t, z);
    return z;
}

Node insertBbSRb(SRbTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, Info info)
{
    return insertSRb(t, mbbX1, mbbY1,  mbbX1, mbbY1,  mbbX2, mbbY2,  info);
}

void getBbPartSRb(SRbTree t, double x, double y, double w, double h, Lista resultado)
{
    tree* tre = t;
    if (tre->root)
    {
        recursiveGetPartBbSRb(tre->root, x, y, w, h, resultado);
    }
}

void getBbSRb(SRbTree t, double x, double y, double w, double h, Lista resultado)
{
    tree* tre = t;
    if (tre->root)
    {
        recursiveGetBbSRb(tre->root, x, y, w, h, resultado);
    }
}

Info getInfoSRb(SRbTree t, Node n, double* xa, double* ya, double* mbbX1, double* mbbY1, double* mbbX2, double* mbbY2)
{
    node* no = n;
    if (xa)
    {
        *xa = no->xa;
    }
    if (ya)
    {
        *ya = no->ya;
    }
    if (mbbX1)
    {
        *mbbX1 = no->mx1;
    }
    if (mbbY1)
    {
        *mbbY1 = no->my1;
    }
    if (mbbX2)
    {
        *mbbX2 = no->mx2;
    }
    if (mbbY2)
    {
        *mbbY2 = no->my2;
    }
    return no->info;
}

Node getNodeSRb(SRbTree t, double xa, double ya, double* mbbX1, double* mbbY1, double* mbbX2, double* mbbY2)
{
    tree* tre = t;
    node* result = searchNode(tre->root, xa, ya, tre->epsilon);
    if (result)
    {
        if (mbbX1)
        {
            *mbbX1 = result->mx1;
        }
        if (mbbY1)
        {
            *mbbY1 = result->my1;
        }
        if (mbbX2)
        {
            *mbbX2 = result->mx2;
        }
        if (mbbY2)
        {
            *mbbY2 = result->my2;
        }
        return result;
    }
    else
    {
        return NULL;
    }
}

void updateInfoSRb(SRbTree t, Node n, Info i)
{
    node* no = n;
    if (no)
    {
        if (isAnchorAndMbbEqual(no->info, i))
        {
            no->info = i;
        }
    }
}

Info removeSRb(SRbTree t, double xa, double ya, double* mbbX1, double* mbbY1, double* mbbX2, double* mbbY2)
{
    tree* tre = t;
    node* z = searchNode(tre->root, xa, ya, tre->epsilon);
    node* y = z;
    node* x = NULL;
    char yOriginalColor = y->color;

    if (!z->left)
    {
        x = z->right;
        transplantSRb(t, z, z->right);
    }
    else if (!z->right)
    {
        x = z->left;
        transplantSRb(t, z, z->left);
    }
    else
    {
        y = minimumNode(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->father == z)
        {
            if (x)
            {
                x->father = y;
            }
        }
        else
        {
            transplantSRb(t, y, y->right);
            y->right = z->right;
            y->right->father = y;
        }
        transplantSRb(t, z, y);
        y->left = z->left;
        y->left->father = y;
        y->color = z->color;
    }
    if (yOriginalColor == 'b')
    {
        if (x)
        {
            deleteFixupSRb(t, x);
        }
    }
    tre->size--;

    // Method 1: create copy than free original
    // void* formToReturn = createSubstitute(z->info);
    // free(z->info);
    // free(z);
    // return formToReturn;

    // Method 2: save original->info than free original --- these one leaks memory
    Info info = z->info;
    // free(z);
    return info;
}

void printSRb(SRbTree t, char* nomeArq)
{
    FILE* dot = fopen(nomeArq, "w");
    if (dot)
    {
        tree* tre = t;

        fprintf(dot, "digraph G\n{\n");
        fprintf(dot, "\tfontname=\"Helvetica,Arial,sans-serif\"\n");
        fprintf(dot, "\tnode [fontname=\"Helvetica,Arial,sans-serif\", fontcolor=\"white\"]\n");
        fprintf(dot, "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n");

        fprintf(dot, "\n");
        recursivePrintNode(tre->root, dot);
        fprintf(dot, "\n");
        recursivePrintEdge(tre->root, dot);

        fprintf(dot, "}\n");
        fclose(dot);
    }
    else
    {
        printf("Error opening the .dot file.\n");
        exit(EXIT_FAILURE);
    }
}

void percursoLargura(SRbTree t, FvisitaNo fVisita, void* aux)
{
    tree* tre = t;
    int h = findHeight(tre->root);
    for (int i = 0; i <= h; i++)
    {
        levelOrder(tre->root, i, fVisita, aux);
    }
}

void percursoSimetrico(SRbTree t, FvisitaNo fVisita, void* aux)
{
    tree* tre = t;
    recursiveSimetrico(tre->root, fVisita, aux);
}

void percursoProfundidade(SRbTree t, FvisitaNo fVisita, void* aux)
{
    tree* tre = t;
    recursiveProfundidade(tre->root, fVisita, aux);
}

void killSRb(SRbTree t)
{
    tree* tre = t;
    recursiveFree(tre->root);
    free(tre);
}
