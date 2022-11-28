#include "diff.h"

// void simplify (Tree<Nod>* tree, Nod* iter) {

//     if (iter == NULL) return;

//     simplify (tree, iter->left);
//     simplify (tree, iter->right);

//     killXZero (tree, iter);
//     killConstants (tree, iter);
//     deXPow (tree, iter);
//     divByX (tree, iter);
//     xPower (tree, iter);
//     killNeutral (tree, iter);

// }

// void killConstants (Tree<Nod>* tree, Nod* iter) {

//     if (iter->left == NULL or iter->right == NULL) return;

//     if (iter->left->type == CONSTANT and iter->right->type == CONSTANT) {

//         tree->verifyHash ();

//         switch (iter->type){

//             case PLUS:
//                 iter->val = iter->left->val + iter->right->val;
//                 break;

//             case MINUS:
//                 iter->val = iter->left->val - iter->right->val;
//                 break;

//             case MULT:
//                 iter->val = iter->left->val * iter->right->val;
//                 break;

//             case DIV:
//                 iter->val = iter->left->val / iter->right->val;
//                 break;

//             case POWER:
//                 iter->val = pow (iter->left->val, iter->right->val);
//                 break;

//             default:
//                 printf ("Wrong nod type at nod %p\n", iter);
//                 dump((*tree));
//                 exit (-1);
//             break;
//         }

//         iter->type = CONSTANT;

//         tree->countHash ();

//         tree->NodDtorRec (iter->left);
//         tree->NodDtorRec (iter->right);

//         tree->verifyHash();
//         iter->left = NULL;
//         iter->right = NULL;
//         tree->countHash();
//     }
// }

// void xPower (Tree<Nod>* tree, Nod* iter) {

//     if (iter->left == NULL or iter->right == NULL) return;

//     if (iter->left->type != X or iter->right->type != X) return;

//     tree->verifyHash ();

//     switch (iter->type){

//         case PLUS:
//             if (iter->left->val == iter->right->val) {

//                 iter->type = MULT;
//                 iter->left->type = CONSTANT;
//                 iter->left->val = 2;
//                 tree->countHash ();
//             }
//             return;

//         case MINUS:
//             if (iter->left->val == iter->right->val) {

//                 iter->type = CONSTANT;
//                 iter->val = 0;
//                 tree->countHash();
//                 tree->NodDtorRec (iter->left);
//                 tree->NodDtorRec (iter->right);
//                 tree->countHash ();
//             }
//             return;

//         case MULT:
//             iter->type = X;
//             iter->val = iter->left->val + iter->right->val;
//             tree->countHash();
//             tree->NodDtorRec (iter->left);
//             tree->NodDtorRec (iter->right);
//             tree->countHash ();
//             return;

//         case DIV:
//             iter->type = X;
//             iter->val = iter->left->val - iter->right->val;
//             tree->countHash();
//             tree->NodDtorRec (iter->left);
//             tree->NodDtorRec (iter->right);
//             tree->countHash ();
//             return;

//         default:
//             printf ("Wrong nod type at nod %p\n", iter);
//             dump((*tree));
//             exit (-1);
//         break;
//     }
// }

// void divByX (Tree<Nod>* tree, Nod* iter) {

//     if (iter->left == NULL or iter->right == NULL) return;

//     if (iter->left->type != CONSTANT or iter->right->type != X or iter->type != DIV) return;

//     iter->type = MULT;
//     iter->left->val = -iter->left->val;
// }

// void deXPow (Tree<Nod>* tree, Nod* iter) {

//     if (iter->left == NULL or iter->right == NULL) return;

//     if (iter->left->type != X or iter->right->type != CONSTANT or iter->type != POWER) return;

//     iter->type = X;
//     iter->val = iter->right->val * iter->left->val;
//     tree->countHash ();

//     tree->NodDtorRec (iter->left);
//     tree->NodDtorRec (iter->right);

//     tree->verifyHash ();
//     iter->left = NULL;
//     iter->right = NULL;
//     tree->countHash ();
// }

// void killNeutral (Tree<Nod>* tree, Nod* iter) {

//     if (iter->type == PLUS or iter->type == MINUS) {

//         if (iter->left->type == CONSTANT and iter->left->val == 0) {

//             tree->NodDtorRec (iter->left);
//             Nod* temp = iter->right;
//             tree->TreeCpy (iter, temp);
//             temp->prev = NULL;
//             tree->NodDtorRec (temp);
//         }
//         else if (iter->right->type == CONSTANT and iter->right->val == 0) {

//             tree->NodDtorRec (iter->right);
//             Nod* temp = iter->left;
//             tree->TreeCpy (iter, temp);
//             temp->prev = NULL;
//             tree->NodDtorRec (temp);
//         }
//     }
//     else if (iter->type == MULT) {

//         if (iter->left->type == CONSTANT and iter->left->val == 1) {

//             tree->NodDtorRec (iter->left);
//             Nod* temp = iter->right;
//             tree->TreeCpy (iter, temp);
//             temp->prev = NULL;
//             tree->NodDtorRec (temp);
//         }
//         else if (iter->right->type == CONSTANT and iter->right->val == 1) {

//             tree->NodDtorRec (iter->right);
//             Nod* temp = iter->left;
//             tree->TreeCpy (iter, temp);
//             temp->prev = NULL;
//             tree->NodDtorRec (temp);
//         }
//         else if (iter->left->type == CONSTANT and iter->left->val == 0 or
//                  iter->right->type == CONSTANT and iter->right->val == 0) {

//             tree->NodDtorRec (iter->left);
//             tree->NodDtorRec (iter->right);
//             tree->NodCtor (iter->prev, CONSTANT, 0, NULL, NULL, iter);
//         }
//     }
//     else if (iter->type == DIV) {

//         if (iter->left->type == CONSTANT and iter->left->val == 0) {

//             tree->NodDtorRec (iter->left);
//             tree->NodDtorRec (iter->right);
//             tree->NodCtor (iter->prev, CONSTANT, 0, NULL, NULL, iter);
//         }
//     }
//     else if (iter->type == POWER) {

//         if (iter->right->type == CONSTANT and iter->right->val == 0) {

//             tree->NodCtor (iter->prev, CONSTANT, 1, iter->left, iter->right, iter);
//             tree->NodDtorRec (iter->left);
//             tree->NodAddLeft (iter->right);
//         }
//         else if (iter->left->type == CONSTANT and (iter->left->val == 1 or iter->left->val == 0)) {

//             tree->NodCtor (iter->prev, CONSTANT, iter->left->val == 1 ? 1 : 0, iter->left, iter->right, iter);
//             tree->NodDtorRec (iter->left);
//             tree->NodDtorRec (iter->right);
//         }
//     }
// }

// void killXZero (Tree<Nod>* tree, Nod* iter) {

//     if (iter->type == X and iter->val == 0) {

//         tree->verifyHash ();
//         iter->type = CONSTANT;
//         iter->val = 1;
//         tree->countHash ();
//     }
// }

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

Tree<Nod> functionReader (char* function, const char varName) {

    Tree<Nod> tree;
    GetG (function, &tree, varName);
    return tree;
}

void GetG (char* function, Tree<Nod>* tree, const char varName) {

    char* s = function;
    GetPlus (&s, tree, tree->getdata (), varName);
    assert (*s == '\0');
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

        iter->type = X;
        iter->val = 1;
        ++*s;
        return;
    }
    assert (**s >= '0' and **s <= '9');
    iter->type = CONSTANT;
    iter->val = strtod (*s, s);
    return;
}