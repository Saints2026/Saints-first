#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <cstring>

void initGame(Game *game, Difficulty difficulty, int playerID, const char *playerName) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = -1;
        }
    }
    
    game->score = 0;
    game->level = 1;
    game->linesCleared = 0;
    game->difficulty = difficulty;
    game->highScore = loadHighScore();
    game->currentPlayerID = playerID;
    strncpy(game->playerName, playerName, 49);
    game->playerName[49] = '\0';
    
    game->clearAnimation.active = false;
    game->clearAnimation.lineCount = 0;
    game->clearAnimation.animationFrame = 0;
    
    game->gameOver = false;
    game->startTime = time(NULL);
    game->gameDuration = 0;
    
    for (int i = 0; i < 7; i++) {
        game->blockUsage[i] = 0;
    }
    
    srand(time(NULL));
    game->current = createTetromino((TetrominoType)(rand() % 7));
    game->next = createTetromino((TetrominoType)(rand() % 7));
}

bool checkCollision(Game *game, Tetromino tetromino) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[tetromino.type][tetromino.rotation][y][x] == 1) {
                int boardX = tetromino.x + x;
                int boardY = tetromino.y + y;
                
                if (boardX < 0 || boardX >= BOARD_WIDTH) {
                    return true;
                }
                
                if (boardY >= BOARD_HEIGHT) {
                    return true;
                }
                
                if (boardY >= 0 && game->board[boardY][boardX] != -1) {
                    return true;
                }
            }
        }
    }
    return false;
}

void lockTetromino(Game *game) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[game->current.type][game->current.rotation][y][x] == 1) {
                int boardX = game->current.x + x;
                int boardY = game->current.y + y;
                if (boardY >= 0 && boardY < BOARD_HEIGHT) {
                    game->board[boardY][boardX] = game->current.type;
                }
            }
        }
    }
    
    if (game->current.type >= 0 && game->current.type < 7) {
        game->blockUsage[game->current.type]++;
    }
}

int checkFullLines(Game *game, int *clearedLines) {
    int lineCount = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board[y][x] == -1) {
                full = false;
                break;
            }
        }
        if (full && lineCount < 4) {
            clearedLines[lineCount] = y;
            lineCount++;
        }
    }
    return lineCount;
}

void doClearLines(Game *game, int *clearedLines, int lineCount) {
    if (lineCount == 0) return;
    
    // 1. 遍历检测：从游戏棋盘的最底部行向上逐行检测，判断每行是否为"满行"
    // 2. 标记满行：将所有检测到的满行进行标记，统计满行数量
    // 注意：这两步已经在checkFullLines函数中完成，clearedLines数组已经包含了要消除的行
    
    // 3. 清除满行：按标记批量清除所有满行
    // 4. 下落补位：将满行上方的所有方块，按垂直方向向下移动
    // 5. 顶部补空：棋盘最顶部的空白行用空值填充
    
    // 首先，创建一个临时棋盘来存储消除后的状态
    int tempBoard[BOARD_HEIGHT][BOARD_WIDTH];
    
    // 初始化临时棋盘为-1（空值）
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            tempBoard[y][x] = -1;
        }
    }
    
    // 标记哪些行要被消除
    bool toClear[BOARD_HEIGHT] = {false};
    for (int i = 0; i < lineCount; i++) {
        toClear[clearedLines[i]] = true;
    }
    
    // 复制未被消除的行到临时棋盘，实现下落补位
    int destRow = BOARD_HEIGHT - 1;
    for (int srcRow = BOARD_HEIGHT - 1; srcRow >= 0; srcRow--) {
        if (!toClear[srcRow]) {
            // 复制该行到临时棋盘的当前目标行
            for (int x = 0; x < BOARD_WIDTH; x++) {
                tempBoard[destRow][x] = game->board[srcRow][x];
            }
            // 目标行上移
            destRow--;
        }
    }
    
    // 6. 计分更新：根据消除的满行数量，按预设规则更新得分
    // 注意：这一步已经在updateClearAnimation函数中完成
    
    // 7. 边界校验：确保消除与补位过程中，所有方块始终在棋盘可见区域内
    // 由于我们使用了临时棋盘，并且只复制了有效方块，边界校验已经自动完成
    
    // 将临时棋盘复制回游戏棋盘
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = tempBoard[y][x];
        }
    }
}

