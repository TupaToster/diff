#define DEFCMD(name, priority, calc, diff, get) ;

DEFCMD (PLUS, 1,
{

    calc (tree, iter->left, x0) + calc (tree, iter->right, x0)
},
{

    tree->setDiff (iter->left, 1);
    tree->setDiff (iter->right, 1);
    writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left, outFile, varName);
    recDiff (tree, iter->right, outFile, varName);
},
{

    Get_2 (s, tree, iter, varName);

    while (**s == '+') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, PLUS, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_2 (s, tree, iter->right, varName);
    }
})

DEFCMD (MINUS, 2,
{

    calc (tree, iter->left, x0) - calc (tree, iter->right, x0)
},
{
    tree->setDiff (iter->left, 1);
    tree->setDiff (iter->right, 1);
    writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left, outFile, varName);
    recDiff (tree, iter->right, outFile, varName);
},
{

    Get_3 (s, tree, iter, varName);

    while (**s == '-') {


        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, MINUS, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_3 (s, tree, iter->right, varName);
    }
})

DEFCMD (MULT, 3,
{

    calc (tree, iter->left, x0) * calc (tree, iter->right, x0)
},
{

    tree->NodMoveLeft (iter, PLUS, 0);
    tree->NodAddRight (iter);
    tree->NodRecCpy (iter->left, iter->right);

    tree->setDiff (iter->left->left, 1);
    tree->setDiff (iter->right->right, 1);
    writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left->left, outFile, varName);
    recDiff (tree, iter->right->right, outFile, varName);
},
{

    Get_4 (s, tree, iter, varName);

    while (**s == '*') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, MULT, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_4 (s, tree, iter->right, varName);
    }
})

DEFCMD (DIV, 4,
{

    calc (tree, iter->left, x0) / calc (tree, iter->right, x0)
},
{

    tree->NodMoveLeft (iter, DIV, 0);
    tree->NodAddRight (iter, POWER, 0);
    tree->NodAddLeft (iter->right);
    tree->NodAddRight (iter->right, CONSTANT, 2);
    tree->NodRecCpy (iter->left->right, iter->right->left);

    tree->setNod (iter->left, MULT, 0);
    tree->NodMoveLeft (iter->left, MINUS, 0);
    tree->NodAddRight (iter->left);
    tree->NodRecCpy (iter->left->left, iter->left->right);

    tree->setDiff (iter->left->left->left, 1);
    tree->setDiff (iter->rleft->right->right, 1);
    writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left->left->left, outFile, varName);
    recDiff (tree, iter->left->right->right, outFile, varName);
},
{

    Get_5 (s, tree, iter, varName);

    while (**s == '/') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, DIV, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_5 (s, tree, iter->right, varName);
    }
})

DEFCMD (POW, 5,
{

    pow (calc (tree, iter->left, x0), calc (tree, iter->right, x0))
},
{

    tree->NodMoveRight (iter, MULT);
    tree->NodAddLeft (iter, LN);
    tree->NodAddLeft (iter->left, CONSTANT, iter->right->left->val);
    tree->NodMoveLeft (iter->right, MULT);
    tree->NodAddRight (iter->right);
    tree->NodRecCpy (iter->right->right, iter->right->left->right);
    tree->setDiff (iter->right->right, 1);
    writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->right, right);
},
{

    Get_6 (s, tree, iter, varName);

    while (**s == '^') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, POWER, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_6 (s, tree, iter->right, varName);
    }
})

DEFCMD (LN, 6,
{

    log (calc (tree, iter->left, x0))
},
{

    tree->NodAddLeft (iter, CONSTANT, 1);
    tree->setNod (iter, DIV);
    tree->NodMoveLeft (iter, MULT);
    tree->NodAddRight (iter);
    tree->NodRecCpy (iter->left->right, iter->right);
    tree->setDiff (iter->right, 1);
    writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->right);
},
{

    if (strncmp (*s, "ln(", 3) == 0) {

        *s+=3;
        tree->setNod (iter, LN, 0);
        tree->NodAddLeft (iter);
        Get_1 (s, tree, iter->left, varName);
        assert (**s == ')');
        ++*s;
    }
    else GetNum (s, tree, iter, varName);
})