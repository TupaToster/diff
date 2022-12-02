CC=g++

CFLAGS+=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SRCS=main.cpp lib/flog.cpp diff/diff.cpp

OBJ=$(SRCS:.cpp=.o)

all: .makefile_includes
	make a.exe

.makefile_includes: $(SRCS)
	rm -rf "$@"
	$(CC) $(CFLAGS) -MM $^ > "$@"

include .makefile_includes

a.exe: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o a.exe

*.o: *.cpp
	$(CC) $(CFLAGS) *.cpp -c

clean:
	rm -rf *.o *.exe*
	rm -rf *png
	rm -rf */*.o */*.exe*
	rm -rf derivative.* *.dot
	clear

run:
	make
	./a

.PHONY: clean run .makefile_includes