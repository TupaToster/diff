#include "protos.h"


int main (int argc, char* argv[]) {

    SafeClass<Nod> lol;
    lol.NodAddLeft ();
    lol.NodAddRight ();
    lol.NodAddLeft (lol.getdata()->left);
    dump (lol);
}