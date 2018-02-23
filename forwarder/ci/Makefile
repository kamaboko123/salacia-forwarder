# Makefile

#コンパイラ
CC = g++
CFLAGS = -std=c++11 -Wall -g -O0
INCLUDE = -I../include
LIB = -lcppunit

#テストしたいライブラリ
LIBSRCDIR = ../src
LIBSRC = $(wildcard $(LIBSRCDIR)/*.cpp)

#テストコード
TESTSRCDIR = ./src
TESTSRC = $(wildcard $(TESTSRCDIR)/*.cpp)

#出力先
OBJDIR = ./obj
LIBOBJDIR = $(OBJDIR)/lib
TESTOBJDIR = $(OBJDIR)/test

LIBOBJECTS = $(addprefix $(LIBOBJDIR)/, $(notdir $(LIBSRC:.cpp=.o)))
TESTOBJECTS = $(addprefix $(TESTOBJDIR)/, $(notdir $(TESTSRC:.cpp=.o)))

TARGETDIR = ./bin
TARGET = $(TARGETDIR)/ci_test

.SECONDARY : $(LIBOBJECTS) $(TESTOBJECTS)

all : $(TARGET)

#実行ファイル生成
$(TARGETDIR)/% : $(LIBOBJECTS) $(TESTOBJECTS)
	mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ $(LIB)

#テスト対象のライブラリのobjファイル
$(LIBOBJDIR)/%.o: $(LIBSRCDIR)/%.cpp
	mkdir -p $(LIBOBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

#テストコードのobjファイル
$(TESTOBJDIR)/%.o: $(TESTSRCDIR)/%.cpp
	mkdir -p $(TESTOBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm -rf $(OBJDIR) $(TARGETDIR)
