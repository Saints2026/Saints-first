#ifndef HISTORY_H
#define HISTORY_H

// 难度级别
enum HistoryDifficulty {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_MEDIUM = 1,
    DIFFICULTY_HARD = 2
};

// 游戏记录节点
typedef struct HistoryNode {
    int recordID;                 // 记录ID
    int playerID;                // 玩家ID
    char playerName[50];         // 玩家名称
    int score;                   // 总得分
    int level;                   // 等级
    int linesCleared;            // 消除总行数
    int gameDuration;            // 游戏时长(秒)
    HistoryDifficulty difficulty; // 难度级别
    char gameDate[20];           // 游戏日期
    int blockUsage[7];           // 各类型方块使用次数统计
    int mostUsedBlock;           // 最常使用方块类型
    struct HistoryNode *next;    // 下一个节点
} HistoryNode;

// 游戏记录链表
typedef struct {
    HistoryNode *head;
    int count;
} HistoryList;

// 初始化游戏记录链表
void initHistoryList(HistoryList *list);

// 添加游戏记录
void addHistoryRecord(HistoryList *list, int playerID, const char *playerName, int score, 
                      int level, int linesCleared, int gameDuration, 
                      int difficulty, int blockUsage[7], int mostUsedBlock);

// 从文件加载游戏记录
void loadHistoryFromFile(HistoryList *list, const char *filename);

// 保存游戏记录到文件
void saveHistoryToFile(HistoryList *list, const char *filename);

// 显示游戏记录
void displayHistory(HistoryList *list);

// 获取记录数量
int getHistoryCount(HistoryList *list);

// 清理游戏记录链表
void clearHistoryList(HistoryList *list);

// 获取难度字符串
const char* getDifficultyString(int difficulty);

#endif // HISTORY_H
