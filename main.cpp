#include "protos.h"
#include "lib/tree.h"

int main (int argc, char* argv[]) {

    SafeClass<Nod> lol;
    lol.NodAddLeft (lol.getdata());
    lol.NodAddRight (lol.getdata());
    lol.NodAddLeft (lol.getdata()->left);
    dump (lol);
}