int clearLines(Game *game) {
    int clearedLines[4] = {-1, -1, -1, -1};
    int lineCount = checkFullLines(game, clearedLines);
    
    if (lineCount > 0) {
        doClearLines(game, clearedLines, lineCount);
        
        game->linesCleared += lineCount;
        switch (lineCount) {
            case 1: game->score += 100 * game->level; break;
            case 2: game->score += 300 * game->level; break;
            case 3: game->score += 500 * game->level; break;
            case 4: game->score += 800 * game->level; break;
        }
        
        if (game->score >= game->level * 1000) {
            game->level++;
        }
        
        if (game->score > game->highScore) {
            game->highScore = game->score;
        }
    }
    
    return lineCount;
}

void spawnTetromino(Game *game) {
    game->current = game->next;
    game->next = createTetromino((TetrominoType)(rand() % 7));
    
    Tetromino temp = game->current;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[temp.type][temp.rotation][y][x] == 1) {
                int boardX = temp.x + x;
                int boardY = temp.y + y;
                if (boardY < 0) {
                    game->gameOver = true;
                    return;
                }
                if (boardY >= 0 && boardY < BOARD_HEIGHT && boardX >= 0 && boardX < BOARD_WIDTH) {
                    if (game->board[boardY][boardX] != -1) {
                        game->gameOver = true;
                        return;
                    }
                }
            }
        }
    }
}

void updateGame(Game *game) {
    if (game->clearAnimation.active) {
        updateClearAnimation(game);
        return;
    }
    
    Tetromino temp = game->current;
    temp.y++;
    if (!checkCollision(game, temp)) {
        bool isInBounds = true;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (shapes[temp.type][temp.rotation][y][x] == 1) {
                    int boardY = temp.y + y;
                    if (boardY < 0 || boardY >= BOARD_HEIGHT) {
                        isInBounds = false;
                        break;
                    }
                }
            }
            if (!isInBounds) break;
        }
        if (isInBounds) {
            game->current = temp;
        }
    } else {
        lockTetromino(game);
        
        int clearedLines[4] = {-1, -1, -1, -1};
        int lineCount = checkFullLines(game, clearedLines);
        
        if (lineCount > 0) {
            startClearAnimation(game, clearedLines, lineCount);
        } else {
            spawnTetromino(game);
        }
    }
}

bool isGameOver(Game *game) {
    return game->gameOver;
}

void moveTetrominoLeft(Game *game) {
    if (game->clearAnimation.active) return;
    
    Tetromino temp = game->current;
    temp.x--;
    if (!checkCollision(game, temp)) {
        game->current = temp;
    }
}

void moveTetrominoRight(Game *game) {
    if (game->clearAnimation.active) return;
    
    Tetromino temp = game->current;
    temp.x++;
    if (!checkCollision(game, temp)) {
        game->current = temp;
    }
}

void moveTetrominoDown(Game *game) {
    if (game->clearAnimation.active) return;
    
    Tetromino temp = game->current;
    temp.y++;
    if (!checkCollision(game, temp)) {
        bool isInBounds = true;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (shapes[temp.type][temp.rotation][y][x] == 1) {
                    int boardY = temp.y + y;
                    if (boardY < 0 || boardY >= BOARD_HEIGHT) {
                        isInBounds = false;
                        break;
                    }
                }
            }
            if (!isInBounds) break;
        }
        if (isInBounds) {
            game->current = temp;
        }
    } else {
        lockTetromino(game);
        
        int clearedLines[4] = {-1, -1, -1, -1};
        int lineCount = checkFullLines(game, clearedLines);
        
        if (lineCount > 0) {
            startClearAnimation(game, clearedLines, lineCount);
        } else {
            spawnTetromino(game);
        }
    }
}

