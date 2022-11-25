#include "protos.h"

int main (int argc, char* argv[]) {

    Tree<Nod> lol;
    char str[100] = {};
    scanf ("%s", str);
    lol = functionReader (str, 'x');
    dump (lol);
    //simplify (&lol, lol.getdata());
    dump (lol);
}