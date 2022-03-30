#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>

class Coor
{
public:
    int i = 0;      // y
    int j = 0;      // x
    int expect = 0; // for AI use
};

class ChessMan
{
public:
    void draw();

public:
    int x = 0;              // x
    int y = 0;              // y
    int value = -1;         // black：1，white：0，blank：-1
    int modle;              // player mod
    bool isnew = false;     // chose
    int number = 0;         // score
    COLORREF color = GREEN; // backround
};

ChessMan Board[19][19]; // board
int win = -1;           // who win  white: 0, black: 1, unsolved: 2）
int who_round = 0;      // who's round
int player_color = 0;
int dx[4]{1, 0, 1, 1}; // 4 direction of : - | \ /
int dy[4]{0, 1, 1, -1};

void DrawBoard();
void Init();
Coor FindBestNextCoor(int color);
void CheckWin();
void MainLoop();

int main()
{
    initgraph(700, 700, NOMINIMIZE);
    setbkcolor(WHITE);
    cleardevice();
    setbkmode(TRANSPARENT);
    while (1)
    {
        Init();
        MainLoop();
        cleardevice();
    }
}

void ChessMan::draw()
{
    COLORREF thefillcolor = getfillcolor();
    setlinestyle(PS_SOLID, 2);
    setfillcolor(color);
    solidrectangle(x, y, x + 30, y + 30);
    if (isnew)
    {

        setlinecolor(LIGHTGRAY);
        line(x + 1, y + 2, x + 8, y + 2);
        line(x + 2, y + 1, x + 2, y + 8);
        line(x + 29, y + 2, x + 22, y + 2);
        line(x + 29, y + 1, x + 29, y + 8);
        line(x + 2, y + 29, x + 8, y + 29);
        line(x + 2, y + 22, x + 2, y + 29);
        line(x + 29, y + 29, x + 22, y + 29);
        line(x + 29, y + 22, x + 29, y + 29);
    }
    setlinecolor(BLACK);
    switch (modle)
    {
    case 0:
        line(x + 15, y, x + 15, y + 30);
        line(x - 1, y + 15, x + 30, y + 15);
        break;
        //  *
        // ***
        //  *
    case 1:
        line(x + 14, y + 15, x + 30, y + 15);
        setlinestyle(PS_SOLID, 3);
        line(x + 15, y, x + 15, y + 30);
        setlinestyle(PS_SOLID, 2);
        break;
        // *
        // ***
        // *
    case 2:
        line(x - 1, y + 15, x + 15, y + 15);
        setlinestyle(PS_SOLID, 3);
        line(x + 15, y, x + 15, y + 30);
        setlinestyle(PS_SOLID, 2);
        break;
        //   *
        // ***
        //   *
    case 3:
        line(x + 15, y + 15, x + 15, y + 30);
        setlinestyle(PS_SOLID, 3);
        line(x - 1, y + 15, x + 30, y + 15);
        setlinestyle(PS_SOLID, 2);
        break;
        // ***
        //  *
        //  *
    case 4:
        line(x + 15, y, x + 15, y + 15);
        setlinestyle(PS_SOLID, 3);
        line(x - 1, y + 15, x + 30, y + 15);
        setlinestyle(PS_SOLID, 2);
        break;
        //  *
        //  *
        // ***
    case 5:
        setlinestyle(PS_SOLID, 3);
        line(x + 15, y, x + 15, y + 15);
        line(x + 15, y + 15, x + 30, y + 15);
        setlinestyle(PS_SOLID, 2);
        break;
        // *
        // *
        // ***
    case 6:
        setlinestyle(PS_SOLID, 3);
        line(x + 15, y, x + 15, y + 15);
        line(x - 1, y + 15, x + 15, y + 15);
        setlinestyle(PS_SOLID, 2);
        break;
        //   *
        //   *
        // ***
    case 7:
        setlinestyle(PS_SOLID, 3);
        line(x - 1, y + 15, x + 15, y + 15);
        line(x + 15, y + 15, x + 15, y + 30);
        setlinestyle(PS_SOLID, 2);
        break;
        // ***
        //   *
        //   *
    case 8:
        setlinestyle(PS_SOLID, 3);
        line(x + 15, y + 15, x + 30, y + 15);
        line(x + 15, y + 15, x + 15, y + 30);
        setlinestyle(PS_SOLID, 2);
        break;
        // ***
        // *
        // *
    case 9:
        line(x + 15, y, x + 15, y + 30);
        line(x - 1, y + 15, x + 30, y + 15);
        setfillcolor(BLACK);
        setlinestyle(PS_SOLID, 1);
        fillcircle(x + 15, y + 15, 4);
        break;
        //  *
        // *O*
        //  *
    }
    switch (value)
    {
    case 0: // white
        setfillcolor(WHITE);
        setlinestyle(PS_SOLID, 1);
        fillcircle(x + 15, y + 15, 13);
        break;
    case 1: // black
        setfillcolor(BLACK);
        setlinestyle(PS_SOLID, 1);
        fillcircle(x + 15, y + 15, 13);
        break;
    }
    setfillcolor(thefillcolor);
}

