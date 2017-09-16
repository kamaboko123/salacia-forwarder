# Makefile

SRC = ../../src/mac_table.c ../../src/calcaddr.c mac_table_test.c
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/mac_table

checksum : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -o $(TARGET) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -g -o $(TARGET)_debug $(SRC)

clean:
	rm -rf $(TARGET_DIR)

