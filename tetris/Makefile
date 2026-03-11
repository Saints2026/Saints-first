# Makefile for Tetris game

CC = g++
CFLAGS = -std=c++11 -I. -I"d:\Saints-first\ege_lib\include"
LDFLAGS = -L"d:\Saints-first\ege_lib\lib\mingw64" -lgraphics64 -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm

SRCS = main.cpp tetris.cpp ui.cpp game.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = tetris.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run:
	$(TARGET)
