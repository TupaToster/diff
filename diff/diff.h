#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <cctype>
#include <clocale>
#include "../lib/flog.h"
#include "../lib/tree.h"

Tree<Nod> functionReader (char* function, char varName);

void GetG (char* function, Tree<Nod>* tree, char varName);

void GetE (char** s, Tree<Nod>* tree, Nod* iter, char varName);

void GetT (char** s, Tree<Nod>* tree, Nod* iter, char varName);

void GetP (char** s, Tree<Nod>* tree, Nod* iter, char varName);

void GetN (char** s, Tree<Nod>* tree, Nod* iter, char varName);

void simplify (Tree<Nod>* tree, Nod* iter);

void killConstants (Tree<Nod>* tree, Nod* iter);

void xPower (Tree<Nod>* tree, Nod* iter);