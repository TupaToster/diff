#include "diff.h"

Tree<Nod> functionReader (char* function, char varName) {

    Tree<Nod> tree;
    GetG (function, &tree, varName);
    return tree;
}

void GetG (char* function, Tree<Nod>* tree, char varName) {

    char* s = function;
    GetE (&s, tree, tree->getdata (), varName);
    assert (*s == '\0');
}

void GetE (char** s, Tree<Nod>* tree, Nod* iter, char varName) {

    GetT (s, tree, iter, varName);

    if (**s == '+' or **s == '-') {

        tree->NodAddLeft (iter, iter->type, iter->val, iter->left, iter->right);
        assert (iter->left != NULL);
        tree->NodCtor (iter->prev, **s == '+' ? PLUS : MINUS, 0, iter->left, NULL, iter);
        assert (iter != NULL);
        tree->NodAddRight (iter);
        ++*s;
        GetE (s, tree, iter->right, varName);
    }
}

void GetT (char** s, Tree<Nod>* tree, Nod* iter, char varName) {

    GetP (s, tree, iter, varName);

    if (**s == '*' or **s == '/') {

        tree->NodAddLeft (iter, iter->type, iter->val, iter->left, iter->right);
        assert (iter->left != NULL);
        tree->NodCtor (iter->prev, **s == '*' ? MULT : DIV, 0, iter->left, NULL, iter);
        assert (iter != NULL);
        tree->NodAddRight (iter);
        ++*s;
        GetT (s, tree, iter->right, varName);
    }
}

void GetP (char** s, Tree<Nod>* tree, Nod* iter, char varName) {

    if (**s == '(') {

        ++*s;
        GetE (s, tree, iter, varName);
        assert (**s == ')');
        ++*s;
    }
    else GetN (s, tree, iter, varName);
}

void GetN (char** s, Tree<Nod>* tree, Nod* iter, char varName) {

    double val = 0;
    if (**s == varName) {

        tree->verifyHash ();
        iter->type = X;
        iter->val = 1;
        tree->countHash ();
        ++*s;
        return;
    }
    assert (isalnum (**s) and !isalpha (**s));
    while (isalnum (**s) and !isalpha (**s)) {

        val = 10* val + (double) (**s - '0');
        ++*s;
    }
    tree->verifyHash ();
    iter->type = CONSTANT;
    iter->val = val;
    tree->countHash ();
    dump ((*tree));
}

void simplify (Tree<Nod>* tree, Nod* iter) {

    if (iter == NULL) return;

    simplify (tree, iter->left);
    simplify (tree, iter->right);

    killConstants (tree, iter);
    xPower (tree, iter);

}

void killConstants (Tree<Nod>* tree, Nod* iter) {

    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->left->type == CONSTANT and iter->right->type == CONSTANT) {

        tree->verifyHash ();

        switch (iter->type){

            case PLUS:
                iter->val = iter->left->val + iter->right->val;
                break;

            case MINUS:
                iter->val = iter->left->val - iter->right->val;
                break;

            case MULT:
                iter->val = iter->left->val * iter->right->val;
                break;

            case DIV:
                iter->val = iter->left->val / iter->right->val;
                break;

            default:
                printf ("Wrong nod type at nod %p\n", iter);
                dump((*tree));
                exit (-1);
            break;
        }

        iter->type = CONSTANT;

        tree->countHash ();
        tree->NodDtorRec (iter->left);
        tree->NodDtorRec (iter->right);
        dump ((*tree));
    }
}

void xPower (Tree<Nod>* tree, Nod* iter) {

    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->left->type == X and iter->left->type == X) {

        tree->verifyHash ();

        switch (iter->type){

            case PLUS:
                if (iter->left->val == iter->right->val) {

                    iter->type = MULT;
                    iter->left->type = CONSTANT;
                    iter->left->val = 2;
                    tree->countHash ();
                }
                return;

            case MINUS:
                if (iter->left->val == iter->right->val) {

                    iter->type = CONSTANT;
                    iter->val = 0;
                    tree->NodDtorRec (iter->left);
                    tree->NodDtorRec (iter->right);
                    iter->left = NULL;
                    iter->right = NULL;
                    tree->countHash ();
                }
                return;

            case MULT:
                iter->type = X;
                iter->val = iter->left->val + iter->right->val;
                tree->NodDtorRec (iter->left);
                tree->NodDtorRec (iter->right);
                iter->left = NULL;
                iter->right = NULL;
                tree->countHash ();
                return;

            case DIV:
                iter->type = X;
                iter->val = iter->left->val - iter->right->val;
                tree->NodDtorRec (iter->left);
                tree->NodDtorRec (iter->right);
                iter->left = NULL;
                iter->right = NULL;
                tree->countHash ();
                return;

            default:
                printf ("Wrong nod type at nod %p\n", iter);
                dump((*tree));
                exit (-1);
            break;
        }
    }
}

