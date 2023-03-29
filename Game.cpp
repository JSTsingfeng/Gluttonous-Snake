#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>

#define SNAKE_NUM 500//蛇的最大节数

#pragma comment( lib, "MSIMG32.LIB")
enum DIR //方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Snake //蛇
{
	int size;//节数
	int dir = RIGHT;//方向
	int speed;//速度
	POINT xy[SNAKE_NUM];//坐标
}snake;
struct Food //食物
{
	int x;//坐标
	int y;//坐标
	bool flag;//是否存在
}food;

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}

int snakeInit()//蛇的初始化
{
	snake.size = 3;
	snake.speed = 1;
	snake.dir = RIGHT;
	for(int i = 0; i < snake.size; i++)
	{
		snake.xy[i].x = 122.5 - 35 * i;
		snake.xy[i].y = 122.5;
	}
	return 0;
}
int bkInit()//背景初始化
{
	initgraph(900, 700, NULL);
	setorigin(0, 700);
	setaspectratio(1, -1);
	return 0;
}
int foodInit()//食物初始化
{
	srand(GetTickCount());//获取电脑运行时间为随机种子
	while (1)
	{
		food.x = 35 + rand() % 596;
		food.y = 35 + rand() % 596;
		if (food.x % 35 == 0 && food.y % 35 == 0)
			if(food.x + 18.5 <= snake.xy[0].x || food.x + 16.5 >= snake.xy[0].x || food.y + 18.5 <= snake.xy[0].y || food.y + 16.5 >= snake.xy[0].y)
			break;
	}
	//printf("x = %d, y = %d", food.x, food.y);
	food.flag = true;
	return 0;
}
int gameDraw()//绘制游戏画面
{
	BeginBatchDraw();//缓冲开始
	IMAGE bk;//绘制背景
	IMAGE fd;//绘制食物
	loadimage(&bk, "grass.png", 350, 70);
	int x = 0, y = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
			putimage(x + 350 * j, y, &bk);
		y += 70;

	}
	setbkmode(TRANSPARENT);
	for (int i = 0; i < snake.size; i++)//绘制蛇
	{
		setlinecolor(RGB(252, 73, 211));
		setfillcolor(RGB(252, 73, 211));
		fillrectangle(snake.xy[i].x - 17.5, snake.xy[i].y + 17.5, snake.xy[i].x + 17.5, snake.xy[i].y - 17.5);
	}
	if (food.flag)
	{
		loadimage(&fd, "apple.png", 35, 35);
		transparentimage(NULL, food.x, food.y, &fd, WHITE);
	}
	EndBatchDraw();//缓冲结束
	return 0;
}
int snakeMove()//蛇的移动
{
	for (int i = snake.size - 1; i > 0; i--)//身体的移动
		snake.xy[i] = snake.xy[i - 1];
	switch (snake.dir)//头的移动
	{
	case UP:
		snake.xy[0].y += 35;
		break;
	case DOWN:
		snake.xy[0].y -= 35;
		break;
	case LEFT:
		snake.xy[0].x -= 35;
		break;
	case RIGHT:
		snake.xy[0].x += 35;
		break;
	}
	return 0;
}
int eatFood()//吃食物
{
	if (food.flag)//若食物存在
	{
		if (food.x + 18.5 >= snake.xy[0].x && food.x + 16.5 <= snake.xy[0].x && food.y + 18.5 >= snake.xy[0].y && food.y + 16.5 <= snake.xy[0].y)
		{
			food.flag = false;
			snake.size++;//身体变长
		}
	}
	if (!food.flag)//若食物不存在则生成一个新食物
	{
		while (1)
		{
			food.x = 35 + rand() % 596;
			food.y = 35 + rand() % 596;
			if (food.x % 35 == 0 && food.y % 35 == 0)
				if (food.x + 18.5 <= snake.xy[0].x || food.x + 16.5 >= snake.xy[0].x || food.y + 18.5 <= snake.xy[0].y || food.y + 16.5 >= snake.xy[0].y)
					break;
		}
		food.flag = true;
	}
	return 0;
}
int keyControl()//键盘控制
{
	ExMessage m;
	peekmessage(&m, EX_KEY);
	if(m.message == WM_KEYDOWN)
		switch (m.vkcode)
		{
		case 'W':
			if(snake.dir != DOWN)
				snake.dir = UP;
			break;
		case 'S':
			if (snake.dir != UP)
				snake.dir = DOWN;
			break;
		case 'A':
			if (snake.dir != RIGHT)
				snake.dir = LEFT;
			break;
		case 'D':
			if (snake.dir != LEFT)
				snake.dir = RIGHT;
			break;
		case VK_ESCAPE :
			while (1)
				if (_getch() == 27)
					break;
		}
	return 0;
}

int mode()//模式
{
	for (int i = 1; i < snake.size; i++)//不能撞身体
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y)
		{
			//closegraph();
			break;
		}
	for (int i = 1; i < snake.size; i++)//不能撞墙
		if (snake.xy[0].x < 0 || snake.xy[0].x > 700 || snake.xy[0].y < 0 || snake.xy[0].y > 700)
		{
			closegraph();
			break;
		}
	/*for (int i = 1; i < snake.size; i++)//能穿墙
	{
		if (snake.xy[0].x < 0)
		{
			snake.xy[0].x = 682.5;
			break;
		}
		else if (snake.xy[0].x > 700)
		{
			snake.xy[0].x = 17.5;
			break;
		}
		else if (snake.xy[0].y < 0)
		{
			snake.xy[0].y = 682.5;
			break;
		}
		else if (snake.xy[0].y > 700)
		{
			snake.xy[0].y = 17.5;
			break;
		}
	}*/
	return 0;
}
