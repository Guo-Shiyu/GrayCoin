#pragma once
#include <vector>

struct OpRecord
{
    int row, col;
};

class Board
{
public:
    static constexpr int
        White = -1,
        Blank = 0,
        Black = 1;

    static constexpr int Width = 15, Height = 15;
    static constexpr int WinRestriction = 5;

private:
    int inner_[Height][Width] = {Blank};
    std::vector<OpRecord> cmdstack_;

public:
    Board() = default;
    Board(const Board& other) = default;
    ~Board() = default;

private:
    // 判断合法坐标
    bool valid_site(int row, int col);

public:
    // 在棋盘上新增加一个棋子
    void add_chessman(int row, int col, bool is_white);
    
    // 查询指定位置上的棋子颜色（white/black/blank)
    int  query_chessman(int row, int col);

    // 悔棋
    void regret_step(int step_num = 1);
    
    // 判断输赢方（白， 黑， 未分胜负）
    int  win_or_blank();
};