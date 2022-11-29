#include "protos.h"

int main (int argc, char* argv[]) {

    Tree<Nod> lol = GetG ("(2*x+x^2)/(2*x^15+4*x^2)", 'x');
    simplify (&lol, lol.getdata());
    dump (lol);
    Tree<Nod> dif = differentiate (&lol);
    dump (dif);

}