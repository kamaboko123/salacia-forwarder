# Makefile

SRC = src/l2sw.c src/calcaddr.c src/mac_table.c
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/l2sw

l2sw : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -o $(TARGET) -D _NO_DEBUG $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -g -o $(TARGET)_debug $(SRC)

prep : $(SRC)
	gcc -g -E $(SRC)

clean:
	rm -rf $(TARGET_DIR)

