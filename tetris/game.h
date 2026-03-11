#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include "history.h"
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

enum Difficulty {
    EASY, MEDIUM, HARD
};

typedef enum {
    MENU,
    DIFFICULTY_SELECT,
    GAME,
    INSTRUCTIONS,
    GAME_OVER
} GameState;

typedef struct {
    bool active;
    int lines[4];
    int lineCount;
    int animationFrame;
} ClearAnimation;

typedef struct {
    int board[20][10];
    Tetromino current;
    Tetromino next;
    int score;
    int level;
    int linesCleared;
    Difficulty difficulty;
    int highScore;
    ClearAnimation clearAnimation;
    bool gameOver;
    HistoryList history;
    int currentPlayerID;
    char playerName[50];
    time_t startTime;
    int gameDuration;
    int blockUsage[7];
} Game;

void initGame(Game *game, Difficulty difficulty, int playerID, const char *playerName);
bool checkCollision(Game *game, Tetromino tetromino);
void lockTetromino(Game *game);
int checkFullLines(Game *game, int *clearedLines);
void doClearLines(Game *game, int *clearedLines, int lineCount);
int clearLines(Game *game);
void spawnTetromino(Game *game);
void updateGame(Game *game);
bool isGameOver(Game *game);
void moveTetrominoLeft(Game *game);
void moveTetrominoRight(Game *game);
void moveTetrominoDown(Game *game);
void dropTetromino(Game *game);
void rotateCurrentTetromino(Game *game);
void startClearAnimation(Game *game, int *clearedLines, int lineCount);
void updateClearAnimation(Game *game);
bool isClearAnimationActive(Game *game);
int loadHighScore();

#endif
