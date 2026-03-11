#ifndef PLAYER_H
#define PLAYER_H

// 玩家数据节点
typedef struct PlayerNode {
    int playerID;                 // 玩家ID
    char playerName[50];         // 玩家名称
    int totalGamesPlayed;        // 总游戏次数
    int totalScore;              // 总得分
    int totalLinesCleared;       // 总消除行数
    int maxScore;                // 最高分
    int totalGameTime;           // 总游戏时长(秒)
    int blockUsage[7];           // 各类型方块使用次数统计
    struct PlayerNode *next;     // 下一个节点
} PlayerNode;

// 玩家数据链表
typedef struct {
    PlayerNode *head;
    int count;
} PlayerList;

// 初始化玩家链表
void initPlayerList(PlayerList *list);

// 添加玩家
void addPlayer(PlayerList *list, int playerID, const char *playerName);

// 更新玩家数据
void updatePlayerData(PlayerList *list, int playerID, int score, int linesCleared, int gameTime, int blockType);

// 从文件加载玩家数据
void loadPlayerDataFromFile(PlayerList *list, const char *filename);

// 保存玩家数据到文件
void savePlayerDataToFile(PlayerList *list, const char *filename);

// 根据ID查找玩家
PlayerNode* findPlayerByID(PlayerList *list, int playerID);

// 清理玩家链表
void clearPlayerList(PlayerList *list);

// 获取最常使用方块类型
int getMostUsedBlockType(PlayerList *list, int playerID);

#endif // PLAYER_H
