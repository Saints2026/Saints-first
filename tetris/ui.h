#ifndef UI_H
#define UI_H

#include <graphics.h>
#include "game.h"

// 颜色定义
#define BG_TOP_COLOR EGERGB(100, 180, 255)
#define BG_BOTTOM_COLOR EGERGB(240, 250, 255)
#define BUTTON_COLOR EGERGB(200, 230, 255)
#define BUTTON_HOVER_COLOR EGERGB(180, 220, 255)
#define TEXT_COLOR EGERGB(40, 80, 120)
#define TITLE_COLOR EGERGB(40, 80, 120)
#define GAME_OVER_COLOR EGERGB(255, 80, 80)

// 按钮结构
typedef struct {
    int x, y, width, height;
    wchar_t text[50];
    int fontsize;
} Button;

// 离屏缓冲区（用于存储背景）
extern PIMAGE backgroundBuffer;

// 初始化背景缓冲区
void initBackgroundBuffer();

// 销毁背景缓冲区
void destroyBackgroundBuffer();

// 绘制背景
void drawBackground();

// 绘制按钮
void drawButton(Button button, bool hover);

// 绘制主菜单
void drawMenu(int mouseX, int mouseY);

// 绘制游戏界面
void drawGame(Game *game, int mouseX, int mouseY);

// 绘制操作说明
void drawInstructions(int mouseX, int mouseY);

// 绘制难度选择界面
void drawDifficultySelect(int mouseX, int mouseY);

// 绘制游戏结束界面
void drawGameOver(int score, int highScore, int linesCleared, int gameDuration, int mouseX, int mouseY);

// 检查鼠标是否在按钮上
bool isMouseInButton(Button button, int mouseX, int mouseY);

#endif // UI_H