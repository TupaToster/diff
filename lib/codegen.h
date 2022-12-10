DEFCMD (PLUS, 1,
{

    calc (tree, iter->left, x0) + calc (tree, iter->right, x0)
},
{

    assert (iter->left != NULL);
    assert (iter->right != NULL);
    tree->setDiff (iter->left, 1);
    tree->setDiff (iter->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left, outFile, varName, writeTex);
    recDiff (tree, iter->right, outFile, varName, writeTex);
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

    assert (iter->left != NULL);
    assert (iter->right != NULL);
    tree->setDiff (iter->left, 1);
    tree->setDiff (iter->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left, outFile, varName, writeTex);
    recDiff (tree, iter->right, outFile, varName, writeTex);
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

    assert (iter->left != NULL);
    assert (iter->right != NULL);
    tree->NodMoveLeft (iter, PLUS, 0);
    tree->NodAddRight (iter);
    tree->NodRecCpy (iter->left, iter->right);

    tree->setDiff (iter->left->left, 1);
    tree->setDiff (iter->right->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left->left, outFile, varName, writeTex);
    recDiff (tree, iter->right->right, outFile, varName, writeTex);
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

    assert (iter->left != NULL);
    assert (iter->right != NULL);
    tree->NodMoveLeft (iter, DIV, 0);
    tree->NodAddRight (iter, POW, 0);
    tree->NodAddLeft (iter->right);
    tree->NodAddRight (iter->right, CONSTANT, 2);
    tree->NodRecCpy (iter->left->right, iter->right->left);

    tree->setNod (iter->left, MULT, 0);
    tree->NodMoveLeft (iter->left, MINUS, 0);
    tree->NodAddRight (iter->left);
    tree->NodRecCpy (iter->left->left, iter->left->right);

    tree->setDiff (iter->left->left->left, 1);
    tree->setDiff (iter->left->right->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->left->left->left, outFile, varName, writeTex);
    recDiff (tree, iter->left->right->right, outFile, varName, writeTex);
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

    assert (iter->left != NULL);
    assert (iter->right != NULL);

    if (iter->left->type == CONSTANT) {

        tree->NodMoveRight (iter, MULT);
        tree->NodAddLeft (iter, LN);
        tree->NodAddLeft (iter->left, CONSTANT, iter->right->left->val);
        tree->NodMoveLeft (iter->right, MULT);
        tree->NodAddRight (iter->right);
        tree->NodRecCpy (iter->right->left->right, iter->right->right);
    }

    tree->setDiff (iter->right->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->right->right, outFile, varName);
},
{

    Get_6 (s, tree, iter, varName);

    while (**s == '^') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, POW, 0, iter->left, NULL, iter);
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

    assert (iter->left != NULL);
    assert (iter->right == NULL);

    tree->NodCtor (iter->prev, iter->type, iter->val, NULL, iter->left, iter);
    tree->NodAddLeft (iter, CONSTANT, 1);
    tree->setNod (iter, DIV);
    tree->NodMoveLeft (iter, MULT);
    tree->NodAddRight (iter);
    tree->NodRecCpy (iter->left->right, iter->right);
    tree->setDiff (iter->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->right, outFile, varName, writeTex);
},
{

    if (strncmp (*s, "ln(", 3) == 0) {

        *s+=3;
        tree->NodAddLeft (iter);
        tree->setNod (iter, LN, 0);
        Get_1 (s, tree, iter->left, varName);
        assert (**s == ')');
        ++*s;
    }
    else Get_7 (s, tree, iter, varName);
})

DEFCMD (BRACKETS, 7, {}, {},
{

    if (**s == '(') {

        ++*s;
        Get_1 (s, tree, iter, varName);
        assert (**s == ')');
        ++*s;
    }
    else Get_8 (s, tree, iter, varName);
})

DEFCMD (GETNUM, 8, {}, {},
{

    if (**s == varName) {

        tree->NodCtor (iter->prev, X, 1, NULL, NULL, iter);
        ++*s;
        return;
    }
    assert (**s >= '0' and **s <= '9');
    tree->NodCtor (iter->prev, CONSTANT, strtod (*s, s), NULL, NULL, iter);
    return;
})