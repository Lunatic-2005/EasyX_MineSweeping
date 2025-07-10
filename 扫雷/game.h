#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define ROW 20
#define COL 20
#define MINENUM 50
#define PICSIZE 40

//#pragma warning(disable : 4996)
IMAGE imgs[20];
void load_test();
void show_map(int map[][COL]);
void paint_front(int map[][COL]);
void paint(int map[][COL], int r, int c);
void init(int map[ROW][COL]);
void play(int map[][COL]);
void openblank(int r, int c, int map[][COL]);
void judgewin(int map[][COL]);
void entrance();
void iswin();
void islost();



