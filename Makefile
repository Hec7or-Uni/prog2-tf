# Macros
CC = g++
CFLAGS = -std=c++11 -fmax-errors=1 
# Reglas explicitas
costemsolap: costemsolap.cpp maxsolape.o
	$(CC) costemsolap.cpp maxsolape.o -o costemsolap $(CFLAGS)
maxsolape.o: maxsolape.cpp maxsolape.hpp
	$(CC) maxsolape.cpp -c $(CFLAGS)
clean:
	rm maxsolape.o costemsolap
	rm tfb.txt