void DrawBoard()
{
    int modle = 0;
    int number = 0;
    // number coordinate
    TCHAR strnum[19][3] = {_T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("13"), _T("14"), _T("15"), _T("16"), _T("17"), _T("18"), _T("19")};
    // letter cooedinate
    TCHAR strabc[19][3] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), _T("H"), _T("I"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), _T("O"), _T("P"), _T("Q"), _T("R"), _T("S")};
    for (int i = 0, k = 0; i < 570; i += 30)
    {
        for (int j = 0, g = 0; j < 570; j += 30)
        {
            Board[k][g].color = 0xFFFFFF; // backround color

            Board[k][g].x = 65 + j;
            Board[k][g].y = 50 + i;
            Board[k][g].number = 0;

            if (k == 0 && g == 0)
            {
                modle = 8;
            }
            else if (k == 0 && g == 18)
            {
                modle = 7;
            }
            else if (k == 18 && g == 18)
            {
                modle = 6;
            }
            else if (k == 18 && g == 0)
            {
                modle = 5;
            }
            else if (k == 0)
            {
                modle = 3;
            }
            else if (k == 18)
            {
                modle = 4;
            }
            else if (g == 0)
            {
                modle = 1;
            }
            else if (g == 18)
            {
                modle = 2;
            }
            else if ((k == 3 && g == 3) || (k == 3 && g == 15) || (k == 15 && g == 3) || (k == 15 && g == 15) || (k == 3 && g == 9) || (k == 9 && g == 3) || (k == 15 && g == 9) || (k == 9 && g == 15) || (k == 9 && g == 9))
            {
                modle = 9;
            }
            else
            {
                modle = 0;
            }
            Board[k][g].modle = modle;
            Board[k][g].draw();
            if (Board[k][g].isnew == true)
            {
                Board[k][g].isnew = false;
            }
            g++;
        }
        k++;
    }
    // mark around board
    LOGFONT nowstyle;
    gettextstyle(&nowstyle);
    settextstyle(0, 0, NULL);
    setbkcolor(0xffffff);
    for (int i = 0; i < 19; i++)
    {
        outtextxy(75 + number, 35, strnum[i]);
        outtextxy(53, 55 + number, strabc[i]);
        number += 30;
    }
    settextstyle(&nowstyle);
}

void Init()
{
    win = -1;
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            Board[i][j].number = 0;
            Board[i][j].value = -1;
        }
    }
}

