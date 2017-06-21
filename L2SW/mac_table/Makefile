# Makefile

mac_table: mac_table.c ../calcaddr.c
	mkdir -p bin
	gcc -o bin/mac_table mac_table.c ../calcaddr.c


debug: mac_table.c ../calcaddr.c
	mkdir -p bin/debug
	gcc -g -o bin/debug/mac_table mac_table.c ../calcaddr.c

clean:
	rm -rf bin/

