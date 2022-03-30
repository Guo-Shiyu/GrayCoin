#include "board.h"
#include <cassert>

bool Board::valid_site(int row, int col)
{
    return row >= 0 && row < Board::Height &&
           col >= 0 && col < Board::Width;
}

void Board::add_chessman(int row, int col, bool is_white)
{
    assert(valid_site(row, col));
    inner_[row][col] = is_white ? Board::White : Board::Black;
    cmdstack_.push_back({row, col});
}

int Board::query_chessman(int row, int col)
{
    assert(valid_site(row, col));
    return inner_[row][col];
}

void Board::regret_step(int step_num = 1)
{
    for (int i = 0; i < step_num; i++)
    {
        auto site = cmdstack_.back();
        inner_[site.row][site.col] = Board::Blank;
        cmdstack_.pop_back();
    }
}

int Board::win_or_blank()
{
    if (cmdstack_.empty())
        return Board::Blank;
    else
    {
        auto travse_and_check = [](int next_color, bool is_init = false)
        {
            static int record_color = INT_MAX;
            static int same = 0;
            if (is_init)
            {
                record_color = INT_MAX;
                same = 0;
            }
            else
            {
                auto cur_color = next_color;
                if (cur_color == Board::Blank)
                    return INT_MAX;

                if (record_color == INT_MAX)
                    record_color = cur_color;

                if (cur_color == record_color)
                {
                    if (++same == Board::WinRestriction)
                        return record_color;
                }
                else if (cur_color == Board::Blank)
                {
                    same = 0;
                    cur_color = INT_MAX;
                }
                else // opposite color
                {
                    same = 1;
                    record_color = cur_color;
                }
            }
        };

        auto final_site = cmdstack_.back();
        travse_and_check(INT_MAX, true);
        for (int i = 0; i < Board::Height; ++i)
        {
            if (auto ret = travse_and_check(inner_[i][final_site.col]); 
                ret == INT_MAX) // can not specify 
                continue;
            else 
                return ret;
        }
    }
}