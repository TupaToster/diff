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
        GetE (s, tree, iter->right, varName);
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
}