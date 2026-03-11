#include "player.h"
#include <fstream>
#include <cstring>
#include <iostream>

void initPlayerList(PlayerList *list) {
    list->head = NULL;
    list->count = 0;
}

void addPlayer(PlayerList *list, int playerID, const char *playerName) {
    PlayerNode *newNode = new PlayerNode;
    newNode->playerID = playerID;
    strncpy(newNode->playerName, playerName, 49);
    newNode->playerName[49] = '\0';
    newNode->totalGamesPlayed = 0;
    newNode->totalScore = 0;
    newNode->totalLinesCleared = 0;
    newNode->maxScore = 0;
    newNode->totalGameTime = 0;
    for (int i = 0; i < 7; i++) {
        newNode->blockUsage[i] = 0;
    }
    newNode->next = NULL;
    
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        PlayerNode *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->count++;
}

void updatePlayerData(PlayerList *list, int playerID, int score, int linesCleared, int gameTime, int blockType) {
    PlayerNode *player = findPlayerByID(list, playerID);
    if (player != NULL) {
        player->totalGamesPlayed++;
        player->totalScore += score;
        player->totalLinesCleared += linesCleared;
        player->totalGameTime += gameTime;
        if (score > player->maxScore) {
            player->maxScore = score;
        }
        if (blockType >= 0 && blockType < 7) {
            player->blockUsage[blockType]++;
        }
    }
}

PlayerNode* findPlayerByID(PlayerList *list, int playerID) {
    PlayerNode *current = list->head;
    while (current != NULL) {
        if (current->playerID == playerID) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void loadPlayerDataFromFile(PlayerList *list, const char *filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int playerID, totalGamesPlayed, totalScore, totalLinesCleared, maxScore, totalGameTime;
        char playerName[50];
        int blockUsage[7];
        
        while (file >> playerID >> playerName >> totalGamesPlayed >> totalScore >> totalLinesCleared 
               >> maxScore >> totalGameTime) {
            for (int i = 0; i < 7; i++) {
                file >> blockUsage[i];
            }
            
            PlayerNode *newNode = new PlayerNode;
            newNode->playerID = playerID;
            strncpy(newNode->playerName, playerName, 49);
            newNode->playerName[49] = '\0';
            newNode->totalGamesPlayed = totalGamesPlayed;
            newNode->totalScore = totalScore;
            newNode->totalLinesCleared = totalLinesCleared;
            newNode->maxScore = maxScore;
            newNode->totalGameTime = totalGameTime;
            for (int i = 0; i < 7; i++) {
                newNode->blockUsage[i] = blockUsage[i];
            }
            newNode->next = NULL;
            
            if (list->head == NULL) {
                list->head = newNode;
            } else {
                PlayerNode *current = list->head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newNode;
            }
            list->count++;
        }
        file.close();
    }
}

void savePlayerDataToFile(PlayerList *list, const char *filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        PlayerNode *current = list->head;
        while (current != NULL) {
            file << current->playerID << " " << current->playerName << " " 
                 << current->totalGamesPlayed << " " << current->totalScore << " "
                 << current->totalLinesCleared << " " << current->maxScore << " "
                 << current->totalGameTime;
            for (int i = 0; i < 7; i++) {
                file << " " << current->blockUsage[i];
            }
            file << std::endl;
            current = current->next;
        }
        file.close();
    }
}

void clearPlayerList(PlayerList *list) {
    PlayerNode *current = list->head;
    while (current != NULL) {
        PlayerNode *temp = current;
        current = current->next;
        delete temp;
    }
    list->head = NULL;
    list->count = 0;
}

int getMostUsedBlockType(PlayerList *list, int playerID) {
    PlayerNode *player = findPlayerByID(list, playerID);
    if (player != NULL) {
        int maxCount = 0;
        int maxType = 0;
        for (int i = 0; i < 7; i++) {
            if (player->blockUsage[i] > maxCount) {
                maxCount = player->blockUsage[i];
                maxType = i;
            }
        }
        return maxType;
    }
    return 0;
}
