#include "board.h"
#include <graphics.h>

class GoBangGame
{
public:
private:
    Board board_;
    bool ai_;

public:
    GoBangGame(bool use_ai): board_(), ai_(use_ai) {}
    GoBangGame(const GoBangGame& other) = default;
    ~GoBangGame() = default;

private:
    void draw();
    void ai_update();
public:
    void main_loop();
    void update();
};