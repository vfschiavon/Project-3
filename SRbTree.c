#include "libs.h"

#include "SRbTree.h"
#include "forms.h"

typedef struct node
{
    // Anchors
    double xa;
    double ya;
    // Minimun bounding boxex of figures
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

void printSRb(SRbTree t, char* nomeArq)
{
    FILE* dot = fopen(nomeArq, "w");
    if (dot)
    {
        tree* tre = t;

        // Create nil node;
        // Function that join all leafs to nil;

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

// Remove this!!!
// void percAux(node* currentnode, FvisitaNo fVisita, void* aux)
// {
//     if (currentnode) {
//         fVisita(currentnode->info, currentnode->xa, currentnode->ya, currentnode->mx1, currentnode->my1, currentnode->mx2, currentnode->my2, aux);
//         percAux(currentnode->left, fVisita, aux);
//         percAux(currentnode->right, fVisita, aux);
//     } else {
//         return;
//     }
// }

void killSRb(SRbTree t)
{
    tree* tre = t;
    recursiveFree(tre->root);
    free(tre);
}
