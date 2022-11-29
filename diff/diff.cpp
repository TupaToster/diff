#include "diff.h"

void simplify (Tree<Nod>* tree, Nod* iter) {

    if (iter == NULL) return;

    simplify (tree, iter->left);
    simplify (tree, iter->right);

    killConstants (tree, iter);
    killXZero (tree, iter);
    deXPow (tree, iter);
    divByX (tree, iter);
    xPower (tree, iter);
    killNeutral (tree, iter);
}

void killConstants (Tree<Nod>* tree, Nod* iter) {

    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->left->type == CONSTANT and iter->right->type == CONSTANT) {

        switch (iter->type){

            case PLUS:
                tree->NodCtor (iter->prev, CONSTANT, iter->left->val + iter->right->val, iter->left, iter->right, iter);
                break;

            case MINUS:
                tree->NodCtor (iter->prev, CONSTANT, iter->left->val - iter->right->val, iter->left, iter->right, iter);
                break;

            case MULT:
                tree->NodCtor (iter->prev, CONSTANT, iter->left->val * iter->right->val, iter->left, iter->right, iter);
                break;

            case DIV:
                tree->NodCtor (iter->prev, CONSTANT, iter->left->val / iter->right->val, iter->left, iter->right, iter);
                break;

            case POWER:
                tree->NodCtor (iter->prev, CONSTANT, pow (iter->left->val, iter->right->val), iter->left, iter->right, iter);
                break;

            default:
                printf ("Wrong nod type at nod %p\n", iter);
                dump((*tree));
                assert (iter->type == BLANK);
            break;
        }

        tree->NodDtorRec (iter->left);
        tree->NodDtorRec (iter->right);
    }
}

void xPower (Tree<Nod>* tree, Nod* iter) {

    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->left->type != X or iter->right->type != X) return;

    switch (iter->type){

        case PLUS:

            if (iter->left->val == iter->right->val) {

                tree->NodCtor (iter->prev, MULT, 0, iter->left, iter->right, iter);
                tree->NodCtor (iter, CONSTANT, 2, iter->left->left, iter->left->right, iter->left);
            }
        return;

        case MINUS:

            if (iter->left->val == iter->right->val) {

                tree->NodCtor (iter->prev, CONSTANT, 0, iter->left, iter->right, iter);
                tree->NodDtorRec (iter->left);
                tree->NodDtorRec (iter->right);
            }
        return;

        case MULT:

            tree->NodCtor (iter->prev, X, iter->left->val + iter->right->val, iter->left, iter->right, iter);
            tree->NodDtorRec (iter->left);
            tree->NodDtorRec (iter->right);
        return;

        case DIV:

            tree->NodCtor (iter->prev, X, iter->left->val - iter->right->val, iter->left, iter->right, iter);
            tree->NodDtorRec (iter->left);
            tree->NodDtorRec (iter->right);
        return;

        default:

            printf ("Wrong nod type at nod %p\n", iter);
            dump((*tree));
            assert (iter->type == BLANK);
        break;
    }
}

void divByX (Tree<Nod>* tree, Nod* iter) {

    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->type == DIV and iter->right->type == X) {

        tree->NodCtor (iter->prev, MULT, 0, iter->left, iter->right, iter);
        tree->NodCtor (iter, X, -iter->right->val, iter->right->left, iter->right->right, iter->right);
    }
}

void deXPow (Tree<Nod>* tree, Nod* iter) {

    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->left->type == X and iter->right->type == CONSTANT and iter->type == POWER) {

        tree->NodCtor (iter->prev, X, iter->right->val * iter->left->val, iter->left, iter->right, iter);

        tree->NodDtorRec (iter->left);
        tree->NodDtorRec (iter->right);
    }
}

