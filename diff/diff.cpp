#include "diff.h"


void researchFunc (char* function, const char* fileName, char varName, double x0) {

    assert (function != NULL);
    assert (fileName != NULL);

    FILE* outFile = fopen (fileName, "w");
    assert (outFile != NULL);

    if (true) {

        int unix_time = 0;
        int sec = 0;
        int min = 0;
        int hrs = 0;

        sscanf (__TIME__, "%d%*c%d%*c%d", &hrs, &min, &sec);
        unix_time = hrs*3600 + min*60 + sec;
        srand (unix_time);
    }

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
    fprintf (outFile, "\\title{Исследование неизвестной на момент заголовка функции}" "\n");
    fprintf (outFile, "\\date{\\today}" "\n");
    fprintf (outFile, "\\begin{document}" "\n");
    fprintf (outFile, "\\maketitle" "\n");

    fprintf (outFile, "\\section{Введение в проблему}" "\n");

    Tree<Nod> func = GetG (function, varName);
    fprintf (outFile, "На операционном столе сегодня находится следующая функция:\\\\" "\n"
                       "$f(%c) = ", varName);
    printFunc (outFile, &func, func.getdata ());
    fprintf (outFile, "$\\\\" "\n"
                      "После некоторых очевиднейших преобразований получаем :\\\\" "\n"
                      "$f(%c) = ", varName);
    simplify (&func, func.getdata());
    printFunc (outFile, &func, func.getdata());
    fprintf (outFile, "$\\\\" "\n");

    fprintf (outFile, "\\section{Нахождение первой производной}" "\n"
                      "Найдем $f\\prime(%c)$ :\\\\", varName);

    Tree<Nod> diff1 = differentiate (&func, outFile, varName);
    dump (diff1);

    fprintf (outFile, "Имеем $f\\prime(%c) = ", varName);
    printFunc (outFile, &diff1, diff1.getdata ());
    fprintf (outFile, "$\\\\" "\n");

    fprintf (outFile, "\\section{Нахождение второй производной}" "\n"
                      "$f\\prime\\prime(%c) = ", varName);
    diff1.NodCtor (diff1.getdata()->prev, diff1.getdata()->type, diff1.getdata()->val, diff1.getdata()->left, diff1.getdata()->right, diff1.getdata(), 0, 1);
    printFunc (outFile, &diff1, diff1.getdata ());
    fprintf (outFile, "$\\\\ \n");

    Tree<Nod> diff2 = differentiate (&diff1, outFile, varName);

    fprintf (outFile, "Имеем $f\\prime\\prime(%c) = ", varName);
    printFunc (outFile, &diff2, diff2.getdata ());
    fprintf (outFile, "$\\\\" "\n");
    dump (diff2);

    fprintf (outFile, "\\section{Найдем первую и вторую производную в точке, а также саму функцию}" "\n");

    fprintf (outFile, "$f(%lg) = %lg$\\\\" "\n", x0, calc (&func, func.getdata (), x0));
    fprintf (outFile, "$f\\prime(%lg) = %lg $\\\\" "\n", x0, calc (&diff1, diff1.getdata (), x0));
    fprintf (outFile, "И вторую производную: \\\\ \n"
                      "$f(%lg)\\prime\\prime = %lg$ \\\\" "\n", x0, calc (&diff2, diff2.getdata (), x0));

    FILE* plot = fopen ("plotgen.py", "w");
    assert (plot != NULL);
    fprintf (plot, "import matplotlib.pyplot as plt" "\n");

    fprintf (plot, "x = [");
    for (double x = x0 - 1; x < x0 + 1; x += 0.01) fprintf (plot, "%lg, ", x);
    fprintf (plot, "%lg]\n", x0 + 1);

    fprintf (plot, "f = [");
    for (double x = x0 - 1; x < x0 + 1; x += 0.01) {
        if (!isnan (calc (&func, func.getdata (), x))) fprintf (plot, "%lg, ", calc (&func, func.getdata (), x));
        else fprintf (plot, "0, ");
    }
    if (!isnan (calc (&func, func.getdata (), x0 + 1))) fprintf (plot, "%lg]\n", calc (&func, func.getdata (), x0 + 1));
    else fprintf (plot, "0]\n");

    fprintf (plot, "f1 = [");
    for (double x = x0 - 1; x < x0 + 1; x += 0.01) {
        if (!isnan (calc (&diff1, diff1.getdata (), x))) fprintf (plot, "%lg, ", calc (&diff1, diff1.getdata (), x));
        else fprintf (plot, "0, ");
    }
    if (!isnan (calc (&diff1, diff1.getdata (), x0 + 1))) fprintf (plot, "%lg]\n", calc (&diff1, diff1.getdata (), x0 + 1));
    else fprintf (plot, "0]\n");

    fprintf (plot, "f2 = [");
    for (double x = x0 - 1; x < x0 + 1; x += 0.01){
        if (!isnan (calc (&diff2, diff2.getdata (), x))) fprintf (plot, "%lg, ", calc (&diff2, diff2.getdata (), x));
        else fprintf (plot, "0, ");
    }

    if (!isnan (calc (&diff2, diff2.getdata (), x0 + 1))) fprintf (plot, "%lg]\n", calc (&diff2, diff2.getdata (), x0 + 1));
    else fprintf (plot, "0]\n");

    fprintf (plot, "plt.grid ()" "\n"
                   "plt.plot (x, f, \'r\')" "\n"
                   "plt.plot (x, f1, \'g\')" "\n"
                   "plt.plot (x, f2, \'b\')" "\n"
                   "plt.show ()" "\n");

    fclose (plot);
    system ("C:/Users/maxsi/AppData/Local/Microsoft/WindowsApps/python3.10.exe d:/los_projectados/diff/plotgen.py");

    fprintf (outFile, "\\end{document}");
}

