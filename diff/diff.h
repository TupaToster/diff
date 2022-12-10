#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <cctype>
#include <clocale>
#include <math.h>
#include "../lib/flog.h"
#include "../lib/tree.h"

#define TRANS_COUNT 14

Tree<Nod> GetG (char* function, const char varName);

#define DEFCMD(name, priority, calc, diff, get) void Get_##priority (char** s, Tree<Nod>* tree, Nod* iter, const char varName = 'x');

#include "..\lib\codegen.h"

#undef DEFCMD

void simplify (Tree<Nod>* tree, Nod* iter);

void killConstants (Tree<Nod>* tree, Nod* iter);

void xPower (Tree<Nod>* tree, Nod* iter);

void divByX (Tree<Nod>* tree, Nod* iter);

void deXPow (Tree<Nod>* tree, Nod* iter);

Tree<Nod> differentiate (Tree<Nod>* tree, FILE* outFile, char varName, bool writeTex = true);

void recDiff (Tree<Nod>* tree, Nod* iter, FILE* outFile, char varName, bool writeTex = true);

void killNeutral (Tree<Nod>* tree, Nod* iter);

void killXZero (Tree<Nod>* tree, Nod* iter);

void writeFuncTex (Tree<Nod>* derivative, FILE* outFile, char varName);

void printFunc (FILE* file, Tree<Nod>* tree, Nod* iter, bool longFunc = 0);

void setDiffStatus (Tree<Nod>* tree, Nod* iter, int diff);

void researchFunc (char* function, const char* fileName, char varName, double x0);

void podstava (Tree<Nod>* tree, Nod* iter, double x0);

double calc (Tree<Nod>* tree, Nod* iter, double x0);