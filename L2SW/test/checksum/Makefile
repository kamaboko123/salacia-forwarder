# Makefile

SRC = ../../src/checksum.c ../../src/endian.c checksum_test.c 
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/checksum

checksum : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -o $(TARGET) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	gcc -g -o $(TARGET)_debug $(SRC)

clean:
	rm -rf $(TARGET_DIR)