double calc (Tree<Nod>* tree, Nod* iter, double x0) {

    if (iter->type == CONSTANT) return iter->val;
    else if (iter->type == X) return pow (x0, iter->val);
    else if (iter->type == PLUS) return calc (tree, iter->left, x0) + calc (tree, iter->right, x0);
    else if (iter->type == MINUS) return calc (tree, iter->left, x0) - calc (tree, iter->right, x0);
    else if (iter->type == MULT) return calc (tree, iter->left, x0) * calc (tree, iter->right, x0);
    else if (iter->type == DIV) return calc (tree, iter->left, x0) / calc (tree, iter->right, x0);
    else return pow (calc (tree, iter->left, x0), calc (tree, iter->right, x0));
}

void podstava (Tree<Nod>* tree, Nod* iter, double x0) {

    if (iter == NULL) return;

    if (iter->type == X) {

        tree->NodCtor (iter->prev, CONSTANT, pow (x0, iter->val), NULL, NULL, iter);
        return;
    }

    podstava (tree, iter->left, x0);
    podstava (tree, iter->right, x0);
}

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

    assert ((iter->left != NULL and iter->right != NULL) or (iter->left == NULL and iter->right == NULL));
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

    assert ((iter->left != NULL and iter->right != NULL) or (iter->left == NULL and iter->right == NULL));
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

    assert ((iter->left != NULL and iter->right != NULL) or (iter->left == NULL and iter->right == NULL));
    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->type == DIV and iter->right->type == X) {

        tree->NodCtor (iter->prev, MULT, 0, iter->left, iter->right, iter);
        tree->NodCtor (iter, X, -iter->right->val, iter->right->left, iter->right->right, iter->right);
    }
}

void deXPow (Tree<Nod>* tree, Nod* iter) {

    assert ((iter->left != NULL and iter->right != NULL) or (iter->left == NULL and iter->right == NULL));
    if (iter->left == NULL or iter->right == NULL) return;

    if (iter->left->type == X and iter->right->type == CONSTANT and iter->type == POWER) {

        tree->NodCtor (iter->prev, X, iter->right->val * iter->left->val, iter->left, iter->right, iter);

        tree->NodDtorRec (iter->left);
        tree->NodDtorRec (iter->right);
    }
}

void killNeutral (Tree<Nod>* tree, Nod* iter) {

    assert ((iter->left != NULL and iter->right != NULL) or (iter->left == NULL and iter->right == NULL));
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

    assert ((iter->left != NULL and iter->right != NULL) or (iter->left == NULL and iter->right == NULL));
    if (iter->type == X and iter->val == 0) tree->NodCtor (iter->prev, CONSTANT, 1, NULL, NULL, iter);
}

