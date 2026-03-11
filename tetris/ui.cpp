#include "ui.h"
#include "game.h"
#include <wchar.h>

// 离屏缓冲区（用于存储背景）
PIMAGE backgroundBuffer = NULL;

// 初始化背景缓冲区
void initBackgroundBuffer() {
    int width = getwidth();
    int height = getheight();
    if (width > 0 && height > 0) {
        backgroundBuffer = newimage(width, height);
        // 在离屏缓冲区中绘制渐变背景
        ege_setpattern_lineargradient(0, 0, EGERGB(100, 180, 255), 0, (float)height, EGERGB(240, 250, 255), backgroundBuffer);
        ege_fillrect(0, 0, (float)width, (float)height, backgroundBuffer);
    }
}

// 销毁背景缓冲区
void destroyBackgroundBuffer() {
    if (backgroundBuffer != NULL) {
        delimage(backgroundBuffer);
        backgroundBuffer = NULL;
    }
}

// 绘制背景
void drawBackground() {
    if (backgroundBuffer != NULL) {
        // 从离屏缓冲区复制背景到屏幕
        putimage(0, 0, backgroundBuffer);
    } else {
        // 如果离屏缓冲区不存在，直接绘制渐变背景
        int height = getheight();
        int width = getwidth();
        if (height > 0 && width > 0) {
            ege_setpattern_lineargradient(0, 0, EGERGB(100, 180, 255), 0, (float)height, EGERGB(240, 250, 255));
            ege_fillrect(0, 0, (float)width, (float)height);
        } else {
            setfillcolor(BG_TOP_COLOR);
            bar(0, 0, 1200, 900);
        }
    }
}

// 绘制按钮
void drawButton(Button button, bool hover) {
    // 绘制按钮背景（圆角矩形）
    setfillcolor(hover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);
    setlinecolor(TEXT_COLOR);
    setlinestyle(PS_SOLID, 2);
    roundrect(button.x, button.y, button.x + button.width, button.y + button.height, 15, 15);
    
    // 绘制按钮文字
    settextcolor(TEXT_COLOR);
    setbkmode(TRANSPARENT);
    setfont(button.fontsize, 0, L"微软雅黑");
    
    // 确保文字完全居中
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(button.x + button.width / 2, button.y + button.height / 2, button.text);
}

