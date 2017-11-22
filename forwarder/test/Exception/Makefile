# Makefile

SRC = exception_test.cpp ../../src/Exception.cpp ../../src/comlib.cpp
C_OPT = -std=c++11 -Wall -O0
TARGET_DIR = ./bin
TARGET = $(TARGET_DIR)/except

except : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET) $(C_OPT) $(SRC)

debug : $(SRC)
	mkdir -p $(TARGET_DIR)
	g++ -o $(TARGET)_debug $(C_OPT) -g $(SRC)

clean:
	rm -rf $(TARGET_DIR)

all:
	make clean
	make
	make debug