Tree<Nod> differentiate (Tree<Nod>* tree, FILE* outFile, char varName) {

    Tree<Nod> diff;
    diff.NodRecCpy (tree->getdata (), diff.getdata ());
    setDiffStatus (&diff, diff.getdata (), 1);
    setDiffStatus (&diff, diff.getdata ()->left, 0);
    setDiffStatus (&diff, diff.getdata ()->right, 0);
    recDiff (&diff, diff.getdata (), outFile, varName);
    simplify (&diff, diff.getdata ());
    return diff;
}

void setDiffStatus (Tree<Nod>* tree, Nod* iter, int diff) {

    if (iter == NULL) return;

    tree->NodCtor (iter->prev, iter->type, iter->val, iter->left, iter->right, iter, 0, diff);

    setDiffStatus (tree, iter->left, diff);
    setDiffStatus (tree, iter->right, diff);
}

void recDiff (Tree<Nod>* tree, Nod* iter, FILE* outFile, char varName) {

    if (iter == NULL) return;

    tree->NodCtor (iter->prev, iter->type, iter->val, iter->left, iter->right, iter, 0, 0);
    if (iter->left != NULL and iter->right != NULL) {

        if (iter->type == PLUS or iter->type == MINUS) {

            tree->NodCtor (iter, iter->left->type, iter->left->val, iter->left->left, iter->left->right, iter->left, 0, 1);
            tree->NodCtor (iter, iter->right->type, iter->right->val, iter->right->left, iter->right->right, iter->right, 0, 1);
            writeFuncTex (tree, outFile, varName);

            recDiff (tree, iter->left, outFile, varName);
            recDiff (tree, iter->right, outFile, varName);
        }
        else if (iter->type == MULT) {

            tree->NodMoveLeft (iter, PLUS, 0);
            tree->NodAddRight (iter);
            tree->NodRecCpy (iter->left, iter->right);

            tree->NodCtor (iter->left, iter->left->left->type, iter->left->left->val, iter->left->left->left, iter->left->left->right, iter->left->left, 0, 1);
            tree->NodCtor (iter->right, iter->right->right->type, iter->right->right->val, iter->right->right->left, iter->right->right->right, iter->right->right, 0, 1);
            writeFuncTex (tree, outFile, varName);

            recDiff (tree, iter->left->left, outFile, varName);
            recDiff (tree, iter->right->right, outFile, varName);
        }
        else if (iter->type == DIV) {

            tree->NodMoveLeft (iter, DIV, 0);
            tree->NodAddRight (iter, POWER, 0);
            tree->NodAddLeft (iter->right);
            tree->NodAddRight (iter->right, CONSTANT, 2);
            tree->NodRecCpy (iter->left->right, iter->right->left);

            tree->NodCtor (iter, MULT, 0, iter->left->left, iter->left->right, iter->left);
            tree->NodMoveLeft (iter->left, MINUS, 0);
            tree->NodAddRight (iter->left);
            tree->NodRecCpy (iter->left->left, iter->left->right);

            tree->NodCtor (iter->left->left, iter->left->left->left->type, iter->left->left->left->val, iter->left->left->left->left, iter->left->left->left->right, iter->left->left->left, 0, 1);
            tree->NodCtor (iter->left->right, iter->left->right->right->type, iter->left->right->right->val, iter->left->right->right->left, iter->left->right->right->right, iter->left->right->right, 0, 1);
            writeFuncTex (tree, outFile, varName);

            recDiff (tree, iter->left->left->left, outFile, varName);
            recDiff (tree, iter->left->right->right, outFile, varName);
        }
    }
    else {

        if (iter->type == X) {

            tree->NodMoveRight (iter, MULT, 0);
            tree->NodAddLeft (iter, CONSTANT, iter->right->val);
            tree->NodCtor (iter, X, iter->right->val - 1, NULL, NULL, iter->right);
        }
        else tree->NodCtor (iter->prev, CONSTANT, 0, NULL, NULL, iter);
    }
}

