# Makefile

SRC = comlib_test.cpp ../../src/comlib.cpp
INCLUDE = -I ../../include
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/comlib

comlib : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(INCLUDE) -std=c++11 $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(INCLUDE) -std=c++11 -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

