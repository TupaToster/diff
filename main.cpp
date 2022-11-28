#include "protos.h"

int main (int argc, char* argv[]) {

    Tree<Nod> lol = GetG ("12+4-8*x^3", 'x');
    dump (lol);
    simplify (&lol, lol.getdata ());
    dump (lol);
    lol.DTOR();
}