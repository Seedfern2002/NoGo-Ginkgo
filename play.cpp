#include<iostream>
#include<cstring>
#include<string>
#include<cstdio>
#include<math.h>
#include<cmath>
#include<time.h>
#include<graphics.h>
#include<stdio.h>
#include<fstream>
using namespace std;
int board[9][9];
int board_2D[9][9];
int board_connection[9][9];
int board_connection2[9][9];
int board_connection3[9][9];
int board_nihility[9][9];
int board_rem[9][9];
int board_mem[81][9][9];
int mx[] = { -1,-1,1,1 };
int my[] = { 1,-1,1,-1 };
bool board_lost[9][9];
int pace = 1;
int mode = 0;
int remain = 0;
int situation = 0;
int situation2 = 2;
int turn=-1;
int connection = 0;
int flag_connection = 1;
int back = 5;
int x, y;
int cx[] = { -1,0,1,0 };
int cy[] = { 0,-1,0,1 };
int x_past1, y_past1;
int y_past2, x_past2;
clock_t clk1, clk2;
bool inBoard(int i, int j);
bool Check_available(int i, int j, int turn);
bool judge_eye(int i, int j, int turn);
bool judge_atari(int i, int j, int turn);
bool tiger(int i, int j, int turn);
bool last(int a[9][9], int turn, int pace);
int liberty(int i, int j, int turn, int depth);
double mark_eye(int i, int j, int turn);
double mark_atari(int i, int j, int turn);
double mark_opening(int a[9][9]);
double mark(int a[9][9], int turn);
int mark_tableware(int turn);
double alphabeta(int x, int y, int depth, int alpha, int beta, int turn, int situation);
void series_connection(int i, int j, int turn, int depth);
void nihility(int i, int j, int turn, int depth);
void reversion();
void selection();
void select();
void selection2();
void select2();
void initGame();
void playNoGo();
void bot();
void cundang();
void dudang();
int main()
{
	selection();
	select();
	return 0;
}
void selection()
{
	initgraph(400, 400);
	setbkcolor(RGB(222, 158, 68));
	cleardevice();
	setfillcolor(RGB(198, 230, 232));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("Consolas"));
	fillrectangle(150, 125, 250, 175);
	RECT r1 = { 150, 125, 250, 175 };
	drawtext(_T("Start the Game"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fillrectangle(150, 225, 250, 275);
	RECT r4 = { 150, 225, 250, 275 };
	drawtext(_T("Continue the Last Game"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setlinecolor(RGB(222, 158, 68));
	rectangle(150, 225, 250, 275);
	rectangle(150, 125, 250, 175);
}
void select()
{
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x >= 150 && m.x <= 250)
		{
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
			if (m.y >= 125 && m.y <= 175)
			{
				setlinecolor(RGB(192,44,56));
				rectangle(150, 125, 250, 175);
			}
			else if (m.y >= 225 && m.y <= 275)
			{
				setlinecolor(RGB(192, 44, 56));
				rectangle(150, 225, 250, 275);
			}
			else
			{
				setlinecolor(RGB(222, 158, 68));
				rectangle(150, 225, 250, 275);
				rectangle(150, 125, 250, 175);
			}
		}
		else
		{
			setlinecolor(RGB(222, 158, 68));
			rectangle(150, 225, 250, 275);
			rectangle(150, 125, 250, 175);
		}
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 150 && m.x <= 250)
			{
				if (m.y >= 125 && m.y <= 175)
				{
					pace = 0;
					back = 5;
					selection2();
					select2();
					return;
				}
				else if (m.y >= 225 && m.y <= 275)
				{
					dudang();
					return;
				}
			}
		}
	}
}
void selection2()
{
	initgraph(400, 600);
	setbkcolor(RGB(222, 158, 68));
	cleardevice();
	setfillcolor(RGB(198, 230, 232));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("Consolas"));
	fillrectangle(120, 125, 280, 175);
	RECT r1 = { 120, 125, 280, 175 };
	drawtext(_T("Select your NoGo stone"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fillrectangle(120, 425, 280, 475);
	RECT r2 = { 120, 425, 280, 475 };
	drawtext(_T("Return"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfillcolor(BLACK);
	solidcircle(170,250, 20);
	setfillcolor(WHITE);
	solidcircle(230, 250, 20);
}
void select2()
{
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x >= 150 && m.x <= 190 && m.y >= 230 && m.y <= 270)
		{
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
			setlinecolor(RGB(192, 44, 56));
			circle(170, 250, 20);
		}
		else if (m.x >= 210 && m.x <= 250 && m.y >= 230 && m.y <= 270)
		{
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
			setlinecolor(RGB(192, 44, 56));
			circle(230, 250, 20);
		}
		else if (m.x >= 120 && m.x <= 280 && m.y >= 425 && m.y <= 475)
		{
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
			setlinecolor(RGB(192, 44, 56));
			rectangle(120, 425, 280, 475);
		}
		else
		{
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
			setlinecolor(RGB(222, 158, 68));
			circle(170, 250, 20);
			circle(230, 250, 20);
			rectangle(120, 425, 280, 475);
		}
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 150 && m.x <= 190 && m.y >= 230 && m.y <= 270)
			{
				initGame();
				mode = 0;
				playNoGo();
				return;
			}
			if (m.x >= 210 && m.x <= 250 && m.y >= 230 && m.y <= 270)
			{
				initGame();
				mode = 1;
				setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
				setfillcolor(BLACK);
				solidcircle(60, 60, 20);
				setlinecolor(RGB(192, 44, 56));
				circle(60, 60, 20);
				board[0][0] = 1;
				board_mem[pace][0][0] = 1;
				x_past1 = 0;
				y_past1 = 0;
				playNoGo();
				return;
			}
			if (m.x >= 120 && m.x <= 280 && m.y >= 425 && m.y <= 475)
			{
				selection();
				select();
				return;
			}
		}
	}
}
void initGame()
{
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
	initgraph(900, 600);
	setbkcolor(RGB(222, 158, 68));
	cleardevice();
	initgraph(900, 600);
	setbkcolor(RGB(222, 158, 68));
	cleardevice();
	setlinecolor(BLACK);
	for (int i = 60; i <= 540; i += 60)
	{
		line(60, i, 540, i);
		line(i, 60, i, 540);
	}
	setfillcolor(RGB(198, 230, 232));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("Consolas"));
	fillrectangle(670, 125, 770, 175);
	RECT r1 = { 670, 125, 770, 175 };
	drawtext(_T("New Start"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fillrectangle(670, 225, 770, 275);
	RECT r2 = { 670, 225, 770, 275 };
	drawtext(_T("Give Up"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fillrectangle(670, 325, 770, 375);
	RECT r3 = { 670, 325, 770, 375 };
	drawtext(_T("Undo"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fillrectangle(670, 425, 770, 475);
	RECT r4 = { 670, 425, 770, 475 };
	drawtext(_T("Save"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void playNoGo()
{
	int a, b;
	int depth;
	bool start = 0;
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.x >= 670 && m.x <= 770)
		{
			setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
			if (m.y >= 125 && m.y <= 175)
			{
				setlinecolor(RGB(192, 44, 56));
				rectangle(670, 125, 770, 175);
			}
			else if (m.y >= 225 && m.y <= 275)
			{
				setlinecolor(RGB(192, 44, 56));
				rectangle(670, 225, 770, 275);
			}
			else if (m.y >= 325 && m.y <= 375)
			{
				setlinecolor(RGB(192, 44, 56));
				rectangle(670, 325, 770, 375);
			}
			else if (m.y >= 425 && m.y <= 475)
			{
				setlinecolor(RGB(192, 44, 56));
				rectangle(670, 425, 770, 475);
			}
			else
			{
				setlinecolor(RGB(222, 158, 68));
				rectangle(670, 425, 770, 475);
				rectangle(670, 125, 770, 175);
				rectangle(670, 225, 770, 275);
				rectangle(670, 325, 770, 375);
			}
		}
		else
		{
			setlinecolor(RGB(222, 158, 68));
			rectangle(670, 425, 770, 475);
			rectangle(670, 125, 770, 175);
			rectangle(670, 225, 770, 275);
			rectangle(670, 325, 770, 375);
		}
		for (int i = 1; i <= 9; i++)
		{
			for (int j = 1; j <= 9; j++)
			{
				if (abs(m.x - i * 60) < 12 && abs(m.y - j * 60) < 12)
				{
					x = i * 60;
					y = j * 60;
					a = i - 1;
					b = j - 1;
				}
			}
		}
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 670 && m.x <= 770)
			{
				if (m.y >= 125 && m.y <= 175)
				{
					memset(board, 0, sizeof(board));
					pace = 0;
					back = 5;
					selection();
					select();
				}
				if (m.y >= 225 && m.y <= 275)
				{
					MessageBox(NULL, TEXT("Failure is the mother of success"), TEXT("Don't worry"), MB_OK);
					closegraph();
					return;
				}
				if (m.y >= 325 && m.y <= 375)
				{
					if (pace <= 10)
					{
						MessageBox(NULL, TEXT("You cannot undo in the first 10 actions"), TEXT("Hi"), MB_OK);
					}
					else if (pace > 10)
					{
						if (back == 0)
						{
							MessageBox(NULL, TEXT("Too many Undo"), TEXT("Sorry"), MB_OK);
						}
						else {
							memset(board, 0, sizeof(board));
							setbkcolor(RGB(222, 158, 68));
							cleardevice();
							setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
							setlinecolor(BLACK);
							for (int i = 60; i <= 540; i += 60)
							{
								line(60, i, 540, i);
								line(i, 60, i, 540);
							}
							setfillcolor(RGB(198, 230, 232));
							settextcolor(BLACK);
							setbkmode(TRANSPARENT);
							settextstyle(20, 0, _T("Consolas"));
							fillrectangle(670, 125, 770, 175);
							RECT r1 = { 670, 125, 770, 175 };
							drawtext(_T("New Start"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							fillrectangle(670, 225, 770, 275);
							RECT r2 = { 670, 225, 770, 275 };
							drawtext(_T("Give Up"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							fillrectangle(670, 325, 770, 375);
							RECT r3 = { 670, 325, 770, 375 };
							drawtext(_T("Undo"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							fillrectangle(670, 425, 770, 475);
							RECT r4 = { 670, 425, 770, 475 };
							drawtext(_T("Save"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							for (int i = 0; i <= 8; i++)
							{
								for (int j = 0; j <= 8; j++)
								{
									board[i][j] = board_mem[pace - 1][i][j];
									if (mode == 0)
									{
										if (board_mem[pace - 1][i][j] == -1)
										{
											setfillcolor(BLACK);
											solidcircle((i + 1) * 60, (j + 1) * 60, 20);
										}
										else if (board_mem[pace - 1][i][j] == 1)
										{
											setfillcolor(WHITE);
											solidcircle((i + 1) * 60, (j + 1) * 60, 20);
										}
									}
									else if (mode == 1)
									{
										if (board_mem[pace - 1][i][j] == -1)
										{
											setfillcolor(WHITE);
											solidcircle((i + 1) * 60, (j + 1) * 60, 20);
										}
										else if (board_mem[pace - 1][i][j] == 1)
										{
											setfillcolor(BLACK);
											solidcircle((i + 1) * 60, (j + 1) * 60, 20);
										}
									}
								}
							}
							pace--;
							back--;
						}
					}
				}
				if (m.y >= 425 && m.y <= 475)
				{
					MessageBox(NULL, TEXT("Save successfully"), TEXT("Reminder"), MB_OK);
					cundang();
				}
			}
			if (m.x > 30 && m.y > 30 && m.x < 570 && m.y < 570)
			{
				if (board[a][b] != 0)
				{
					MessageBox(NULL, TEXT("Here exists stone"), TEXT("Reminder"), MB_OK);
					continue;
				}
				if (!Check_available(a, b, turn))
				{
					MessageBox(NULL, TEXT("Illegal operation"), TEXT("Reminder"), MB_OK);
					continue;
				}
				if (mode == 0)
				{
					setfillcolor(BLACK);
				}
				else if (mode==1)
				{
					setfillcolor(WHITE);
				}
				solidcircle(x, y, 20);
				setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
				setlinecolor(RGB(192, 44, 56));
				circle(x, y, 20);
				if (start||mode==1)
				{
					setlinecolor(RGB(222, 158, 68));
					circle(x_past1 * 60 + 60, y_past1 * 60 + 60, 20);
				}
				start = 1;
				board[a][b] = turn;
				x_past2 = a;
				y_past2 = b;
				turn *=-1;
				pace++;

				bot();
				int cflag = 1;

				for (int i = 0; i <= 8; i++)
				{
					for (int j = 0; j <= 8; j++)
					{
						if (board[i][j] == 0)
						{
							if (Check_available(i, j, -1))
							{
								cflag = 0;
								break;
							}
						}
						if (cflag == 0)
							break;
					}
				}
				if (cflag)
				{
					MessageBox(NULL, TEXT("您失败了"), TEXT("可惜"), MB_OK);
					closegraph();
					return;
				}
			}
		}
	}
}
void bot()
{
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (Check_available(i, j, 1))
			{
				remain++;
			}
		}
	}
	int depth;
	clk1 = clock();
	int cflag = 1;
	if (pace <= 13)
	{
		situation = 0;
		depth = 2;
	}
	else if (pace > 13)
	{
		situation = 1;
		if (pace <= 19)
			depth = 2;
		else if (pace > 19 && pace <= 25)
			depth = 3;
		else if (pace > 25)
			depth = 4;
	}
	if (remain < 12 && remain > 7)
	{
		situation = 1;
		depth = 5;
		situation2 = 1;
	}
	if (remain <= 7)
	{
		situation = 2;
		depth = 10;
		situation2 = 1;
	}

	int ans_i, ans_j;
	int ans_i1, ans_j1;
	int bflag = 0;
	int mmax = -99999;
	ans_i1 = 0;
	ans_j1 = 0;
	int depth1 = 2;
	if (pace > 30)
	{
		depth1 = 4;
	}
	if (remain <= 7)
	{
		depth1 = 6;
	}
	int depth2 = min(depth1, depth - 1);
	for (int k = 1; k <= 9; k++)
	{
		if (y_past2 - k >= 0 && y_past2 - k <= 8)
		{
			for (int i = x_past2 - k; i <= k + x_past2; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y_past2 - k, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(i, y_past2 - k, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = i;
						ans_j1 = y_past2 - k;
						mmax = mark_now;
					}
				}
			}
		}
		if (y_past2 + k >= 0 && y_past2 + k <= 8)
		{
			for (int i = x_past2 - k; i <= k + x_past2; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y_past2 + k, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(i, y_past2 + k, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = i;
						ans_j1 = y_past2 + k;
						mmax = mark_now;
					}
				}
			}
		}
		if (x_past2 + k >= 0 && x_past2 + k <= 8)
		{
			for (int j = y_past2 - k + 1; j < k + y_past2; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x_past2 + k, j, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(x_past2 + k, j, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = x_past2 + k;
						ans_j1 = j;
						mmax = mark_now;
					}
				}
			}
		}
		if (x_past2 - k >= 0 && x_past2 - k <= 8)
		{
			for (int j = y_past2 - k + 1; j < k + y_past2; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x_past2 - k, j, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(x_past2 - k, j, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = x_past2 - k;
						ans_j1 = j;
						mmax = mark_now;
					}
				}
			}
		}
	}
	mmax = -99999;
	int ans_i2, ans_j2;
	int tflag = 1;
	for (int k = 1; k <= 9; k++)
	{
		if (y_past2 - k >= 0 && y_past2 - k <= 8)
		{
			for (int i = x_past2 - k; i <= k + x_past2; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y_past2 - k, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(i, y_past2 - k, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i2 = i;
						ans_j2 = y_past2 - k;
						mmax = mark_now;
					}
				}
			}
		}
		if (tflag == 0)
			break;
		if (y_past2 + k >= 0 && y_past2 + k <= 8)
		{
			for (int i = x_past2 - k; i <= k + x_past2; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y_past2 + k, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(i, y_past2 + k, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i2 = i;
						ans_j2= y_past2 + k;
						mmax = mark_now;
					}
				}
			}
		}
		if (tflag == 0)
			break;
		if (x_past2 + k >= 0 && x_past2 + k <= 8)
		{
			for (int j = y_past2 - k + 1; j < k + y_past2; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x_past2 + k, j, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(x_past2 + k, j, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i2 = x_past2 + k;
						ans_j2 = j;
						mmax = mark_now;
					}
				}
			}
		}
		if (tflag == 0)
			break;
		if (x_past2 - k >= 0 && x_past2 - k <= 8)
		{
			for (int j = y_past2 - k + 1; j < k + y_past2; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x_past2 - k, j, 1))
				{
					cflag = 0;
					int mark_now = alphabeta(x_past2 - k, j, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i2 = x_past2 - k;
						ans_j2 = j;
						mmax = mark_now;
					}
				}
			}
			if (tflag == 0)
				break;
		}
	}
	if (tflag)
	{
		ans_i = ans_i1, ans_j = ans_j1;
	}
	else if (tflag == 0)
	{
		ans_i = ans_i1, ans_j = ans_j1;
	}
	if (cflag)
	{
		MessageBox(NULL, TEXT("您取得胜利"), TEXT("恭喜"), MB_OK);
		closegraph();
		return;
	}
	board[ans_i][ans_j] = 1;
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			board_mem[pace][i][j] = board[i][j];
		}
	}
	if (mode == 1)
	{
		setfillcolor(BLACK);
	}
	else if (mode == 0)
	{
		setfillcolor(WHITE);
	}
	solidcircle((ans_i + 1) * 60, (ans_j + 1) * 60, 20);
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
	setlinecolor(RGB(192, 44, 56));
	circle((ans_i + 1) * 60, (ans_j + 1) * 60, 20);
	setlinecolor(RGB(222, 158, 68));
	circle(x_past2 * 60 + 60, y_past2 * 60 + 60, 20);
	x_past1 = ans_i;
	y_past1 = ans_j;
	turn *= -1;
}
bool inBoard(int i, int j)
{
	if (i >= 0 && i <= 8 && j >= 0 && j <= 8)
		return true;
	return false;
}
bool Check_available(int i, int j, int turn)
{
	if (board[i][j] != 0)
		return false;
	else
	{
		board[i][j] = turn;
		if (liberty(i, j, turn, 0) == 0)
		{
			board[i][j] = 0;
			return false;
		}
		else
		{
			for (int k = 0; k < 4; k++)
			{
				if (inBoard(i + cx[k], j + cy[k]) && board[i + cx[k]][j + cy[k]] == -turn)
					if (liberty(i + cx[k], j + cy[k], -turn, 0) == 0)
					{
						board[i][j] = 0;
						return false;
					}
			}
		}
	}
	board[i][j] = 0;
	return true;
}
bool judge_eye(int i, int j, int turn)
{
	if (!Check_available(i, j, turn))
		return false;
	for (int k = 0; k < 4; k++)
	{
		if (inBoard(i + cx[k], j + cy[k]))
			if (board[i + cx[k]][j + cy[k]] == 0 || board[i + cx[k]][j + cy[k]] == -turn)
				return false;
	}
	board_lost[i][j] = 1;
	return true;
}
bool judge_atari(int i, int j, int turn)
{
	if (liberty(i, j, turn, 0) == 1)
	{
		for (int k = 0; k < 4; k++)
		{
			if (inBoard(i + cx[k], j + cy[k]) && board[i + cx[k]][j + cy[k]] == 0 && !judge_eye(i + cx[k], j + cy[k], turn) && Check_available(i + cx[k], j + cy[k], turn))
			{
				if (board_lost[i + cx[k]][j + cy[k]])
				{
					return false;
				}
				board_lost[i + cx[k]][j + cy[k]] = 1;
				return true;
			}
		}
	}
	return false;
}
bool tiger(int i, int j, int turn)
{
	if (board[i][j] != 0)
		return false;
	int flag = 0;
	for (int k = 0; k < 4; k++)
	{
		if (!inBoard(i + cx[k], j + cy[k]))
		{
			flag++;
		}
		else
		{
			if (board[i + cx[k]][j + cy[k]] == turn)
			{
				flag++;
			}
			if (board[i + cx[k]][j + cy[k]] == -turn)
			{
				return false;
			}
		}
	}
	if (flag == 3)
		return true;
	return false;
}
bool last(int a[9][9], int turn, int pace)
{
	if (pace < 30) return false;
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (a[i][j] == 0)
			{
				if (Check_available(i, j, turn))
					return false;
			}
		}
	}
	return true;
}
int liberty(int i, int j, int turn, int depth)
{
	if (depth == 0)
	{
		memset(board_2D, 0, sizeof(board_2D));
	}
	int liberty_piece = 0;
	if (board[i][j] == turn)
	{
		for (int k = 0; k < 4; k++)
		{
			if (inBoard(i + cx[k], j + cy[k]) && board[i + cx[k]][j + cy[k]] == 0)
			{
				liberty_piece += 1;
			}
			if (inBoard(i + cx[k], j + cy[k]) && board[i + cx[k]][j + cy[k]] == board[i][j])
			{
				board_2D[i][j] = 1;
				if (board_2D[i + cx[k]][j + cy[k]] == 0)
					liberty_piece += liberty(i + cx[k], j + cy[k], turn, 1);
			}
		}
	}
	return liberty_piece;
}
int mark_tableware(int turn)
{
	int mark;
	int num = 0;
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (Check_available(i, j, 1) && !Check_available(i, j, -1))
			{
				mark += 100;
			}
			if (Check_available(i, j, -1) && !Check_available(i, j, 1))
			{
				mark -= 100;
			}
			if (Check_available(i, j, 1) && Check_available(i, j, -1))
			{
				num++;
			}
		}
	}
	if (turn == 1)
	{
		if (num % 2 == 0)
		{
			mark += 50;
		}
		else if (num % 2 == 1)
		{
			mark -= 50;
		}
	}
	if (turn == -1)
	{
		if (num % 2 == 0)
		{
			mark -= 50;
		}
		else if (num % 2 == 1)
		{
			mark += 50;
		}
	}
	return mark;
}
double mark_eye(int i, int j, int turn)
{
	double mark;
	if (turn == -1)
	{
		if (board_connection2[i][j] != 0)
		{
			return 110;
		}
	}
	if (turn == 1)
	{
		if (board_connection3[i][j] != 0)
		{
			return 110;
		}
	}
	nihility(i, j, turn, situation2);
	if (liberty(i, j, turn, 0) == 0)
		return 0;
	reversion();
	board[i][j] = turn;
	int liberty_eye = liberty(i, j, turn, 0);
	if (liberty_eye == 0)
	{
		board[i][j] = 0;
		return 0;
	}
	if (liberty_eye <= 7)
		mark = log10(liberty_eye + 3) * 110;
	else
	{
		mark = 110;
	}
	board[i][j] = 0;
	return mark;
}
double mark_atari(int i, int j, int turn)
{
	double mark;
	int x, y;
	for (int k = 0; k < 4; k++)
	{
		if (inBoard(i + cx[k], j + cy[k]))
			if (board[i + cx[k]][j + cy[k]] == 0)
			{
				x = i + cx[k];
				y = j + cy[k];
			}
	}
	if (turn == -1)
	{
		if (board_connection2[x][y] != 0)
		{
			return 80;
		}
	}
	if (turn == 1)
	{
		if (board_connection3[x][y] != 0)
		{
			return 80;
		}
	}
	nihility(x, y, turn, situation2);
	if (liberty(x, y, turn, 0) == 0)
		return 0;
	reversion();
	board[x][y] = turn;
	int liberty_atari = liberty(i, j, turn, 0);
	if (liberty_atari == 0)
		return 0;
	if (liberty_atari <= 6)
		mark = log10(liberty_atari + 4) * 80;
	else
		mark = 80;
	if (x == 0)
	{
		if (i == 1)
			mark /= 1.4;
		else
			mark /= 1.2;
	}
	if (x == 8)
	{
		if (i == 7)
			mark /= 1.4;
		else
			mark /= 1.2;
	}
	if (y == 8)
	{
		if (j == 7)
			mark /= 1.4;
		else
			mark /= 1.2;
	}
	if (y == 0)
	{
		if (j == 1)
			mark /= 1.4;
		else
			mark /= 1.2;
	}
	board[x][y] = 0;
	return mark;
}
double mark_opening(int a[9][9])
{
	double mark_bot = 0;
	double mark_player = 0;
	if (a[0][0] == 1)
	{
		if (a[0][3] == 1 || a[3][0] == 1)
			mark_bot += 5;
		if (a[0][3] == 1 && a[3][0] == 1)
			mark_bot += 10;
	}
	if (a[0][0] == -1)
	{
		mark_player += 10;
		if (a[0][3] == -1 || a[3][0] == -1)
			mark_player += 5;
		if (a[0][3] == -1 && a[3][0] == -1)
			mark_player += 10;
	}
	if (a[0][8] == 1)
	{
		mark_bot += 10;
		if (a[0][5] == 1 || a[3][8] == 1)
			mark_bot += 5;
		if (a[0][5] == 1 && a[3][8] == 1)
			mark_bot += 10;
	}
	if (a[0][8] == -1)
	{
		mark_player += 10;
		if (a[0][5] == -1 || a[3][8] == -1)
			mark_player += 5;
		if (a[0][5] == -1 && a[3][8] == -1)
			mark_player += 10;
	}
	if (a[8][8] == 1)
	{
		mark_bot += 10;
		if (a[8][5] == 1 || a[5][8] == 1)
			mark_bot += 5;
		if (a[8][5] == 1 && a[5][8] == 1)
			mark_bot += 10;
	}
	if (a[8][8] == -1)
	{
		mark_player += 10;
		if (a[8][5] == -1 || a[5][8] == -1)
			mark_player += 5;
		if (a[8][5] == -1 && a[5][8] == -1)
			mark_player += 10;
	}
	if (a[8][0] == 1)
	{
		mark_bot += 10;
		if (a[8][3] == 1 || a[5][0] == 1)
			mark_bot += 5;
		if (a[8][3] == 1 && a[5][0] == 1)
			mark_bot += 10;
	}
	if (a[8][0] == -1)
	{
		mark_player += 10;
		if (a[8][3] == -1 || a[5][0] == -1)
			mark_player += 5;
		if (a[8][3] == -1 && a[5][0] == -1)
			mark_player += 10;
	}
	if (a[0][0] == 1 && a[1][1] == 1)
		mark_bot += 40;
	if (a[0][0] == -1 && a[1][1] == -1)
		mark_player += 40;
	if (a[0][8] == 1 && a[1][7] == 1)
		mark_bot += 40;
	if (a[0][8] == -1 && a[1][7] == -1)
		mark_player += 40;
	if (a[8][8] == 1 && a[7][7] == 1)
		mark_bot += 40;
	if (a[8][8] == -1 && a[7][7] == -1)
		mark_player += 40;
	if (a[8][0] == 1 && a[7][1] == 1)
		mark_bot += 40;
	if (a[8][0] == -1 && a[7][1] == -1)
		mark_player += 40;
	memset(board_connection2, 0, sizeof(board_connection2));
	memset(board_connection3, 0, sizeof(board_connection3));
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			flag_connection = 1;
			if (board[i][j] == 0)
			{
				if (Check_available(i, j, -1) && !Check_available(i, j, 1))
				{
					connection = 0;
					memset(board_connection, 0, sizeof(board_connection));
					series_connection(i, j, -1, 0);
					if (connection >= 2)
					{
						mark_player += 40 * (connection - 1);
					}
					if (connection == 1)
					{
						board_connection2[i][j] = 0;
					}
				}
			}
			flag_connection = 1;
			if (board[i][j] == 0)
			{
				if (Check_available(i, j, 1) && !Check_available(i, j, -1))
				{
					connection = 0;
					memset(board_connection, 0, sizeof(board_connection));
					series_connection(i, j, 1, 0);
					if (connection >= 2)
					{
						mark_bot += 40 * (connection - 1);
					}
					if (connection == 1)
					{
						board_connection3[i][j] = 0;
					}
				}
			}
		}
	}
	memset(board_lost, 0, sizeof(board_lost));
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			
			if (a[i][j] == 0)
			{
				if (judge_eye(i, j, 1))
				{
					mark_bot += mark_eye(i, j, 1);
				}
				if (judge_eye(i, j, -1))
				{
					mark_player += mark_eye(i, j, -1);
				}
			}
			if (a[i][j] == 1)
			{
				if (judge_atari(i, j, 1))
				{
					mark_bot += mark_atari(i, j, 1);
				}
			}
			if (a[i][j] == -1)
			{
				if (judge_atari(i, j, -1))
				{
					mark_player += mark_atari(i, j, -1);
				}
			}
		}
	}
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (board_lost[i][j] == 0 && board[i][j] == 0)
			{
				if (tiger(i, j, -1))
				{
					mark_player += 30;
				}
				if (tiger(i, j, 1))
				{
					mark_bot += 30;
				}
			}
		}
	}
	return mark_bot - mark_player;
}
double mark(int a[9][9], int turn)
{
	float mark_bot = 0, mark_enemy = 0;
	int ble = 0, blb = 0;
	memset(board_connection2, 0, sizeof(board_connection2));
	memset(board_connection3, 0, sizeof(board_connection3));
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			flag_connection = 1;
			if (board[i][j] == 0)
			{
				if (Check_available(i, j, -1) && !Check_available(i, j, 1))
				{
					connection = 0;
					memset(board_connection, 0, sizeof(board_connection));
					series_connection(i, j, -1, 0);
					if (connection >= 2)
					{
						mark_enemy += 40 * (connection - 1);
					}
					if (connection == 1)
					{
						board_connection2[i][j] = 0;
					}
				}
			}
			flag_connection = 1;
			if (board[i][j] == 0)
			{
				if (Check_available(i, j, 1) && !Check_available(i, j, -1))
				{
					connection = 0;
					memset(board_connection, 0, sizeof(board_connection));
					series_connection(i, j, 1, 0);
					if (connection >= 2)
					{
						mark_bot += 40 * (connection - 1);
					}
					if (connection == 1)
					{
						board_connection3[i][j] = 0;
					}
				}
			}
		}
	}
	memset(board_lost, 0, sizeof(board_lost));
	int num = 0;
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (Check_available(i, j, 1) && Check_available(i, j, -1))
			{
				num++;
			}
			if (a[i][j] == 0)
			{
				if (judge_eye(i, j, 1))
				{
					mark_bot += mark_eye(i, j, 1);
					blb++;
				}
				if (judge_eye(i, j, -1))
				{
					mark_enemy += mark_eye(i, j, -1);
					ble++;
				}
			}
			if (a[i][j] == 1)
			{
				if (judge_atari(i, j, 1))
				{
					mark_bot += mark_atari(i, j, 1);
					blb++;
				}
			}
			if (a[i][j] == -1)
			{
				if (judge_atari(i, j, -1))
				{
					mark_enemy += mark_atari(i, j, -1);
					ble++;
				}
			}
		}
	}
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (board_lost[i][j] == 0 && board[i][j] == 0)
			{
				if (tiger(i, j, -1))
				{
					mark_enemy += 30;
				}
				if (tiger(i, j, 1))
				{
					mark_bot += 30;
				}
			}
		}
	}
	if (ble == blb)
	{
		if (turn == 1)
		{
			if (num % 2 == 0)
			{
				mark_bot += 30;
			}
			else if (num % 2 == 1)
			{
				mark_enemy += 30;
			}
		}
		if (turn == -1)
		{
			if (num % 2 == 0)
			{
				mark_enemy += 30;
			}
			else if (num % 2 == 1)
			{
				mark_bot += 30;
			}
		}
	}
	return mark_bot - mark_enemy;
}
double alphabeta(int x, int y, int depth, int alpha, int beta, int turn, int situation)
{
	clk2 = clock();
	if (clk2 - clk1 > 1500)
		return -100000;
	board[x][y] = turn;
	if (depth == 0 || last(board, turn, pace))
	{
		float mark_now;
		if (situation == 0)
			mark_now = mark_opening(board);
		else if (situation == 1)
			mark_now = mark(board, turn);
		else if (situation == 2)
			mark_now = mark_tableware(turn);
		board[x][y] = 0;
		return mark_now;
	}
	if (turn == -1)
	{
		int flag = 0;

		for (int k = 1; k <= 9; k++)
		{
			if (y - k >= 0 && y - k <= 8)
			{
				for (int i = x - k; i <= k + x; i++)
				{
					if (i < 0 || i>8) continue;
					if (Check_available(i, y - k, 1))
					{
						int com = alphabeta(i, y - k, depth - 1, alpha, beta, 1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						alpha = max(alpha, com);
					}
					if (alpha >= beta)
					{
						flag++;
						break;
					}
				}
			}
			if (flag)
				break;
			if (y + k >= 0 && y + k <= 8)
			{
				for (int i = x - k; i <= k + x; i++)
				{
					if (i < 0 || i>8) continue;
					if (Check_available(i, y + k, 1))
					{
						int com = alphabeta(i, y + k, depth - 1, alpha, beta, 1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						alpha = max(alpha, com);
					}
					if (alpha >= beta)
					{
						flag++;
						break;
					}
				}
			}
			if (flag)
				break;
			if (x + k >= 0 && x + k <= 8)
			{
				for (int j = y - k + 1; j < k + y; j++)
				{
					if (j < 0 || j>8) continue;
					if (Check_available(x + k, j, 1))
					{
						int com = alphabeta(x + k, j, depth - 1, alpha, beta, 1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						alpha = max(alpha, com);
					}
					if (alpha >= beta)
					{
						flag++;
						break;
					}
				}
			}
			if (flag)
				break;
			if (x - k >= 0 && x - k <= 8)
			{
				for (int j = y - k + 1; j < k + y; j++)
				{
					if (j < 0 || j>8) continue;
					if (Check_available(x - k, j, 1))
					{
						int com = alphabeta(x - k, j, depth - 1, alpha, beta, 1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						alpha = max(alpha, com);
					}
					if (alpha >= beta)
					{
						flag++;
						break;
					}
				}
				if (flag)
					break;
			}
		}
		board[x][y] = 0;
		return alpha;
	}
	if (turn == 1)
	{
		int flag = 0;

		for (int k = 1; k <= 9; k++)
		{
			if (y - k >= 0 && y - k <= 8)
			{
				for (int i = x - k; i <= k + x; i++)
				{
					if (i < 0 || i>8) continue;
					if (Check_available(i, y - k, -1))
					{
						int com = alphabeta(i, y - k, depth - 1, alpha, beta, -1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						beta = min(beta, com);
					}
					if (beta <= alpha)
					{
						flag++;
						break;
					}
				}
			}
			if (flag)
				break;
			if (y + k >= 0 && y + k <= 8)
			{
				for (int i = x - k; i <= k + x; i++)
				{
					if (i < 0 || i>8) continue;
					if (Check_available(i, y + k, -1))
					{
						int com = alphabeta(i, y + k, depth - 1, alpha, beta, -1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						beta = min(beta, com);
					}
					if (beta <= alpha)
					{
						flag++;
						break;
					}
				}
			}
			if (flag)
				break;
			if (x + k >= 0 && x + k <= 8)
			{
				for (int j = y - k + 1; j < k + y; j++)
				{
					if (j < 0 || j>8) continue;
					if (Check_available(x + k, j, -1))
					{
						int com = alphabeta(x + k, j, depth - 1, alpha, beta, -1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						beta = min(beta, com);
					}
					if (beta <= alpha)
					{
						flag++;
						break;
					}
				}
			}
			if (flag)
				break;
			if (x - k >= 0 && x - k <= 8)
			{
				for (int j = y - k + 1; j < k + y; j++)
				{
					if (j < 0 || j>8) continue;
					if (Check_available(x - k, j, -1))
					{
						int com = alphabeta(x - k, j, depth - 1, alpha, beta, -1, situation);
						if (com == -100000)
						{
							board[x][y] = 0;
							return -100000;
						}
						beta = min(beta, com);
					}
					if (beta <= alpha)
					{
						flag++;
						break;
					}
				}
				if (flag)
					break;
			}
		}
		board[x][y] = 0;
		return beta;
	}
}
void series_connection(int i, int j, int turn, int depth)
{

	if (board[i][j] == 0)
	{
		if (turn == -1 && board_connection2[i][j] != 0)
		{
			return;
		}
		if (turn == 1 && board_connection3[i][j] != 0)
		{
			return;
		}
		if (Check_available(i, j, turn) && !Check_available(i, j, -turn))
		{
			connection++;
			board_connection[i][j] = 1;
			if (turn == -1)
			{
				board_connection2[i][j] = 1;
			}
			else if (turn == 1)
			{
				board_connection3[i][j] = 1;
			}
			for (int k = 0; k < 4; k++)
			{
				if (inBoard(i + cx[k], j + cy[k]) && board_connection[i + cx[k]][j + cy[k]] == 0)
					series_connection(i + cx[k], j + cy[k], turn, 0);
			}
		}
		if (!Check_available(i, j, turn) && !Check_available(i, j, -turn))
		{
			if (flag_connection)
			{
				flag_connection = 0;
				connection++;
			}
			board_connection[i][j] = 1;
			if (turn == -1)
			{
				board_connection2[i][j] = 1;
			}
			else if (turn == 1)
			{
				board_connection3[i][j] = 1;
			}
			for (int k = 0; k < 4; k++)
			{
				if (inBoard(i + cx[k], j + cy[k]) && board_connection[i + cx[k]][j + cy[k]] == 0)
					series_connection(i + cx[k], j + cy[k], turn, 0);
			}
		}
		else return;
	}
	if (board[i][j] == turn)
	{
		board_connection[i][j] = 1;
		for (int k = 0; k < 4; k++)
		{
			if (inBoard(i + cx[k], j + cy[k]) && board_connection[i + cx[k]][j + cy[k]] == 0)
				series_connection(i + cx[k], j + cy[k], turn, 0);
		}
	}
}
void nihility(int i, int j, int turn, int depth)
{
	if (depth == 0)
		return;
	if (board[i][j] == 0)
	{
		board_rem[i][j] = turn;
		board[i][j] = turn;
		depth--;
	}
	if (board[i][j] == turn && board_nihility[i][j] != turn)
	{
		board_nihility[i][j] = turn;
		for (int k = 0; k < 4; k++)
		{
			if (inBoard(i + cx[k], j + cy[k]))
			{
				if (board[i][j] != -turn)
				{
					nihility(i + cx[k], j + cy[k], turn, depth);
				}
			}
		}
	}
}
void reversion()
{
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (board_rem[i][j] != 0)
			{
				board[i][j] = 0;
				board_rem[i][j] = 0;
			}
		}
	}
}
void cundang()
{
	ofstream outfile;
	outfile.open("游戏存档");
	outfile << mode << endl;
	outfile << pace << endl;
	outfile << back << endl;
	outfile << x_past1 << endl;
	outfile << y_past1 << endl;
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			outfile << board[i][j] << endl;
		}
	}
	for (int k = 0; k <= pace; k++)
	{
		for (int i = 0; i <= 8; i++)
		{
			for (int j = 0; j <= 8; j++)
			{
				outfile << board_mem[k][i][j] << endl;
			}
		}
	}
	return;
}
void dudang()
{
	ifstream infile;
	infile.open("游戏存档");
	infile >> mode;
	infile >> pace;
	infile >> back;
	infile >> x_past1;
	infile >> y_past1;
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			infile >> board[i][j];
		}
	}
	for (int k = 0; k <= pace; k++)
	{
		for (int i = 0; i <= 8; i++)
		{
			for (int j = 0; j <= 8; j++)
			{
				infile >> board_mem[k][i][j];
			}
		}
	}
	initGame();
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			if (mode == 0)
			{
				if (board[i][j] == -1)
				{
					setfillcolor(BLACK);
					solidcircle((i + 1) * 60, (j + 1) * 60, 20);
					setlinecolor(RGB(222, 158, 68));
					circle(i * 60 + 60, j * 60 + 60, 20);
				}
				else if (board[i][j] == 1)
				{
					setfillcolor(WHITE);
					solidcircle((i + 1) * 60, (j + 1) * 60, 20);
					setlinecolor(RGB(222, 158, 68));
					circle(i * 60 + 60, j * 60 + 60, 20);
				}
			}
			else if (mode == 1)
			{
				if (board[i][j] == -1)
				{
					setfillcolor(WHITE);
					solidcircle((i + 1) * 60, (j + 1) * 60, 20);
					setlinecolor(RGB(222, 158, 68));
					circle(i * 60 + 60, j * 60 + 60, 20);
				}
				else if (board[i][j] == 1)
				{
					setfillcolor(BLACK);
					solidcircle((i + 1) * 60, (j + 1) * 60, 20);
					setlinecolor(RGB(222, 158, 68));
					circle(i * 60 + 60, j * 60 + 60, 20);
				}
			}
		}
	}
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 3);
	setlinecolor(RGB(192, 44, 56));
	circle(x_past1*60+60, y_past1*60+60, 20);
	playNoGo();
	return;
}