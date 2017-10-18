# Makefile

SRC = comlib_test.cpp ../../src/comlib.cpp
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/comlib

comlib : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