void killNeutral (Tree<Nod>* tree, Nod* iter) {

    if (iter->type == PLUS) {

        if (iter->left->type == CONSTANT and iter->left->val == 0) {

            tree->NodDtorRec (iter->left);
            tree->NodUnMoveRight (iter);
        }
        else if (iter->right->type == CONSTANT and iter->right->val == 0) {

            tree->NodDtorRec (iter->right);
            tree->NodUnMoveLeft (iter);
        }
    }
    else if (iter->type == MULT) {

        if (iter->left->type == CONSTANT and iter->left->val == 1) {

            tree->NodDtorRec (iter->left);
            tree->NodUnMoveRight (iter);
        }
        else if (iter->right->type == CONSTANT and iter->right->val == 1) {

            tree->NodDtorRec (iter->right);
            tree->NodUnMoveLeft (iter);
        }
        else if (iter->left->type == CONSTANT and iter->left->val == 0 or
                 iter->right->type == CONSTANT and iter->right->val == 0) {

            tree->NodDtorRec (iter->left);
            tree->NodDtorRec (iter->right);
            tree->NodCtor (iter->prev, CONSTANT, 0, NULL, NULL, iter);
        }
    }
    else if (iter->type == DIV) {

        if (iter->left->type == CONSTANT and iter->left->val == 0) {

            tree->NodDtorRec (iter->left);
            tree->NodDtorRec (iter->right);
            tree->NodCtor (iter->prev, CONSTANT, 0, NULL, NULL, iter);
        }
    }
    else if (iter->type == POWER) {

        if (iter->right->type == CONSTANT and iter->right->val == 0) {

            tree->NodCtor (iter->prev, CONSTANT, 1, iter->left, iter->right, iter);
            tree->NodDtorRec (iter->left);
            tree->NodDtorRec (iter->right);
        }
        else if (iter->left->type == CONSTANT and (iter->left->val == 1 or iter->left->val == 0)) {

            tree->NodCtor (iter->prev, CONSTANT, iter->left->val == 1 ? 1 : 0, iter->left, iter->right, iter);
            tree->NodDtorRec (iter->left);
            tree->NodDtorRec (iter->right);
        }
    }
}

void killXZero (Tree<Nod>* tree, Nod* iter) {

    if (iter->type == X and iter->val == 0) tree->NodCtor (iter->prev, CONSTANT, 1, NULL, NULL, iter);
}

Tree<Nod> differentiate (Tree<Nod>* tree) {

    Tree<Nod> diff;
    diff.NodRecCpy (tree->getdata (), diff.getdata ());
    recDiff (&diff, diff.getdata (), tree, tree->getdata ());
    simplify (&diff, diff.getdata ());
    return diff;
}

void recDiff (Tree<Nod>* tree, Nod* iter, Tree<Nod>* func, Nod* fIter) {

    if (iter == NULL) return;

    if (iter->left != NULL and iter->right != NULL) {

        recDiff (tree, iter->left, func, fIter->left);
        recDiff (tree, iter->right, func, fIter->right);

        if (iter->type == PLUS or iter->type == MINUS) return;

        if (iter->type == MULT) {

            if (iter->left->type == CONSTANT and iter->right->type != CONSTANT) tree->NodCtor (iter, fIter->left->type, fIter->left->val, fIter->left->left, fIter->left->right, iter->left);
            else if (iter->left->type != CONSTANT and iter->right->type == CONSTANT) tree->NodCtor (iter, fIter->right->type, fIter->right->val, fIter->right->left, fIter->right->right, iter->right);
            else {

                tree->NodMoveLeft (iter->left, MULT, 0);
                tree->NodAddRight (iter->left);
                tree->NodRecCpy (fIter->left, iter->left->right);

                tree->NodCtor (iter->prev, PLUS, 0, iter->left, iter->right, iter);

                tree->NodMoveLeft (iter->right, MULT, 0);
                tree->NodAddRight (iter->right);
                tree->NodRecCpy (fIter->right, iter->right->right);
            }
        }
        else if (iter->type = DIV) {

            if (iter->left->type == CONSTANT and iter->right->type != CONSTANT) tree->NodCtor (iter, iter->left->type, iter->left->val, iter->left->left, iter->left->right, iter);
            else if (iter->left->type != CONSTANT and iter->right->type == CONSTANT) tree->NodCtor (iter, iter->right->type, iter->right->val, iter->right->left, iter->right->right, iter);
            else {

                tree->NodMoveLeft (iter->left, MINUS, 0);
                tree->NodMoveLeft (iter->left->left, MULT, 0);
                tree->NodAddRight (iter->left->left);
                tree->NodRecCpy (fIter->left, iter->left->left->right);

                tree->NodAddRight (iter->left);
                tree->NodRecCpy (iter->right, iter->left->right);
                tree->NodMoveLeft (iter->left->right, MULT, 0);
                tree->NodAddRight (iter->left->right);
                tree->NodRecCpy (fIter->right, iter->left->right->right);



                tree->NodMoveLeft (iter->right, POWER, 0);
                tree->NodAddRight (iter->right, CONSTANT, 2);
            }
        }
    }
    else {

        if (iter->type == X) {

            tree->NodAddLeft (iter, CONSTANT, iter->val);
            tree->NodAddRight (iter, X, iter->val - 1);
            tree->NodCtor (iter->prev, MULT, 0, iter->left, iter->right, iter);
        }
        else tree->NodCtor (iter->prev, CONSTANT, 0, NULL, NULL, iter);
    }
}