Coor FindBestNextCoor(int color)
{
    // 评分表
    int Score[3][5] = {
        {0, 20, 360, 5800, 92840}, // 防守0子
        {0, 0, 20, 360, 92840},    // 防守1子
        {0, 0, 0, 0, 92840}        // 防守2子
    };
    Coor bestseat;            // 最佳位置
    int MAXnumber[361] = {0}; // 最佳分数（可能有多个）
    int MAXx[361] = {0};      // 最佳 x 坐标（可能有多个）
    int MAXy[361] = {0};      // 最佳 y 坐标（可能有多个）
    int number = 0;           // 下一个最佳分数储存位置
    int truenumber;           // 输出的最佳分数位置
    int nowi = 0;             // 现在遍历到的y坐标
    int nowj = 0;             // 现在遍历到的x坐标
    int length[4];            // 四个方向的长度
    int emeny[4];             // 四个方向的敌子
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            if (Board[i][j].value == -1)
            {

                Board[i][j].value = color;
                for (int k = 0; k < 4; k++)
                {
                    length[k] = 0;
                    emeny[k] = 0;
                    nowi = i;
                    nowj = j;
                    while (nowi <= 18 && nowj <= 18 && nowi >= 0 && nowj >= 0 && Board[nowi][nowj].value == color)
                    {
                        length[k]++;
                        nowj += dx[k];
                        nowi += dy[k];
                    }
                    if (Board[nowi][nowj].value == 1 - color || nowi < 0 || nowj < 0 || nowi > 18 || nowj > 18)
                    {
                        emeny[k]++;
                    }
                    nowi = i;
                    nowj = j;
                    while (nowi <= 18 && nowj <= 18 && nowi >= 0 && nowj >= 0 && Board[nowi][nowj].value == color)
                    {
                        length[k]++;
                        nowj -= dx[k];
                        nowi -= dy[k];
                    }
                    if (Board[nowi][nowj].value == 1 - color || nowi < 0 || nowj < 0 || nowi > 18 || nowj > 18)
                    {
                        emeny[k]++;
                    }
                    length[k] -= 2;
                    if (length[k] > 4)
                    {
                        length[k] = 4;
                    }
                    Board[i][j].number += Score[emeny[k]][length[k]] * 4 + !(!length[k]) * 20;
                    length[k] = 0;
                    emeny[k] = 0;
                }

                Board[i][j].value = 1 - color;
                for (int k = 0; k < 4; k++)
                {
                    length[k] = 0;
                    emeny[k] = 0;
                    nowi = i;
                    nowj = j;
                    while (nowi <= 18 && nowj <= 18 && nowi >= 0 && nowj >= 0 && Board[nowi][nowj].value == 1 - color)
                    {
                        length[k]++;
                        nowj += dx[k];
                        nowi += dy[k];
                    }
                    if (Board[nowi][nowj].value == color || nowi < 0 || nowj < 0 || nowi > 18 || nowj > 18)
                    {
                        emeny[k]++;
                    }
                    nowi = i;
                    nowj = j;
                    while (nowi <= 18 && nowj <= 18 && nowi >= 0 && nowj >= 0 && Board[nowi][nowj].value == 1 - color)
                    {
                        length[k]++;
                        nowj -= dx[k];
                        nowi -= dy[k];
                    }
                    if (Board[nowi][nowj].value == color || nowi < 0 || nowj < 0 || nowi > 18 || nowj > 18)
                    {
                        emeny[k]++;
                    }
                    length[k] -= 2;
                    if (length[k] > 4)
                    {
                        length[k] = 4;
                    }
                    Board[i][j].number += Score[emeny[k]][length[k]];
                    length[k] = 0;
                    emeny[k] = 0;
                }
                Board[i][j].value = -1;
            }
            if (Board[i][j].number == MAXnumber[0])
            {

                MAXnumber[number] = Board[i][j].number;
                MAXy[number] = i;
                MAXx[number] = j;
                number++;
            }
            if (Board[i][j].number > MAXnumber[0])
            {

                for (int k = 0; k < number; k++)
                {
                    MAXnumber[k] = 0;
                    MAXy[k] = 0;
                    MAXx[k] = 0;
                }
                number = 0;
                MAXnumber[number] = Board[i][j].number;
                MAXy[number] = i;
                MAXx[number] = j;
                number++;
            }
        }
    }

    srand(time(NULL));
    truenumber = rand() % number;
    bestseat.i = MAXy[truenumber];
    bestseat.j = MAXx[truenumber];
    bestseat.expect = MAXnumber[truenumber];

    return bestseat;
}