// 绘制主菜单
void drawMenu(int mouseX, int mouseY) {
    drawBackground();
    
    settextcolor(TITLE_COLOR);
    setfont(80, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(getwidth() / 2, 100, L"俄罗斯方块");
    
    Button startButton = {getwidth() / 2 - 200, 250, 400, 80, L"开始游戏", 45};
    Button instrButton = {getwidth() / 2 - 200, 350, 400, 80, L"操作说明", 45};
    Button exitButton = {getwidth() / 2 - 200, 450, 400, 80, L"退出游戏", 45};
    
    drawButton(startButton, isMouseInButton(startButton, mouseX, mouseY));
    drawButton(instrButton, isMouseInButton(instrButton, mouseX, mouseY));
    drawButton(exitButton, isMouseInButton(exitButton, mouseX, mouseY));
    
    setfont(20, 0, L"微软雅黑");
    settextjustify(RIGHT_TEXT, BOTTOM_TEXT);
    outtextxy(getwidth() - 20, getheight() - 20, L"方向键控制: ←→↓ 旋转: ↑ 快速下落: 空格");
}

// 绘制游戏界面
void drawGame(Game *game, int mouseX, int mouseY) {
    drawBackground();
    
    // 绘制游戏棋盘
    int blockSize = 40;
    int boardX = 100;
    int boardY = 50;
    
    // 绘制棋盘背景（深色背景，使方块更加突出）
    setfillcolor(EGERGB(30, 30, 50));
    bar(boardX, boardY, boardX + BOARD_WIDTH * blockSize, boardY + BOARD_HEIGHT * blockSize);
    
    // 绘制棋盘边框（加粗边框，使边界更加清晰）
    setlinecolor(EGERGB(255, 255, 255));
    setlinestyle(PS_SOLID, 4);
    rectangle(boardX - 2, boardY - 2, boardX + BOARD_WIDTH * blockSize + 2, boardY + BOARD_HEIGHT * blockSize + 2);
    
    // 绘制棋盘网格线（使边界更加清晰）
    setlinecolor(EGERGB(60, 60, 80));
    setlinestyle(PS_SOLID, 1);
    for (int x = 0; x <= BOARD_WIDTH; x++) {
        line(boardX + x * blockSize, boardY, boardX + x * blockSize, boardY + BOARD_HEIGHT * blockSize);
    }
    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        line(boardX, boardY + y * blockSize, boardX + BOARD_WIDTH * blockSize, boardY + y * blockSize);
    }
    
    // 绘制棋盘内的方块
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board[y][x] != -1) {
                // 检查是否在消除动画中
                bool isClearing = false;
                if (game->clearAnimation.active) {
                    for (int i = 0; i < game->clearAnimation.lineCount; i++) {
                        if (game->clearAnimation.lines[i] == y) {
                            isClearing = true;
                            break;
                        }
                    }
                }
                
                if (isClearing) {
                    // 绘制消除动画效果（超快闪烁）
                    int frame = game->clearAnimation.animationFrame;
                    
                    // 超快闪烁效果（每帧切换颜色，最后一帧直接消失）
                    if (frame >= 3) {
                        continue; // 最后两帧不绘制，直接消失
                    } else if (frame % 2 == 0) {
                        setfillcolor(EGERGB(255, 255, 255)); // 白色闪烁
                    } else {
                        setfillcolor(tetrominoColors[game->board[y][x]]);
                    }
                } else {
                    setfillcolor(tetrominoColors[game->board[y][x]]);
                }
                
                int drawX = boardX + x * blockSize;
                int drawY = boardY + y * blockSize;
                // 只绘制在棋盘内的部分，确保方块的整个部分都在棋盘内
                if (drawY >= 0 && drawY + blockSize <= boardY + BOARD_HEIGHT * blockSize && drawX >= 0 && drawX + blockSize <= boardX + BOARD_WIDTH * blockSize) {
                    bar(drawX, drawY, drawX + blockSize, drawY + blockSize);
                    // 绘制方块边框（深色边框+白色高光）
                    setlinecolor(EGERGB(100, 100, 100));
                    setlinestyle(PS_SOLID, 2);
                    rectangle(drawX, drawY, drawX + blockSize, drawY + blockSize);
                    // 绘制白色高光
                    setlinecolor(EGERGB(255, 255, 255));
                    line(drawX, drawY, drawX + blockSize, drawY);
                    line(drawX, drawY, drawX, drawY + blockSize);
                }
            }
        }
    }
    
    // 绘制当前方块（消除动画期间不绘制，因为方块已经锁定到棋盘上）
    if (!game->clearAnimation.active) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (shapes[game->current.type][game->current.rotation][y][x] == 1) {
                    int drawX = boardX + (game->current.x + x) * blockSize;
                    int drawY = boardY + (game->current.y + y) * blockSize;
                    // 只绘制在棋盘内的部分，确保方块的整个部分都在棋盘内
                    if (drawY >= 0 && drawY + blockSize <= boardY + BOARD_HEIGHT * blockSize && drawX >= 0 && drawX + blockSize <= boardX + BOARD_WIDTH * blockSize) {
                        setfillcolor(game->current.color);
                        bar(drawX, drawY, drawX + blockSize, drawY + blockSize);
                        // 绘制方块边框（深色边框+白色高光）
                        setlinecolor(EGERGB(100, 100, 100));
                        setlinestyle(PS_SOLID, 2);
                        rectangle(drawX, drawY, drawX + blockSize, drawY + blockSize);
                        // 绘制白色高光
                        setlinecolor(EGERGB(255, 255, 255));
                        line(drawX, drawY, drawX + blockSize, drawY);
                        line(drawX, drawY, drawX, drawY + blockSize);
                    }
                }
            }
        }
    }
    
    // 绘制右侧信息框
    int infoBoxX = boardX + BOARD_WIDTH * blockSize + 100;
    int infoBoxY = 50;
    int infoBoxWidth = 300;
    int infoBoxHeight = 150;
    
    // 绘制下一个方块信息框
    setfillcolor(BUTTON_COLOR);
    bar(infoBoxX, infoBoxY, infoBoxX + infoBoxWidth, infoBoxY + infoBoxHeight);
    settextcolor(TEXT_COLOR);
    setfont(35, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(infoBoxX + infoBoxWidth / 2, infoBoxY + 20, L"下一个方块");
    
    // 绘制下一个方块预览
    int previewX = infoBoxX + infoBoxWidth / 2 - 2 * blockSize;
    int previewY = infoBoxY + 60;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shapes[game->next.type][game->next.rotation][y][x] == 1) {
                setfillcolor(game->next.color);
                bar(previewX + x * blockSize / 2, previewY + y * blockSize / 2, 
                    previewX + (x + 1) * blockSize / 2, previewY + (y + 1) * blockSize / 2);
                setlinecolor(EGERGB(255, 255, 255));
                rectangle(previewX + x * blockSize / 2, previewY + y * blockSize / 2, 
                          previewX + (x + 1) * blockSize / 2, previewY + (y + 1) * blockSize / 2);
            }
        }
    }
    
    // 绘制分数信息框
    int scoreBoxY = infoBoxY + infoBoxHeight + 20;
    setfillcolor(BUTTON_COLOR);
    bar(infoBoxX, scoreBoxY, infoBoxX + infoBoxWidth, scoreBoxY + infoBoxHeight);
    settextcolor(TEXT_COLOR);
    outtextxy(infoBoxX + infoBoxWidth / 2, scoreBoxY + 20, L"分数");
    
    // 绘制分数值
    wchar_t scoreText[20];
    swprintf(scoreText, 20, L"%d", game->score);
    setfont(40, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(infoBoxX + infoBoxWidth / 2, scoreBoxY + infoBoxHeight / 2, scoreText);
    
    // 绘制等级信息框
    int levelBoxY = scoreBoxY + infoBoxHeight + 20;
    setfillcolor(BUTTON_COLOR);
    bar(infoBoxX, levelBoxY, infoBoxX + infoBoxWidth, levelBoxY + infoBoxHeight);
    settextcolor(TEXT_COLOR);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(infoBoxX + infoBoxWidth / 2, levelBoxY + 20, L"等级");
    
    // 绘制等级值
    wchar_t levelText[20];
    swprintf(levelText, 20, L"%d", game->level);
    setfont(40, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(infoBoxX + infoBoxWidth / 2, levelBoxY + infoBoxHeight / 2, levelText);
    
    // 绘制最高分信息框
    int highScoreBoxY = levelBoxY + infoBoxHeight + 20;
    setfillcolor(BUTTON_COLOR);
    bar(infoBoxX, highScoreBoxY, infoBoxX + infoBoxWidth, highScoreBoxY + infoBoxHeight);
    settextcolor(TEXT_COLOR);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(infoBoxX + infoBoxWidth / 2, highScoreBoxY + 20, L"最高分");
    
    // 绘制最高分值
    wchar_t highScoreText[20];
    swprintf(highScoreText, 20, L"%d", game->highScore);
    setfont(40, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(infoBoxX + infoBoxWidth / 2, highScoreBoxY + infoBoxHeight / 2, highScoreText);
    
    // 绘制返回菜单按钮
    Button backButton = {infoBoxX, highScoreBoxY + infoBoxHeight + 40, 300, 60, L"返回菜单", 35};
    drawButton(backButton, isMouseInButton(backButton, mouseX, mouseY));
}

// 绘制操作说明
void drawInstructions(int mouseX, int mouseY) {
    drawBackground();
    
    // 绘制标题
    settextcolor(TITLE_COLOR);
    setfont(40, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(getwidth() / 2, 100, L"操作说明");
    
    // 绘制操作说明内容
    setfont(30, 0, L"微软雅黑");
    settextjustify(LEFT_TEXT, TOP_TEXT);
    
    int textX = getwidth() / 2 - 300;
    int textY = 200;
    int lineHeight = 40;
    
    outtextxy(textX, textY, L"【基础操作】");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"方向键 ← →：控制方块左右移动");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"方向键 ↑：控制方块旋转");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"方向键 ↓：加速方块下落");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"空格键：方块快速落地");
    textY += lineHeight * 2;
    
    outtextxy(textX, textY, L"【计分规则】");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"消除 1 行：100 分 × 当前等级");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"消除 2 行：300 分 × 当前等级");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"消除 3 行：500 分 × 当前等级");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"消除 4 行：800 分 × 当前等级");
    textY += lineHeight * 2;
    
    outtextxy(textX, textY, L"【游戏结束】");
    textY += lineHeight;
    outtextxy(textX + 50, textY, L"方块堆积到顶部时，游戏结束");
    
    // 绘制返回菜单按钮
    Button backButton = {getwidth() / 2 - 100, getheight() - 100, 200, 60, L"返回菜单", 35};
    drawButton(backButton, isMouseInButton(backButton, mouseX, mouseY));
}

