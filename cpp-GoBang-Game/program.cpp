#include "program.h"

void GoBangGame::draw()
{
}

void GoBangGame::ai_update()
{
}

void GoBangGame::main_loop()
{
    initgraph(600, 600);
    draw();
    while (true)
    {
        while (MouseHit()) // 当有鼠标消息的时候执行
        {
            auto msg = GetMouseMsg(); // 获取鼠标消息

            switch (msg.uMsg) // 根据不同的鼠标消息，执行不同的代码
            {
            case WM_LBUTTONDOWN:
                circle(msg.x, msg.y, 10);
                break;
            case WM_RBUTTONDOWN:
                rectangle(msg.x - 20, msg.y - 20, msg.y - 20, msg.y + 20);
                break;
            }
        }
    }
}

void GoBangGame::update()
{
}
