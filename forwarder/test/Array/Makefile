# Makefile

SRC = array_test.cpp ../../src/Exception.cpp ../../src/comlib.cpp
C_OPT = -std=c++11 -Wall
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/array

array : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) $(C_OPT) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(INCLUDE) $(C_OPT) -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
