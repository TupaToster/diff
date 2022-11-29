#include "protos.h"
#include "lib/dynArray.h"

int main (int argc, char* argv[]) {

    dynarray<int> lol (5);
    *lol(4) = 4;
    dump (lol);

}