// 绘制难度选择界面
void drawDifficultySelect(int mouseX, int mouseY) {
    drawBackground();
    
    settextcolor(TITLE_COLOR);
    setfont(60, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(getwidth() / 2, 150, L"选择难度");
    
    Button easyButton = {getwidth() / 2 - 150, 300, 300, 70, L"简单", 40};
    Button mediumButton = {getwidth() / 2 - 150, 390, 300, 70, L"中等", 40};
    Button hardButton = {getwidth() / 2 - 150, 480, 300, 70, L"困难", 40};
    Button backButton = {getwidth() / 2 - 100, 570, 200, 60, L"返回", 35};
    
    drawButton(easyButton, isMouseInButton(easyButton, mouseX, mouseY));
    drawButton(mediumButton, isMouseInButton(mediumButton, mouseX, mouseY));
    drawButton(hardButton, isMouseInButton(hardButton, mouseX, mouseY));
    drawButton(backButton, isMouseInButton(backButton, mouseX, mouseY));
}

// 绘制游戏结束界面
void drawGameOver(int score, int highScore, int linesCleared, int gameDuration, int mouseX, int mouseY) {
    drawBackground();
    
    settextcolor(GAME_OVER_COLOR);
    setfont(80, 0, L"微软雅黑");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(getwidth() / 2, 100, L"游戏结束");
    
    settextcolor(TEXT_COLOR);
    setfont(40, 0, L"微软雅黑");
    
    wchar_t scoreText[50];
    swprintf(scoreText, 50, L"得分：%d", score);
    outtextxy(getwidth() / 2, 210, scoreText);
    
    wchar_t highScoreText[50];
    swprintf(highScoreText, 50, L"最高分：%d", highScore);
    outtextxy(getwidth() / 2, 260, highScoreText);
    
    wchar_t linesText[50];
    swprintf(linesText, 50, L"消除行数：%d", linesCleared);
    outtextxy(getwidth() / 2, 310, linesText);
    
    wchar_t timeText[50];
    int minutes = gameDuration / 60;
    int seconds = gameDuration % 60;
    swprintf(timeText, 50, L"游戏时长：%d分%d秒", minutes, seconds);
    outtextxy(getwidth() / 2, 360, timeText);
    
    Button restartButton = {getwidth() / 2 - 150, 420, 300, 70, L"重新开始", 40};
    Button backButton = {getwidth() / 2 - 150, 510, 300, 70, L"返回菜单", 40};
    
    drawButton(restartButton, isMouseInButton(restartButton, mouseX, mouseY));
    drawButton(backButton, isMouseInButton(backButton, mouseX, mouseY));
}

// 检查鼠标是否在按钮上
bool isMouseInButton(Button button, int mouseX, int mouseY) {
    return mouseX >= button.x && mouseX <= button.x + button.width && 
           mouseY >= button.y && mouseY <= button.y + button.height;
}
