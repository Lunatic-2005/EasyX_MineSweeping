#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<easyx.h>
#include"game.h"
#include<graphics.h>

int main()
{
	entrance();
    return 0;
}

void load_test()
{
	for (int i = 0; i <= 12; i++)
	{
		char imgPath[80] = { 0 };
		sprintf_s(imgPath, "Pics/%d.png", i); //sprintf可用来格式化输出字符串并保存到指定指针（如imgPath）中
		loadimage(&imgs[i], imgPath);
	}
}
void show_map(int map[][COL])
//用于打印扫雷地图数组
{
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			printf("%d\t", map[i][k]);
		}
		printf("\n");
	}
}
void paint_front(int map[][COL])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			putimage(i * 40, j * 40, &imgs[12]);

		}
	}
}
void paint(int map[][COL], int r, int c)
{
	if (map[r][c] == 20)   //此处为0
	{
		map[r][c] -= 20;
		putimage(r * 40, c * 40, &imgs[map[r][c]]);

		openblank(r, c, map);
	}

	if (map[r][c] >= 21 && map[r][c] <= 28) //此处为1-8，
	{
		map[r][c] -= 20;
		putimage(r * 40, c * 40, &imgs[map[r][c]]);
	}
	else if (map[r][c] >= 29 && map[r][c] <= 38)
	{
		putimage(r * 40, c * 40, &imgs[11]);


		//右键时来到此if语句，作用是表示旗帜
		//右键后+10，得到的是29-38
	}
	else if (map[r][c] == 19) //点到雷上，游戏结束，打印整张表格
	{

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if (i == r && j == c)
				{
					putimage(i * 40, j * 40, &imgs[10]);//输出此处的红底雷
				}
				else if (map[i][j] >= 21 && map[i][j] <= 28)
				{
					map[i][j] -= 20;
					putimage(i * 40, j * 40, &imgs[map[i][j]]);
				}
				else if (map[i][j] == 19 && (i != r || j != c))
				{
					putimage(i * 40, j * 40, &imgs[9]);
				}
				else if (map[i][j] == 20)
					putimage(i * 40, j * 40, &imgs[0]);

			}
		}
		islost();
	}
}
void init(int map[][COL])
//用于设置雷;
{
	//随即设置雷（-1）
	srand((unsigned)time(NULL));
	//设置随机数种子（时间戳）
	int i = 0;
	while (i < MINENUM)
	{
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
			continue;
		}
		else
		{
			continue;
		}
	}
	//把雷周围的九宫格内部数字+1
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			//1、找到雷
			if (map[i][j] == -1)
			{
				//2、将雷周围的九宫格内数字+1，注意排除其中有雷的坐标
				for (int r = i - 1; r <= i + 1; r++)
				{
					for (int c = j - 1; c <= j + 1; c++)
					{
						if ((r >= 0) && (r <= ROW - 1) && (c >= 0) && (c <= COL - 1) && (map[r][c] != -1))
							//确保访问不会越界
							//设置各个坐标上的数字
							++map[r][c];
					}
				}
			}
		}
	}
	//加密
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < ROW; j++)
		{
			map[i][j] += 20;
		}
	}
}
void play(int map[][COL])
{
	while (true)
	{
	begin:
		MOUSEMSG msg = GetMouseMsg();
		//用GetMouseMsg函数获取鼠标信息存进MOUSEMSG结构中
		int r = msg.x / PICSIZE;
		int c = msg.y / PICSIZE;
		//将坐标转换成数组下标
		switch (msg.uMsg)
		{
		case WM_RBUTTONDOWN:
		{
			if (map[r][c] >= 19 && map[r][c] <= 28)
			{
				map[r][c] += 10;
				paint(map, r, c);
				break;
			}
			if (map[r][c] >= 29 && map[r][c] <= 38)
			{
				map[r][c] -= 10;
				putimage(r * 40, c * 40, &imgs[12]);
				break;
			}
		}
		case WM_LBUTTONDOWN:
		{
			paint(map, r, c);
			judgewin(map);

		}

		}

	}
}
void openblank(int r, int c, int map[ROW][COL])
{
	//r与c为点击格子下标
	if (map[r][c] == 0)
	{
		for (int k = r - 1; k <= r + 1; k++)
		{
			for (int m = c - 1; m <= c + 1; m++)
			{
				if ((k >= 0 && k < ROW && m >= 0 && m < COL) && map[k][m] >= 19 && map[k][m] <= 28)
				{
					map[k][m] -= 20;
					//map[r][c] -= 20;

					putimage(k * 40, m * 40, &imgs[map[k][m]]);

					openblank(k, m, map);
				}
			}
		}
	}
}
void judgewin(int map[][COL])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if ((map[i][j] <= 8 && map[i][j] >= 0)/*此处已经显示数字*/ || map[i][j] == 29/*此处为雷且已经标记*/ || map[i][j] == 19/*此处为雷并且未标记*/)
			{
				continue;
			}

			else if (map[i][j] >= 20 && map[i][j] <= 28)
			{
				goto ctn;
			}

		}
	}
win:
	iswin();

ctn:
	;
}
void entrance()
{
	int map[20][20] = { 0 };
	int choose = MessageBox(GetHWnd(), "do you wanna play it now?", "CHOOSE", MB_OKCANCEL);
	switch (choose)
	{
	case 1:
	{
		initgraph(800, 800, SHOWCONSOLE);
		init(map);
		load_test();
	/*	show_map(map);*/
		paint_front(map);
		play(map);
		entrance();
		int ret = getchar();
	}
	case 0:
		exit(666);
	}

}
void iswin()
{
	int ret = MessageBox(GetHWnd(), "you win!\nclick ok to play again,or click cancel to quit.", "attention", MB_OKCANCEL);
	switch (ret)
	{
	case 1:
		entrance();
	case 2:
		exit(666);
	}
}
void islost()
{
	int ret = MessageBox(GetHWnd(), "you lose!\nclick ok to play again,or click cancel to quit.", "attention", MB_OKCANCEL);
	switch (ret)
	{
	case 1:
		entrance();
	case 2:
		exit(666);
	}
}


