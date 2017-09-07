# Makefile

l2hub: l2hub.c
	mkdir -p bin
	gcc -o bin/l2hub l2hub.c


debug: l2hub.c
	mkdir -p bin/debug
	gcc -g -o bin/debug/l2hub l2hub.c

clean:
	rm -rf bin/

