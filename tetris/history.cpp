#include "history.h"
#include <fstream>
#include <ctime>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

void initHistoryList(HistoryList *list) {
    list->head = NULL;
    list->count = 0;
}

void addHistoryRecord(HistoryList *list, int playerID, const char *playerName, int score, 
                      int level, int linesCleared, int gameDuration, 
                      int difficulty, int blockUsage[7], int mostUsedBlock) {
    HistoryNode *newNode = new HistoryNode;
    newNode->recordID = list->count + 1;
    newNode->playerID = playerID;
    strncpy(newNode->playerName, playerName, 49);
    newNode->playerName[49] = '\0';
    newNode->score = score;
    newNode->level = level;
    newNode->linesCleared = linesCleared;
    newNode->gameDuration = gameDuration;
    newNode->difficulty = (HistoryDifficulty)difficulty;
    newNode->mostUsedBlock = mostUsedBlock;
    
    time_t now = time(0);
    struct tm *timeinfo = localtime(&now);
    strftime(newNode->gameDate, 20, "%Y-%m-%d %H:%M:%S", timeinfo);
    
    for (int i = 0; i < 7; i++) {
        newNode->blockUsage[i] = blockUsage[i];
    }
    
    newNode->next = NULL;
    
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        newNode->next = list->head;
        list->head = newNode;
    }
    list->count++;
    
    if (list->count > 20) {
        HistoryNode *current = list->head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        delete current->next;
        current->next = NULL;
        list->count--;
    }
}

const char* getDifficultyString(int difficulty) {
    switch (difficulty) {
        case DIFFICULTY_EASY:
            return "简单";
        case DIFFICULTY_MEDIUM:
            return "中等";
        case DIFFICULTY_HARD:
            return "困难";
        default:
            return "未知";
    }
}

void loadHistoryFromFile(HistoryList *list, const char *filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string token;
        while (file >> token) {
            if (token.substr(0, 4) == "记录ID:") {
                int recordID = std::stoi(token.substr(4));
                
                file >> token; // 玩家ID:
                int playerID = std::stoi(token.substr(4));
                
                file >> token; // 玩家名称:
                char playerName[50];
                file >> playerName;
                
                file >> token; // 总得分:
                int score = std::stoi(token.substr(4));
                
                file >> token; // 等级:
                int level = std::stoi(token.substr(2));
                
                file >> token; // 消除总行数:
                int linesCleared = std::stoi(token.substr(6));
                
                file >> token; // 游戏时长(秒):
                int gameDuration = std::stoi(token.substr(8));
                
                file >> token; // 难度:
                int difficulty = std::stoi(token.substr(2));
                
                file >> token; // 最常使用方块:
                int mostUsedBlock = std::stoi(token.substr(6));
                
                file >> token; // 游戏日期:
                char gameDate[20];
                file >> gameDate;
                file >> token; // 时间部分
                strcat(gameDate, " ");
                strcat(gameDate, token.c_str());
                
                file >> token; // 方块使用统计:
                int blockUsage[7];
                for (int i = 0; i < 7; i++) {
                    file >> blockUsage[i];
                }
                
                HistoryNode *newNode = new HistoryNode;
                newNode->recordID = recordID;
                newNode->playerID = playerID;
                strcpy(newNode->playerName, playerName);
                newNode->score = score;
                newNode->level = level;
                newNode->linesCleared = linesCleared;
                newNode->gameDuration = gameDuration;
                newNode->difficulty = (HistoryDifficulty)difficulty;
                newNode->mostUsedBlock = mostUsedBlock;
                strcpy(newNode->gameDate, gameDate);
                for (int i = 0; i < 7; i++) {
                    newNode->blockUsage[i] = blockUsage[i];
                }
                newNode->next = NULL;
                
                if (list->head == NULL) {
                    list->head = newNode;
                } else {
                    newNode->next = list->head;
                    list->head = newNode;
                }
                list->count++;
            }
        }
        file.close();
    }
}

