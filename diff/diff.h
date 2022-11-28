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

Tree<Nod> GetG (char* function, const char varName);

void GetPlus (char** s, Tree<Nod>* tree, Nod* iter, const char varName);

void GetMult (char** s, Tree<Nod>* tree, Nod* iter, const char varName);

void GetNum (char** s, Tree<Nod>* tree, Nod* iter, const char varName);

void GetP (char** s, Tree<Nod>* tree, Nod* iter, const char varName);

void simplify (Tree<Nod>* tree, Nod* iter);

void killConstants (Tree<Nod>* tree, Nod* iter);

void xPower (Tree<Nod>* tree, Nod* iter);

void GetPower (char** s, Tree<Nod>* tree, Nod* iter, const char varName);

void divByX (Tree<Nod>* tree, Nod* iter);

void deXPow (Tree<Nod>* tree, Nod* iter);

Tree<Nod> differentiate (Tree<Nod>* tree);

void recDiff (Tree<Nod>* derivative, Nod* dIter, Tree<Nod>* func, Nod* fIter);

void killNeutral (Tree<Nod>* tree, Nod* iter);

void killXZero (Tree<Nod>* tree, Nod* iter);