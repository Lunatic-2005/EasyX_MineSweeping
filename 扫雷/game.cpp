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
		sprintf_s(imgPath, "Pics/%d.png", i); //sprintf��������ʽ������ַ��������浽ָ��ָ�루��imgPath����
		loadimage(&imgs[i], imgPath);
	}
}
void show_map(int map[][COL])
//���ڴ�ӡɨ�׵�ͼ����
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
	if (map[r][c] == 20)   //�˴�Ϊ0
	{
		map[r][c] -= 20;
		putimage(r * 40, c * 40, &imgs[map[r][c]]);

		openblank(r, c, map);
	}

	if (map[r][c] >= 21 && map[r][c] <= 28) //�˴�Ϊ1-8��
	{
		map[r][c] -= 20;
		putimage(r * 40, c * 40, &imgs[map[r][c]]);
	}
	else if (map[r][c] >= 29 && map[r][c] <= 38)
	{
		putimage(r * 40, c * 40, &imgs[11]);


		//�Ҽ�ʱ������if��䣬�����Ǳ�ʾ����
		//�Ҽ���+10���õ�����29-38
	}
	else if (map[r][c] == 19) //�㵽���ϣ���Ϸ��������ӡ���ű��
	{

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if (i == r && j == c)
				{
					putimage(i * 40, j * 40, &imgs[10]);//����˴��ĺ����
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
//����������;
{
	//�漴�����ף�-1��
	srand((unsigned)time(NULL));
	//������������ӣ�ʱ�����
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
	//������Χ�ľŹ����ڲ�����+1
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			//1���ҵ���
			if (map[i][j] == -1)
			{
				//2��������Χ�ľŹ���������+1��ע���ų��������׵�����
				for (int r = i - 1; r <= i + 1; r++)
				{
					for (int c = j - 1; c <= j + 1; c++)
					{
						if ((r >= 0) && (r <= ROW - 1) && (c >= 0) && (c <= COL - 1) && (map[r][c] != -1))
							//ȷ�����ʲ���Խ��
							//���ø��������ϵ�����
							++map[r][c];
					}
				}
			}
		}
	}
	//����
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
		//��GetMouseMsg������ȡ�����Ϣ���MOUSEMSG�ṹ��
		int r = msg.x / PICSIZE;
		int c = msg.y / PICSIZE;
		//������ת���������±�
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
	//r��cΪ��������±�
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
			if ((map[i][j] <= 8 && map[i][j] >= 0)/*�˴��Ѿ���ʾ����*/ || map[i][j] == 29/*�˴�Ϊ�����Ѿ����*/ || map[i][j] == 19/*�˴�Ϊ�ײ���δ���*/)
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