void CheckWin()
{
    bool isinit = true;
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            if (Board[i][j].value != -1)
            {
                isinit = false;
                int nowcolor = Board[i][j].value;
                int length[4] = {0, 0, 0, 0};
                for (int k = 0; k < 4; k++)
                {
                    int nowi = i;
                    int nowj = j;
                    while (nowi <= 18 && nowj <= 18 && nowi >= 0 && nowj >= 0 && Board[nowi][nowj].value == nowcolor)
                    {
                        length[k]++;
                        nowj += dx[k];
                        nowi += dy[k];
                    }
                    nowi = i;
                    nowj = j;
                    while (nowi <= 18 && nowj <= 18 && nowi >= 0 && nowj >= 0 && Board[nowi][nowj].value == 1 - nowcolor)
                    {
                        length[k]++;
                        nowj -= dx[k];
                        nowi -= dy[k];
                    }
                }
                for (int k = 0; k < 4; k++)
                {
                    if (length[k] >= 5)
                    { // 5 chessman in line
                        if (nowcolor == player_color)
                        {
                            win = player_color;
                        }
                        if (nowcolor == 1 - player_color)
                        {
                            win = 1 - player_color; // AI win
                        }
                    }
                }
                if ((!isinit) && FindBestNextCoor(player_color).expect == 0 && FindBestNextCoor(1 - player_color).expect == 0)
                {
                    win = 2; // unsolved
                }
            }
        }
    }
}

void MainLoop()
{

    int oldi = 0;
    int oldj = 0;
    // choose random coler
    srand(time(NULL));
    player_color = rand() % 2;
    solidrectangle(40, 25, 645, 630);

    settextstyle(30, 15, 0, 0, 0, 1000, false, false, false);
    settextcolor(BLACK);

    if (player_color == 0)
    {
        outtextxy(150, 650, _T("you are white "));
        who_round = 1;
    }
    else
    {
        outtextxy(150, 650, _T("you are black "));
        who_round = 0;
    }
    DrawBoard(); // 绘制
    while (1)
    {
    NEXTROUND:
        if (who_round == 0)
        {
            // player's round
            ExMessage mouse = getmessage(EM_MOUSE);
            for (int i = 0; i < 19; i++)
            {
                for (int j = 0; j < 19; j++)
                {
                    if (mouse.x > Board[i][j].x && mouse.x < Board[i][j].x + 30    // check x coor
                        && mouse.y > Board[i][j].y && mouse.y < Board[i][j].y + 30 // check y coor
                        && Board[i][j].value == -1)                                // is blank
                    {
                        if (mouse.message == WM_LBUTTONDOWN)
                        {
                            Board[i][j].value = player_color;
                            Board[i][j].isnew = true;
                            oldi = -1;
                            oldj = -1;

                            who_round = 1;
                            goto DRAW;
                        }
                        if (oldi != -1 && oldj != -1)
                        {

                            Board[oldi][oldj].isnew = false;
                            Board[oldi][oldj].draw();
                        }
                        Board[i][j].isnew = true;
                        Board[i][j].draw();
                        oldi = i;
                        oldj = j;
                    }
                }
            }
        }
        else
        {

            Coor best;
            best = FindBestNextCoor(1 - player_color);
            if (best.expect == 0)
            {

                int drawi = 9;
                int drawj = 9;
                while (Board[drawi][drawj].value != -1)
                {
                    drawi--;
                    drawj++;
                }
                Board[drawi][drawj].value = 1 - player_color;
                Board[drawi][drawj].isnew = true;
            }
            else
            {
                Board[best.i][best.j].value = 1 - player_color;
                Board[best.i][best.j].isnew = true;
            }
            who_round = 0;
            goto DRAW;
        }
    }
DRAW:
    CheckWin();
    DrawBoard();
    if (win == -1) // no one win
    {
        Sleep(500);
        goto NEXTROUND;
    }
    settextcolor(RED);
    Sleep(1000);
    if (win == 0)
    {
        outtextxy(320, 320, _T("white win"));
    }
    if (win == 1)
    {
        outtextxy(320, 320, _T("black win"));
    }
    if (win == 2)
    {
        outtextxy(320, 320, _T("continue~~"));
    }
    // wait time
    Sleep(5000);
    return;
}