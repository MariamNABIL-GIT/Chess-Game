CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = chess.exe

OBJS = board.o gamestate.o input.o moves.o pieces.o game.o

all: gameFiles $(TARGET)

gameFiles:
	@if not exist gameFiles mkdir gameFiles

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
	@if exist *.o del /Q *.o 2>nul
	@if exist $(TARGET) del /Q $(TARGET) 2>nul
	@echo Clean complete!

clean-saves:
	@if exist gameFiles rmdir /s /q gameFiles 2>nul
	@echo Saved games deleted!

run: $(TARGET)
	.\$(TARGET)

help:
	@echo Chess Game Makefile
	@echo Commands:
	@echo   make              - Build the game
	@echo   make run          - Build and run the game
	@echo   make clean        - Remove compiled files
	@echo   make clean-saves  - Delete all saved games
	@echo   make help         - Show this help

.PHONY: all clean clean-saves run help gameFiles