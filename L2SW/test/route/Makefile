# Makefile

SRC = route.c ../../src/calcaddr.c
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/route

route : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -o $(TARGET) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -g -o $(TARGET)_debug $(SRC)

clean:
	rm -rf $(TARGET_DIR)

