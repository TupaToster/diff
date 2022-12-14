DEFCMD (PLUS, 1,
{

    printFunc (file, tree, iter->left, longFunc, tex);
    fprintf (file, "+");
    printFunc (file, tree, iter->right, longFunc, tex);
},
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

    while (**skipSpaces (s) == '+') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, PLUS, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_2 (s, tree, iter->right, varName);
    }
})

DEFCMD (MINUS, 2,
{

    printFunc (file, tree, iter->left, longFunc, tex);
    fprintf (file, "-");
    printFunc (file, tree, iter->right, longFunc, tex);
},
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

    while (**skipSpaces (s) == '-') {


        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, MINUS, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_3 (s, tree, iter->right, varName);
    }
})

DEFCMD (MULT, 3,
{

    if (iter->left->type < iter->type) fprintf (file, "(");
    printFunc (file, tree, iter->left, longFunc, tex);
    if (iter->left->type < iter->type) fprintf (file, ")");

    fprintf (file, "*");

    if (iter->right->type < iter->type) fprintf (file, "(");
    printFunc (file, tree, iter->right, longFunc, tex);
    if (iter->right->type < iter->type) fprintf (file, ")");
},
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

    while (**skipSpaces (s) == '*') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, MULT, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_4 (s, tree, iter->right, varName);
    }
})

DEFCMD (DIV, 4,
{

    if (longFunc) {

        fprintf (file, "\\frac{");
        printFunc (file, tree, iter->left, longFunc, tex);
        fprintf (file, "}{");
        printFunc (file, tree, iter->right, longFunc, tex);
        fprintf (file, "}");
    }
    else {

        if (iter->left->type < iter->type) fprintf (file, "(");
        printFunc (file, tree, iter->left, longFunc, tex);
        if (iter->left->type < iter->type) fprintf (file, ")");

        fprintf (file, "/");

        if (iter->right->type < iter->type) fprintf (file, "(");
        printFunc (file, tree, iter->right, longFunc, tex);
        if (iter->right->type < iter->type) fprintf (file, ")");
    }
},
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

    while (**skipSpaces (s) == '/') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, DIV, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_5 (s, tree, iter->right, varName);
    }
})

DEFCMD (POW, 5,
{

        if (iter->left->type < iter->type) fprintf (file, "(");
        printFunc (file, tree, iter->left, longFunc, tex);
        if (iter->left->type < iter->type) fprintf (file, ")");

        if (tex) {
            fprintf (file, "^{");
            printFunc (file, tree, iter->right, longFunc, tex);
            fprintf (file, "}");
        }
        else {
            fprintf (file, "**");
            if (iter->right->type < iter->type) fprintf (file, "(");
            printFunc (file, tree, iter->right, longFunc, tex);
            if (iter->right->type < iter->type) fprintf (file, ")");
        }
},
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

    while (**skipSpaces (s) == '^') {

        tree->NodMoveLeft (iter);
        tree->NodCtor (iter->prev, POW, 0, iter->left, NULL, iter);
        tree->NodAddRight (iter);
        ++*s;
        Get_6 (s, tree, iter->right, varName);
    }
})

DEFCMD (LN, 6,
{

    if (tex) fprintf (file, "ln(");
    else fprintf (file, "np.log(");

    printFunc (file, tree, iter->left, longFunc, tex);

    fprintf (file, ")");
},
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

    if (strncmp (*skipSpaces (s), "ln", strlen ("ln")) == 0) {

        *s += strlen ("ln");
        assert (**skipSpaces (s) == '(');
        ++*s;
        tree->NodAddLeft (iter);
        tree->setNod (iter, LN, 0);
        Get_1 (s, tree, iter->left, varName);
        assert (**skipSpaces (s) == ')');
        ++*s;
    }
    else Get_7 (s, tree, iter, varName);
})

DEFCMD (SIN, 7,
{

    if (tex) fprintf (file, "sin(");
    else fprintf (file, "np.sin(");
    printFunc (file, tree, iter->left, longFunc, tex);
    fprintf (file, ")");
},
{

    sin (calc (tree, iter->left, x0))
},
{

    assert (iter->left != NULL);
    assert (iter->right == NULL);

    tree->NodMoveLeft (iter, MULT);
    tree->setNod (iter->left, COS);
    tree->NodAddRight (iter);
    tree->NodRecCpy (iter->left->left, iter->right);
    tree->setDiff (iter->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->right, outFile, varName, writeTex);
},
{

    if (strncmp (*skipSpaces (s), "sin", strlen ("sin")) == 0) {

        *s += strlen ("sin");
        assert (**skipSpaces (s) == '(');
        ++*s;
        tree->NodAddLeft (iter);
        tree->setNod (iter, SIN, 0);
        Get_1 (s, tree, iter->left, varName);
        assert (**skipSpaces (s) == ')');
        ++*s;
    }
    else Get_8 (s, tree, iter, varName);
})

DEFCMD (COS, 8,
{

    if (tex) fprintf (file, "cos(");
    else fprintf (file, "np.cos(");
    printFunc (file, tree, iter->left, longFunc, tex);
    fprintf (file, ")");
},
{

    cos (calc (tree, iter->left, x0))
},
{

    tree->NodMoveLeft (iter, MULT);
    tree->NodAddRight (iter);
    tree->NodRecCpy (iter->left->left, iter->right);
    tree->setNod (iter->left, COS);
    tree->NodMoveRight (iter->left, MINUS);
    tree->NodAddLeft (iter->left, CONSTANT, 0);
    tree->setDiff (iter->right, 1);
    if (writeTex) writeFuncTex (tree, outFile, varName);

    recDiff (tree, iter->right, outFile, varName, writeTex);
},
{

    if (strncmp (*skipSpaces (s), "cos", strlen ("cos")) == 0) {

        *s += strlen ("cos");
        assert (**skipSpaces (s) == '(');
        ++*s;
        tree->NodAddLeft (iter);
        tree->setNod (iter, SIN, 0);
        Get_1 (s, tree, iter->left, varName);
        assert (**skipSpaces (s) == ')');
        ++*s;
    }
    else Get_9 (s, tree, iter, varName);
})

DEFCMD (BRACKETS, 9, {}, {}, {},
{

    if (**skipSpaces (s) == '(') {

        ++*s;
        Get_1 (s, tree, iter, varName);
        assert (**skipSpaces (s) == ')');
        ++*s;
    }
    else Get_10 (s, tree, iter, varName);
})

DEFCMD (GETNUM, 10, {}, {}, {},
{

    if (**skipSpaces (s) == varName) {

        tree->NodCtor (iter->prev, X, 1, NULL, NULL, iter);
        ++*s;
        return;
    }
    assert (**s >= '0' and **s <= '9');
    tree->NodCtor (iter->prev, CONSTANT, strtod (*s, s), NULL, NULL, iter);
    return;
})