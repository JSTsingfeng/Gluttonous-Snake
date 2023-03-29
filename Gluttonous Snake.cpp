
#include<conio.h>
#include<graphics.h>
#include"game.h"


int main()
{
    snakeInit();
    bkInit();
    foodInit();
    while (1)
    {
        gameDraw();
        keyControl();
        mode();
        snakeMove();
        eatFood();
        Sleep(200);
    }
}


