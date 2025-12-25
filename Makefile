CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = chess.exe

OBJS = board.o gamestate.o input.o moves.o pieces.o game.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	@echo ========================================
	@echo Build successful!
	@echo Run with: .\chess.exe
	@echo ========================================

board.o: source/board.c
	$(CC) $(CFLAGS) -c source/board.c -o board.o

gamestate.o: source/gamestate.c
	$(CC) $(CFLAGS) -c source/gamestate.c -o gamestate.o

input.o: source/input.c
	$(CC) $(CFLAGS) -c source/input.c -o input.o

moves.o: source/moves.c
	$(CC) $(CFLAGS) -c source/moves.c -o moves.o

pieces.o: source/pieces.c
	$(CC) $(CFLAGS) -c source/pieces.c -o pieces.o

game.o: source/game.c
	$(CC) $(CFLAGS) -c source/game.c -o game.o

clean:
	del /Q *.o $(TARGET) 2>nul

run: $(TARGET)
	.\$(TARGET)

.PHONY: all clean run