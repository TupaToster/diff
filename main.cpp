#include "protos.h"

int main (int argc, char* argv[]) {

    char* t = strdup ("(4+12+12)*2");
    printf ("%d", GetG (t));
    SafeClass<Nod> lol;
    lol.NodAddLeft (lol.getdata());
    lol.NodAddRight (lol.getdata());
    lol.NodAddLeft (lol.getdata()->left);
    dump (lol);
}