void writeTeX (Tree<Nod>* derivative, Nod* dIter, Tree<Nod>* func, Nod* fIter) {

    static int actionCounter = 0;

    const char* fileName = "derivative.tex";
    FILE* outFile  = fopen (fileName, "a");

    if (actionCounter == 0) {

        fprintf (outFile, "\\documentclass[a4paper]{article}" "\n");
        fprintf (outFile, "\\usepackage[a4paper,top=2cm,bottom=2cm,left=0.5cm,right=0.5cm,marginparwidth=1.75cm]{geometry}" "\n");
        fprintf (outFile, "\\usepackage[]{graphicx}" "\n");
        fprintf (outFile, "\\usepackage[]{wrapfig}" "\n");
        fprintf (outFile, "\\usepackage[T2A]{fontenc}" "\n");
        fprintf (outFile, "\\usepackage[utf8]{inputenc}" "\n");
        fprintf (outFile, "\\usepackage[english, russian]{babel}" "\n");
        fprintf (outFile, "\\usepackage[]{amsmath,amsfonts,amssymb,amsthm,mathtools}" "\n");
        fprintf (outFile, "\\usepackage[]{wasysym}" "\n");
        fprintf (outFile, "\\usepackage[]{float}" "\n");
        fprintf (outFile, "\\usepackage{multicol}" "\n");
        fprintf (outFile, "\\usepackage{mathtext}" "\n");
        fprintf (outFile, "\\usepackage{amsmath}" "\n");
        fprintf (outFile, "\\usepackage{amsfonts}" "\n");
        fprintf (outFile, "\\usepackage{indentfirst}" "\n");
        fprintf (outFile, "\\usepackage{longtable}" "\n");
        fprintf (outFile, "\\usepackage{natbib}" "\n");
        fprintf (outFile, "\\usepackage{mathrsfs}" "\n");
        fprintf (outFile, "\\title{The legendary derivative}" "\n");
        fprintf (outFile, "\\date{\\today}" "\n");
        fprintf (outFile, "\\begin{document}" "\n");
        fprintf (outFile, "\\maketitle" "\n");
    }

}

Tree<Nod> GetG (char* function, const char varName) {

    Tree<Nod> tree;
    char* s = function;
    GetPlus (&s, &tree, tree.getdata (), varName);
    assert (*s == '\0');
    return tree;
}

void GetPlus (char** s, Tree<Nod>* tree, Nod* iter, const char varName) {

    GetMult (s, tree, iter, varName);

    while (**s == '+' or **s == '-') {


        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, **s == '+' ? PLUS : MINUS, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        GetMult (s, tree, iter->right, varName);
    }
}

void GetMult (char** s, Tree<Nod>* tree, Nod* iter, const char varName) {

    GetPower (s, tree, iter, varName);

    while (**s == '*' or **s == '/') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, **s == '*' ? MULT : DIV, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        GetPower (s, tree, iter->right, varName);
    }
}

void GetPower (char** s, Tree<Nod>* tree, Nod* iter, const char varName) {

    GetP (s, tree, iter, varName);

    while (**s == '^') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, POWER, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        GetP (s, tree, iter->right, varName);
    }
}

void GetP (char** s, Tree<Nod>* tree, Nod* iter, const char varName) {

    if (**s == '(') {

        ++*s;
        GetPlus (s, tree, iter, varName);
        assert (**s == ')');
        ++*s;
        return;
    }
    else GetNum (s, tree, iter, varName);
}

void GetNum (char** s, Tree<Nod>* tree, Nod* iter, const char varName) {

    if (**s == varName) {

        tree->NodCtor (iter->prev, X, 1, NULL, NULL, iter);
        ++*s;
        return;
    }
    assert (**s >= '0' and **s <= '9');
    tree->NodCtor (iter->prev, CONSTANT, strtod (*s, s), NULL, NULL, iter);
    return;
}