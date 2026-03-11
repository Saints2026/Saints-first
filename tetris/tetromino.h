#ifndef TETROMINO_H
#define TETROMINO_H

#include <graphics.h>

// 方块类型枚举
enum TetrominoType {
    I, O, T, L, J, S, Z
};

// 方块结构
typedef struct {
    TetrominoType type;
    int x, y;          // 方块在棋盘上的坐标
    int rotation;      // 旋转状态 (0-3)
    COLORREF color;    // 方块颜色
} Tetromino;

// 方块形状定义（7种类型，每种4种旋转状态）
extern int shapes[7][4][4][4];

// 方块颜色定义
extern COLORREF tetrominoColors[7];

// 初始化方块
Tetromino createTetromino(TetrominoType type);

// 旋转方块
void rotateTetromino(Tetromino *tetromino);

#endif // TETROMINO_H