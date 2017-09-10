# Makefile

switch: l2sw.c calcaddr.c mac_table/mac_table.c
	mkdir -p bin
	gcc -o bin/l2sw l2sw.c calcaddr.c mac_table/mac_table.c

debug: l2sw.c calcaddr.c  mac_table/mac_table.c
	mkdir -p bin/debug
	gcc -g -o bin/debug/l2sw l2sw.c calcaddr.c mac_table/mac_table.c

clean:
	rm -rf bin/

