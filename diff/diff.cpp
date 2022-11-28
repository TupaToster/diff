#include "diff.h"

void simplify (Tree<Nod>* tree, Nod* iter) {

    if (iter == NULL) return;

    simplify (tree, iter->left);
    simplify (tree, iter->right);

    killConstants (tree, iter);
    killXZero (tree, iter);
    flog (iter);
    dump (*tree);
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
            exit (-1);
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

// Tree<Nod> differentiate (Tree<Nod>* tree) {

//     Tree<Nod> diff;
//     recDiff (&diff, diff.getdata (), tree, tree->getdata ());
//     simplify (&diff, diff.getdata ());
//     return diff;
// }

// void recDiff (Tree<Nod>* derivative, Nod* dIter, Tree<Nod>* func, Nod* fIter) {

//     if (fIter == NULL) return;

//     flog (dIter);
//     dump (*derivative);
//     if (fIter->left != NULL and fIter->right != NULL) {

//         derivative->NodAddLeft (dIter);
//         derivative->NodAddRight (dIter);
//         recDiff (derivative, dIter->left, func, fIter->left);
//         recDiff (derivative, dIter->right, func, fIter->right);

//         derivative->verifyHash ();
//         dIter->type = fIter->type;
//         derivative->countHash ();

//         if (dIter->type == PLUS or dIter->type == MINUS) return;
//         if (dIter->type == MULT) {

//             if (dIter->left->type == CONSTANT and dIter->right->type != CONSTANT) {

//                 derivative->verifyHash ();
//                 dIter->left->val = fIter->left->val;
//                 derivative->countHash ();
//             }
//             else if (dIter->left->type != CONSTANT and dIter->right->type == CONSTANT) {

//                 derivative->verifyHash ();
//                 dIter->right->val = fIter->right->val;
//                 derivative->countHash ();
//             }
//             else if (dIter->left->type != CONSTANT and dIter->right->type != CONSTANT) {

//                 derivative->TreeMoveToLeft (dIter->left, MULT, 0);
//                 derivative->NodAddRight (dIter->left);
//                 derivative->TreeCpy (dIter->left, fIter->left);

//                 derivative->NodCtor (dIter->prev, PLUS, 0, dIter->left, dIter->right, dIter);

//                 derivative->TreeMoveToLeft (dIter->right, MULT, 0);
//                 derivative->NodAddRight (dIter->right);
//                 derivative->TreeCpy (dIter->right, fIter->right);
//             }
//         }
//         else if (dIter->type == DIV) {

//             if (dIter->left->type == CONSTANT and dIter->right->type != CONSTANT) {

//                 derivative->verifyHash ();
//                 dIter->left->val = fIter->left->val;
//                 derivative->countHash ();
//             }
//             else if (dIter->left->type != CONSTANT and dIter->right->type == CONSTANT) {

//                 derivative->verifyHash ();
//                 dIter->right->val = dIter->right->val;
//                 derivative->countHash ();
//             }
//             else if (dIter->left->type != CONSTANT and dIter->right->type != CONSTANT) {

//                 derivative->TreeMoveToLeft (dIter->left, MINUS, 0);
//                 derivative->TreeMoveToLeft (dIter->left->left, MULT, 0);
//                 derivative->NodAddRight (dIter->left->left);
//                 derivative->TreeCpy (dIter->left->left->right, fIter->left);

//                 derivative->NodAddRight (dIter->left);
//                 derivative->TreeCpy (dIter->left->right, dIter->right);
//                 derivative->TreeMoveToLeft (dIter->left->right, MULT, 0);
//                 derivative->NodAddRight (dIter->left->right);
//                 derivative->TreeCpy (dIter->left->right, fIter->right);

//                 derivative->TreeMoveToLeft (dIter->right, POWER, 0);
//                 derivative->NodAddRight (dIter->right, CONSTANT, 2);
//             }
//         }
//     }
//     else {

//         if (fIter->type == X) {

//             derivative->NodAddLeft (dIter, CONSTANT, fIter->val);
//             derivative->NodAddRight (dIter, X, fIter->val - 1);
//             derivative->verifyHash ();
//             dIter->type = MULT;
//             dIter->val = 0;
//             derivative->countHash ();
//         }
//         else if (fIter->type == CONSTANT) {

//             derivative->verifyHash ();
//             dIter->type = CONSTANT;
//             dIter->val = 0;
//             derivative->countHash ();
//         }
//         else {

//             flogprintf ("!!!!!!!!!!!!!!!!!!!!!!!!Something very wrong\n");
//             flog (dIter);
//             flog (fIter);
//             dump (*derivative);
//             dump (*func);
//             //exit (-1);
//         }
//     }
// }

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