void writeFuncTex (Tree<Nod>* derivative, FILE* outFile, char varName) {

    const char* transactions[TRANS_COUNT] = {"Очевидно, что:",
                                    "На пятой лекции Знаменской доказывалось, что:",
                                    "Если бы вы посещали вуз, вы бы знали, что:",
                                    "Жак Фреско однажды сказал:",
                                    "Если присмотреться, то можно заметить, что:",
                                    "Пропущенные шаги оставим как упражнение для читателя",
                                    "Ничто не точно, разве что:",
                                    "Для тех, кто написал реферат по бжд должно быть очевидно, что:",
                                    "Автору не очень хочется писать, как он получил все это, поэтому он просто напишет \"очевидным переходом получаем\":",
                                    "И.Р. Дединский всегда говорил, что:",
                                    "Дифференциатор сломался, поэтому предыдущие шаги были пропущены, тем не менее:",
                                    "Ладно:",
                                    "У внимательного читателя возникнет вопрос: \"почему так?\" Не очень внимательный автор отчета даст такой ответ: \"хз\" и напишет:",
                                    "что?:"};

    if (rand() % 3 == 0) return;

    fprintf (outFile, "%s\\\\" "\n", transactions [rand () % TRANS_COUNT]);
    fprintf (outFile, "$");
    printFunc (outFile, derivative, derivative->getdata ());
    fprintf (outFile, "$\\\\" "\n");
}

void printFunc (FILE* file, Tree<Nod>* tree, Nod* iter, bool longFunc) {

    if (iter == NULL) return;

    if (iter == tree->getdata ()) {

        if (tree->getTreeSize (iter) >= 70) {

            longFunc = 1;
        }
        else longFunc = 0;
    }

    if (iter->diff) fprintf (file, "(");
    if (iter->type == DIV and longFunc == 0) fprintf (file, "\\frac{");
    else if ((iter->type == MULT or (iter->type == DIV and longFunc))
        and iter->left != NULL and iter->right != NULL and (iter->left->type == MINUS or iter->left->type == PLUS)) fprintf (file, "(");
    else if (iter->type == POWER and iter->left != NULL and iter->right != NULL and (iter->left->type == MINUS
                                                                                or iter->left->type == PLUS
                                                                                or iter->left->type == MULT
                                                                                or iter->left->type == DIV
                                                                                or iter->left->type == POWER)) fprintf (file, "(");
    printFunc (file, tree, iter->left);

    if ((iter->type == MULT or (iter->type == DIV and longFunc))
        and iter->left != NULL and iter->right != NULL and (iter->left->type == MINUS or iter->left->type == PLUS)) fprintf (file, ")");
    else if (iter->type == POWER and iter->left != NULL and iter->right != NULL and (iter->left->type == MINUS
                                                                                or iter->left->type == PLUS
                                                                                or iter->left->type == MULT
                                                                                or iter->left->type == DIV
                                                                                or iter->left->type == POWER)) fprintf (file, ")");

    if (iter->type == CONSTANT) fprintf (file, "%lg", iter->val);
    else if (iter->type == X) {
        fprintf (file, "x");
        if (iter->val != 1) fprintf (file, "^{%lg}", iter->val);
    }
    else if (iter->type == PLUS) fprintf (file, "+");
    else if (iter->type == MINUS) fprintf (file, "-");
    else if (iter->type == MULT) fprintf (file, "*");
    else if (iter->type == DIV and longFunc == 0) fprintf (file, "}{");
    else if (iter->type == DIV and longFunc == 1) fprintf (file, "/");
    else if (iter->type == POWER) fprintf (file, "^{");

    if ((iter->type == MULT or (iter->type == DIV and longFunc))
        and iter->right != NULL and (iter->right->type == MINUS or iter->right->type == PLUS)) fprintf (file, "(");



    printFunc (file, tree, iter->right);

    if ((iter->type == MULT or (iter->type == DIV and longFunc))
        and iter->right != NULL and (iter->right->type == MINUS or iter->right->type == PLUS)) fprintf (file, ")");
    else if (iter->type == POWER or (iter->type == DIV and longFunc == 0)) fprintf (file, "}");
    else if (iter->type == DIV and longFunc == 1) fprintf (file, ")");
    else if (iter->diff) fprintf (file, ")\\prime ");
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