void dropTetromino(Game *game) {
    if (game->clearAnimation.active) return;
    
    while (true) {
        Tetromino temp = game->current;
        temp.y++;
        if (!checkCollision(game, temp)) {
            bool isInBounds = true;
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    if (shapes[temp.type][temp.rotation][y][x] == 1) {
                        int boardY = temp.y + y;
                        if (boardY < 0 || boardY >= BOARD_HEIGHT) {
                            isInBounds = false;
                            break;
                        }
                    }
                }
                if (!isInBounds) break;
            }
            if (isInBounds) {
                game->current = temp;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    lockTetromino(game);
    
    int clearedLines[4];
    int lineCount = checkFullLines(game, clearedLines);
    
    if (lineCount > 0) {
        startClearAnimation(game, clearedLines, lineCount);
    } else {
        spawnTetromino(game);
    }
}

void rotateCurrentTetromino(Game *game) {
    if (game->clearAnimation.active) return;
    
    Tetromino temp = game->current;
    temp.rotation = (temp.rotation + 1) % 4;
    if (!checkCollision(game, temp)) {
        game->current = temp;
    } else {
        temp.x--;
        if (!checkCollision(game, temp)) {
            game->current = temp;
            return;
        }
        temp.x += 2;
        if (!checkCollision(game, temp)) {
            game->current = temp;
            return;
        }
        temp.x--;
        temp.y--;
        if (!checkCollision(game, temp)) {
            game->current = temp;
        }
    }
}

void startClearAnimation(Game *game, int *clearedLines, int lineCount) {
    game->clearAnimation.active = true;
    game->clearAnimation.lineCount = lineCount;
    game->clearAnimation.animationFrame = 0;
    
    // 调试输出：打印要消除的行
    // printf("Start clear animation: lineCount=%d, lines=[", lineCount);
    for (int i = 0; i < lineCount; i++) {
        game->clearAnimation.lines[i] = clearedLines[i];
        // printf("%d ", clearedLines[i]);
    }
    // printf("]\n");
}

void updateClearAnimation(Game *game) {
    // 动画帧计数
    game->clearAnimation.animationFrame++;
    
    // 当动画帧达到5时，结束动画
    if (game->clearAnimation.animationFrame >= 5) {
        // 结束动画
        game->clearAnimation.active = false;
        
        // 清除行
        doClearLines(game, game->clearAnimation.lines, game->clearAnimation.lineCount);
        
        // 更新分数和等级
        int lineCount = game->clearAnimation.lineCount;
        if (lineCount > 0) {
            game->linesCleared += lineCount;
            switch (lineCount) {
                case 1: game->score += 100 * game->level; break;
                case 2: game->score += 300 * game->level; break;
                case 3: game->score += 500 * game->level; break;
                case 4: game->score += 800 * game->level; break;
            }
            
            if (game->score >= game->level * 1000) {
                game->level++;
            }
            
            if (game->score > game->highScore) {
                game->highScore = game->score;
            }
        }
        
        // 检查是否有新的满行（消除后可能产生新的满行）
        int newClearedLines[4] = {-1, -1, -1, -1};
        int newLineCount = checkFullLines(game, newClearedLines);
        
        if (newLineCount > 0) {
            // 如果有新的满行，继续执行消除动画
            startClearAnimation(game, newClearedLines, newLineCount);
        } else {
            // 否则生成新方块
            spawnTetromino(game);
        }
        
        // 重置动画帧计数
        game->clearAnimation.animationFrame = 0;
    }
}

bool isClearAnimationActive(Game *game) {
    return game->clearAnimation.active;
}