void saveHistoryToFile(HistoryList *list, const char *filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "========== 游戏历史记录 ==========" << std::endl;
        HistoryNode *current = list->head;
        while (current != NULL) {
            file << "记录ID:" << current->recordID << std::endl;
            file << "玩家ID:" << current->playerID << std::endl;
            file << "玩家名称:" << current->playerName << std::endl;
            file << "总得分:" << current->score << std::endl;
            file << "等级:" << current->level << std::endl;
            file << "消除总行数:" << current->linesCleared << std::endl;
            file << "游戏时长(秒):" << current->gameDuration << std::endl;
            file << "难度:" << current->difficulty << std::endl;
            file << "最常使用方块:" << current->mostUsedBlock << std::endl;
            file << "游戏日期:" << current->gameDate << std::endl;
            file << "方块使用统计:";
            for (int i = 0; i < 7; i++) {
                file << " " << current->blockUsage[i];
            }
            file << std::endl;
            file << std::endl;
            current = current->next;
        }
        
        file << "========== 玩家统计信息 ==========" << std::endl;
        if (list->head != NULL) {
            int playerID = list->head->playerID;
            char playerName[50];
            strncpy(playerName, list->head->playerName, 49);
            playerName[49] = '\0';
            int currentLevel = 1;
            int highScore = 0;
            int totalGames = 0;
            int totalScore = 0;
            int mostUsedBlock = 0;
            int difficultyPreference = 0;
            int totalGameDuration = 0;
            char lastGameDate[20] = {0};
            int consecutiveDays = 1;
            
            int blockUsage[7] = {0};
            int difficultyCount[3] = {0};
            
            current = list->head;
            while (current != NULL) {
                if (current->playerID == playerID) {
                    totalGames++;
                    totalScore += current->score;
                    totalGameDuration += current->gameDuration;
                    currentLevel = current->level;
                    
                    if (current->score > highScore) {
                        highScore = current->score;
                    }
                    
                    for (int i = 0; i < 7; i++) {
                        blockUsage[i] += current->blockUsage[i];
                    }
                    
                    difficultyCount[current->difficulty]++;
                    
                    strncpy(lastGameDate, current->gameDate, 19);
                    lastGameDate[19] = '\0';
                }
                current = current->next;
            }
            
            int maxBlockCount = 0;
            for (int i = 0; i < 7; i++) {
                if (blockUsage[i] > maxBlockCount) {
                    maxBlockCount = blockUsage[i];
                    mostUsedBlock = i;
                }
            }
            
            int maxDifficultyCount = 0;
            for (int i = 0; i < 3; i++) {
                if (difficultyCount[i] > maxDifficultyCount) {
                    maxDifficultyCount = difficultyCount[i];
                    difficultyPreference = i;
                }
            }
            
            file << "玩家ID:" << playerID << std::endl;
            file << "玩家名称:" << playerName << std::endl;
            file << "当前等级:" << currentLevel << std::endl;
            file << "历史最高分:" << highScore << std::endl;
            file << "总游戏次数:" << totalGames << std::endl;
            file << "总得分:" << totalScore << std::endl;
            file << "最常使用方块:" << mostUsedBlock << std::endl;
            file << "难度偏好:" << difficultyPreference << std::endl;
            file << "成就数量:0" << std::endl;
            file << "总游戏时长:" << totalGameDuration << std::endl;
            file << "最后游戏日期:" << lastGameDate << std::endl;
            file << "连续登录天数:" << consecutiveDays << std::endl;
        }
        
        file << "========== 全局最高分记录 ==========" << std::endl;
        int globalHighScore = 0;
        char highScorePlayer[50] = {0};
        char highScoreDate[20] = {0};
        
        current = list->head;
        while (current != NULL) {
            if (current->score > globalHighScore) {
                globalHighScore = current->score;
                strncpy(highScorePlayer, current->playerName, 49);
                highScorePlayer[49] = '\0';
                strncpy(highScoreDate, current->gameDate, 19);
                highScoreDate[19] = '\0';
            }
            current = current->next;
        }
        
        file << "最高分:" << globalHighScore << std::endl;
        file << "最高分玩家:" << highScorePlayer << std::endl;
        file << "最高分日期:" << highScoreDate << std::endl;
        
        file.close();
    }
}

void displayHistory(HistoryList *list) {
    HistoryNode *current = list->head;
    int index = 1;
    while (current != NULL) {
        std::cout << index << ". 记录ID:" << current->recordID << " 玩家ID:" << current->playerID 
                  << " 玩家名称:" << current->playerName << " 日期:" << current->gameDate 
                  << " 得分:" << current->score << " 等级:" << current->level 
                  << " 消除行数:" << current->linesCleared << " 时长:" << current->gameDuration 
                  << "秒 难度:" << getDifficultyString(current->difficulty) 
                  << " 最常使用方块:" << current->mostUsedBlock << std::endl;
        current = current->next;
        index++;
    }
}

int getHistoryCount(HistoryList *list) {
    return list->count;
}

void clearHistoryList(HistoryList *list) {
    HistoryNode *current = list->head;
    while (current != NULL) {
        HistoryNode *temp = current;
        current = current->next;
        delete temp;
    }
    list->head = NULL;
    list->count = 0;
}

int loadHighScore() {
    std::ifstream file("history.txt");
    int highScore = 0;
    if (file.is_open()) {
        std::string line;
        bool foundHighScore = false;
        while (std::getline(file, line)) {
            if (line.find("最高分:") != std::string::npos) {
                size_t pos = line.find("最高分:");
                if (pos != std::string::npos) {
                    std::string scoreStr = line.substr(pos + 4);
                    try {
                        highScore = std::stoi(scoreStr);
                        foundHighScore = true;
                        break;
                    } catch (const std::exception& e) {
                        highScore = 0;
                    }
                }
            }
        }
        file.close();
    }
    return highScore;
}

void saveHighScore(int score) {
    std::ifstream inputFile("history.txt");
    std::vector<std::string> lines;
    std::string line;
    
    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
    }
    
    std::ofstream outputFile("history.txt");
    if (outputFile.is_open()) {
        for (size_t i = 0; i < lines.size(); i++) {
            if (lines[i].find("最高分:") != std::string::npos) {
                outputFile << "最高分:" << score << std::endl;
            } else {
                outputFile << lines[i] << std::endl;
            }
        }
        outputFile.close();
    }
}
