#include <graphics.h>
#include "game.h"
#include "ui.h"
#include <time.h>

int main() {
    initgraph(1200, 900);
    setbkcolor(BG_TOP_COLOR);
    cleardevice();
    setrendermode(RENDER_MANUAL);
    
    // 初始化背景缓冲区
    initBackgroundBuffer();
    
    Game game;
    initHistoryList(&game.history);
    loadHistoryFromFile(&game.history, "history.txt");
    
    initGame(&game, EASY, 1, "Player1");
    
    GameState state = MENU;
    
    int frameCount = 0;
    int dropSpeed = 30;
    
    int mouseDownX = -1, mouseDownY = -1;
    bool mouseDown = false;
    int lastClickTime = 0;
    const int CLICK_DELAY = 200; // 200ms的点击延迟，防止重复触发
    
    while (is_run()) {
        // 优先处理键盘事件，确保方向键响应实时性
        // 使用EGE库的kbmsg()函数处理键盘输入
        while (kbmsg()) {
            key_msg msg = getkey();
            if (msg.msg == key_msg_down) {
                int key = msg.key;
                
                // 处理方向键
                if (key == key_left) { // 左箭头
                    if (state == GAME && !game.clearAnimation.active) {
                        moveTetrominoLeft(&game);
                    }
                } else if (key == key_right) { // 右箭头
                    if (state == GAME && !game.clearAnimation.active) {
                        moveTetrominoRight(&game);
                    }
                } else if (key == key_up) { // 上箭头
                    if (state == GAME && !game.clearAnimation.active) {
                        rotateCurrentTetromino(&game);
                    }
                } else if (key == key_down) { // 下箭头
                    if (state == GAME && !game.clearAnimation.active) {
                        moveTetrominoDown(&game);
                    }
                } else if (key == ' ') { // 空格键
                    if (state == GAME && !game.clearAnimation.active) {
                        dropTetromino(&game);
                    }
                } else if (key == 27) { // ESC键
                    if (state == GAME) {
                        int gameDuration = (int)(time(NULL) - game.startTime);
                        int mostUsedBlock = 0;
                        int maxCount = 0;
                        for (int i = 0; i < 7; i++) {
                            if (game.blockUsage[i] > maxCount) {
                                maxCount = game.blockUsage[i];
                                mostUsedBlock = i;
                            }
                        }
                        addHistoryRecord(&game.history, game.currentPlayerID, game.playerName, game.score, 
                                       game.level, game.linesCleared, gameDuration, 
                                       game.difficulty, game.blockUsage, mostUsedBlock);
                        saveHistoryToFile(&game.history, "history.txt");
                    }
                    state = MENU;
                }
            }
        }
        
        // 处理鼠标事件
        int mouseX = 0, mouseY = 0;
        bool mouseClicked = false;
        
        while (mousemsg()) {
            mouse_msg msg = getmouse();
            mouseX = msg.x;
            mouseY = msg.y;
            
            if (msg.is_down()) {
                mouseDown = true;
                mouseDownX = msg.x;
                mouseDownY = msg.y;
            } else if (msg.is_up() && mouseDown) {
                if (mouseX >= mouseDownX - 10 && mouseX <= mouseDownX + 10 &&
                    mouseY >= mouseDownY - 10 && mouseY <= mouseDownY + 10) {
                    mouseClicked = true;
                }
                mouseDown = false;
            }
        }
        
        if (mouseClicked) {
            // 检查是否在点击延迟期内，防止重复触发
            int currentTime = clock();
            if (currentTime - lastClickTime >= CLICK_DELAY) {
                lastClickTime = currentTime;
                
                if (state == MENU) {
                    Button startButton = {getwidth() / 2 - 200, 250, 400, 80, L"开始游戏", 45};
                    Button instrButton = {getwidth() / 2 - 200, 350, 400, 80, L"操作说明", 45};
                    Button exitButton = {getwidth() / 2 - 200, 450, 400, 80, L"退出游戏", 45};
                    
                    if (isMouseInButton(startButton, mouseX, mouseY)) {
                        // 点击开始游戏后进入难度选择界面
                        state = DIFFICULTY_SELECT;
                    } else if (isMouseInButton(instrButton, mouseX, mouseY)) {
                        state = INSTRUCTIONS;
                    } else if (isMouseInButton(exitButton, mouseX, mouseY)) {
                        break;
                    }
                } else if (state == DIFFICULTY_SELECT) {
                    Button easyButton = {getwidth() / 2 - 150, 300, 300, 70, L"简单", 40};
                    Button mediumButton = {getwidth() / 2 - 150, 390, 300, 70, L"中等", 40};
                    Button hardButton = {getwidth() / 2 - 150, 480, 300, 70, L"困难", 40};
                    Button backButton = {getwidth() / 2 - 100, 570, 200, 60, L"返回", 35};
                    
                    if (isMouseInButton(easyButton, mouseX, mouseY)) {
                        initGame(&game, EASY, 1, "Player1");
                        state = GAME;
                    } else if (isMouseInButton(mediumButton, mouseX, mouseY)) {
                        initGame(&game, MEDIUM, 1, "Player1");
                        state = GAME;
                    } else if (isMouseInButton(hardButton, mouseX, mouseY)) {
                        initGame(&game, HARD, 1, "Player1");
                        state = GAME;
                    } else if (isMouseInButton(backButton, mouseX, mouseY)) {
                        state = MENU;
                    }
                } else if (state == INSTRUCTIONS) {
                    Button backButton = {getwidth() / 2 - 100, getheight() - 100, 200, 60, L"返回菜单", 35};
                    if (isMouseInButton(backButton, mouseX, mouseY)) {
                        state = MENU;
                    }
                } else if (state == GAME) {
                    // 计算与ui.cpp中相同的按钮位置
                    int blockSize = 40;
                    int boardX = 100;
                    int infoBoxX = boardX + BOARD_WIDTH * blockSize + 100;
                    int infoBoxY = 50;
                    int infoBoxWidth = 300;
                    int infoBoxHeight = 150;
                    int highScoreBoxY = infoBoxY + infoBoxHeight * 3 + 60; // 三个信息框的高度
                    Button backButton = {infoBoxX, highScoreBoxY + infoBoxHeight + 40, 300, 60, L"返回菜单", 35};
                    if (isMouseInButton(backButton, mouseX, mouseY)) {
                        int gameDuration = (int)(time(NULL) - game.startTime);
                        int mostUsedBlock = 0;
                        int maxCount = 0;
                        for (int i = 0; i < 7; i++) {
                            if (game.blockUsage[i] > maxCount) {
                                maxCount = game.blockUsage[i];
                                mostUsedBlock = i;
                            }
                        }
                        addHistoryRecord(&game.history, game.currentPlayerID, game.playerName, game.score, 
                                       game.level, game.linesCleared, gameDuration, 
                                       game.difficulty, game.blockUsage, mostUsedBlock);
                        saveHistoryToFile(&game.history, "history.txt");
                        
                        state = MENU;
                    }
                } else if (state == GAME_OVER) {
                    Button restartButton = {getwidth() / 2 - 150, 420, 300, 70, L"重新开始", 40};
                    Button backButton = {getwidth() / 2 - 150, 510, 300, 70, L"返回菜单", 40};
                    
                    if (isMouseInButton(restartButton, mouseX, mouseY)) {
                        initGame(&game, game.difficulty, 1, "Player1");
                        state = GAME;
                    } else if (isMouseInButton(backButton, mouseX, mouseY)) {
                        int gameDuration = game.gameDuration;
                        int mostUsedBlock = 0;
                        int maxCount = 0;
                        for (int i = 0; i < 7; i++) {
                            if (game.blockUsage[i] > maxCount) {
                                maxCount = game.blockUsage[i];
                                mostUsedBlock = i;
                            }
                        }
                        addHistoryRecord(&game.history, game.currentPlayerID, game.playerName, game.score, 
                                       game.level, game.linesCleared, gameDuration, 
                                       game.difficulty, game.blockUsage, mostUsedBlock);
                        saveHistoryToFile(&game.history, "history.txt");
                        
                        state = MENU;
                    }
                }
            }
        }
        
        if (state == GAME) {
            frameCount++;
            dropSpeed = 30 - (game.level - 1) * 3;
            if (dropSpeed < 5) dropSpeed = 5;
            
            // 控制消除动画的更新频率
            static int animationTimer = 0;
            if (game.clearAnimation.active) {
                // 消除动画激活时，控制更新频率
                animationTimer++;
                if (animationTimer >= 10) { // 每10帧更新一次消除动画
                    updateGame(&game);
                    animationTimer = 0;
                }
            } else if (frameCount >= dropSpeed) {
                // 消除动画未激活时，按正常速度更新
                updateGame(&game);
                frameCount = 0;
            }
            
            if (game.gameOver) {
                game.gameDuration = (int)(time(NULL) - game.startTime);
                int mostUsedBlock = 0;
                int maxCount = 0;
                for (int i = 0; i < 7; i++) {
                    if (game.blockUsage[i] > maxCount) {
                        maxCount = game.blockUsage[i];
                        mostUsedBlock = i;
                    }
                }
                addHistoryRecord(&game.history, game.currentPlayerID, game.playerName, game.score, 
                               game.level, game.linesCleared, game.gameDuration, 
                               game.difficulty, game.blockUsage, mostUsedBlock);
                saveHistoryToFile(&game.history, "history.txt");
                
                state = GAME_OVER;
            }
        }
        
        switch (state) {
            case MENU:
                drawMenu(mouseX, mouseY);
                break;
            case DIFFICULTY_SELECT:
                drawDifficultySelect(mouseX, mouseY);
                break;
            case GAME:
                drawGame(&game, mouseX, mouseY);
                break;
            case INSTRUCTIONS:
                drawInstructions(mouseX, mouseY);
                break;
            case GAME_OVER:
                drawGameOver(game.score, game.highScore, game.linesCleared, 
                           game.gameDuration, mouseX, mouseY);
                break;
        }
        
        delay_ms(16);
        delay_fps(60);
    }
    
    clearHistoryList(&game.history);
    destroyBackgroundBuffer();
    closegraph();
    return 0;
}
