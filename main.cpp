#include "protos.h"

int main (int argc, char* argv[]) {

<<<<<<< Updated upstream
    Tree<Nod> lol = GetG ("12+4-8*x^3", 'x');
    dump (lol);
    simplify (&lol, lol.getdata ());
    dump (lol);
    Tree<Nod> d = differentiate (&lol);
    dump (d);
    lol.DTOR();
=======
    Tree<Nod> lol = GetG ("(2*x+x^2)/(2*x^15+4*x^2)", 'x');
    simplify (&lol, lol.getdata());
    dump (lol);
    Tree<Nod> dif = differentiate (&lol);
    dump (dif);

>>>>>>> Stashed changes
}