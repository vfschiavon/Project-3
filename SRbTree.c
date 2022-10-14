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
    x->right = y->left;
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

void rightRotate(SRbTree t, node* x)
{
    node* y = x->left;
    x->left = y->right;
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

node* searchNode(node* currentnode, double pXa, double pYa)
{
    node* result = NULL;
    if (currentnode)
    {
        if (currentnode->xa == pXa && currentnode->ya == pYa)
        {
            result = currentnode;
        }
        else
        {
            result = searchNode(currentnode->left, pXa, pYa);
            if (!result)
            {
                result = searchNode(currentnode->right, pXa, pYa);
            }
        }
    }
    else
    {
        return NULL;
    }
    return result;
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
            if (w->left->color == 'b' && w->right->color == 'b')
            {
                w->color = 'r';
                x = x->father;
            }
            else
            {
                if (w->right->color == 'b')
                {
                    w->left->color = 'b';
                    w->color = 'r';
                    rightRotate(t, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = 'b';
                w->right->color = 'b';
                leftRotate(t, x->father);
                x = t->root;
            }
        }
        else
        {
            w = x->father->left;
            if (w->color == 'r')
            {
                w->color = 'b';
                x->father->color = 'r';
                rightRotate(t, x->father);
                w = x->father->left;
            }
            if (w->right->color == 'b' && w->left->color == 'b')
            {
                w->color = 'r';
                x = x->father;
            }
            else
            {
                if (w->left->color == 'b')
                {
                    w->right->color = 'b';
                    w->color = 'r';
                    leftRotate(t, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = 'b';
                w->left->color = 'b';
                rightRotate(t, x->father);
                x = t->root;
            }
        }
    }
    x->color = 'b';
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
    node* result = searchNode(tre->root, xa, ya);
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
    node* z = searchNode(tre->root, xa, ya);
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
            x->father = y;
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
        deleteFixupSRb(t, x);
    }

    // Doing right?????
    printf("z->info: %d\n", getFormId(z->info));
    Info info = z->info;
    free(z);
    tre->size--;
    printf("info: %d\n", getFormId(info));

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
