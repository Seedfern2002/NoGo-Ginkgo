#include<iostream>
#include<cmath>
#include<cstring>
#include<time.h>
#include"jsoncpp/json.h"
using namespace std;
int board[9][9];
int power[9][9];
int board_2D[9][9];
int board_3D[9][9];
int board_4D[9][9];
int board_eye[9][9];
int board_connection[9][9];
int board_connection2[9][9];    // The connection status of enemy
int board_connection3[9][9];    // The connection status of bot
int board_nihility[9][9];
int board_rem[9][9];
int board_atari1[9][9];
int board_atari2[9][9];
bool board_blockade[9][9];  // The block status
int board_blc[9][9];
int board_lost[9][9];
int situation = 0;
int situation2 = 2;
int connection = 0;
int flag_connection = 1;
int cx[] = { -1,0,1,0 };
int cy[] = { 0,-1,0,1 };
int mx[] = { -1,1,-1,1 };
int my[] = { -1,-1,1,1 };
int cz[] = { -1,1,1,-1 };
int cc[] = { 1,-1,1,-1 };
int turnID;
int remain = 0;
int ans_x, ans_y;
clock_t clk1, clk2;

bool inBoard(int i,int j);
bool Check_available(int i,int j,int turn);
bool judge_eye(int i,int j,int turn);
bool judge_atari(int i,int j,int turn);
bool tiger(int i,int j,int turn);
bool judge_double(int i,int j,int turn);
bool last(int a[9][9], int turn, int pace);
int liberty(int i,int j,int turn,int depth);
double mark_eye(int i,int j,int turn);
double mark_atari(int i,int j,int turn);
double mark_opening(int a[9][9]);
double mark(int a[9][9],int turn);
int mark_tableware(int turn);
double alphabeta(int x, int y, int depth, int alpha, int beta, int turn, int situation);
void series_connection(int i, int j, int turn, int depth);
void nihility(int i, int j, int turn,int depth);
void reversion();
void play_NoGo(int x,int y,int depth);
int main()
{
	clk1 = clock();
	string str;
	int x, y;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	// recover status 
	turnID = input["responses"].size();
	for (int i = 0; i < turnID; i++)
	{
		x = input["requests"][i]["x"].asInt(), y = input["requests"][i]["y"].asInt();
		if (x != -1) board[x][y] = -1;
		x = input["responses"][i]["x"].asInt(), y = input["responses"][i]["y"].asInt();
		if (x != -1) board[x][y] = 1;
	}
	x = input["requests"][turnID]["x"].asInt(), y = input["requests"][turnID]["y"].asInt();
	if (x != -1) board[x][y] = -1;
	// output the decision JSON
	Json::Value ret;
	Json::Value action;

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
	if (turnID < 13)
	{
		situation = 0;
		depth = 2;
	}
	if (turnID < 18&&turnID>=13)
	{
		situation = 1;
		depth = 2;
	}
	if (turnID >= 18 && turnID <= 24)
	{
		situation = 1;
		depth = 3;
	}
	if (turnID > 24)
	{
		situation = 1;
		depth = 4;
		situation2 = 2;
	}
	if (remain < 12&&remain > 7)
	{
		situation = 1;
		depth = 5;
		situation2 = 1;
	}
	if (remain<=7)
	{
		situation = 2;
		depth = 10;
		situation2 = 1;
	}
	play_NoGo(x, y, depth);

	action["x"] = ans_x, action["y"] = ans_y;
	ret["response"] = action;
	Json::FastWriter writer;

	cout << writer.write(ret) << endl;
	return 0;
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
				if(inBoard(i+cx[k],j+cy[k])&&board[i+cx[k]][j+cy[k]]==-turn)
					if (liberty(i + cx[k], j + cy[k], -turn, 0) == 0 )
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
				board_lost[i + cx[k]][j + cy[k]] = 1 ;
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
bool judge_double(int i, int j, int turn)
{
	int flag = 0;
	for (int k = 0; k < 4; k++)
	{
		if (inBoard(i + cx[k], j + cy[k]))
			if (tiger(i + cx[k], j + cy[k], turn))
			{
				if(inBoard(i+2*cx[k],j+2*cy[k])&&Check_available(i+2*cx[k],j+2*cy[k],turn))
					flag++;
			}
	}
	if (flag >= 2)
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
	int num=0;
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
double mark(int a[9][9],int turn)
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
	if (clk2 - clk1 > 990000)
		return -100000;
	board[x][y] = turn;
	if (depth == 0 || last(board, turn, turnID))
	{
		float mark_now;
		if (situation == 0)
			mark_now = mark_opening(board);
		else if (situation == 1)
			mark_now = mark(board,turn);
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
					if (Check_available(i, y-k, 1))
					{
						int com = alphabeta(i, y-k, depth - 1, alpha, beta, 1, situation);
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
					if (Check_available(i, y+k, 1))
					{
						int com = alphabeta(i, y+k, depth - 1, alpha, beta, 1, situation);
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
					if (Check_available(x+k, j, 1))
					{
						int com = alphabeta(x+k, j, depth - 1, alpha, beta, 1, situation);
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
		reversepower(x, y, turn);
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
					if (Check_available(i, y-k, -1))
					{
						int com = alphabeta(i, y-k, depth - 1, alpha, beta, -1, situation);
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
					if (Check_available(i, y+k, -1))
					{
						int com = alphabeta(i, y+k, depth - 1, alpha, beta, -1, situation);
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
					if (Check_available(x+k, j, -1))
					{
						int com = alphabeta(x+k, j, depth - 1, alpha, beta, -1, situation);
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
					if (Check_available(x-k, j, -1))
					{
						int com = alphabeta(x-k, j, depth - 1, alpha, beta, -1, situation);
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
		if (turn==-1&&board_connection2[i][j]!=0)//���������ͨ�������ظ����㣻
		{
			return;
		}
		if (turn == 1 && board_connection3[i][j]!=0)
		{
			return;
		}
		if (Check_available(i, j, turn) && !Check_available(i, j, -turn))//���Լ������Ӷ����ֲ������ӣ�����ͨ����һ
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
		if (!Check_available(i, j, turn) && !Check_available(i, j, -turn))//��˫�����������ӣ������һ������Ϊ����Ч��ͨ��
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
void nihility(int i, int j, int turn,int depth)
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
void play_NoGo(int x, int y,int depth)
{
	int ans_i1, ans_j1;
	int bflag = 0;
	int mmax = -99999;
	ans_i1 = 0;
	ans_j1 = 0;
	int depth1 = 2;
	if (turnID > 30)
	{
		depth1 = 4;
	}
	if (remain<= 7)
	{
		depth1 = 6;
	}
	int depth2 = min(depth1, depth - 1);
	for (int k = 1; k <= 9; k++)
	{
		if (y - k >= 0 && y - k <= 8)
		{
			for (int i = x - k; i <= k + x; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y - k, 1))
				{
					int mark_now = alphabeta(i, y - k, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = i;
						ans_j1 = y - k;
						mmax = mark_now;
					}
				}
			}
		}
		if (y + k >= 0 && y + k <= 8)
		{
			for (int i = x - k; i <= k + x; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y + k, 1))
				{
					int mark_now = alphabeta(i, y + k, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = i;
						ans_j1 = y + k;
						mmax = mark_now;
					}
				}
			}
		}
		if (x + k >= 0 && x + k <= 8)
		{
			for (int j = y - k + 1; j < k + y; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x + k, j, 1))
				{
					int mark_now = alphabeta(x + k, j, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = x + k;
						ans_j1 = j;
						mmax = mark_now;
					}
				}
			}
		}
		if (x - k >= 0 && x - k <= 8)
		{
			for (int j = y - k + 1; j < k + y; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x - k, j, 1))
				{
					int mark_now = alphabeta(x - k, j, depth2, mmax, 99999, 1, situation);
					if (mark_now > mmax)
					{
						ans_i1 = x - k;
						ans_j1 = j;
						mmax = mark_now;
					}
				}
			}
		}
	}
	mmax = -99999;
	int ans_i, ans_j;
	int tflag = 1;
	for (int k = 1; k <= 9; k++)
	{
		if (y - k >= 0 && y - k <= 8)
		{
			for (int i = x - k; i <= k + x; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y - k, 1))
				{
					int mark_now = alphabeta(i, y - k, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i = i;
						ans_j = y - k;
						mmax = mark_now;
					}
				}
			}
		}
		if (tflag == 0)
			break;
		if (y + k >= 0 && y + k <= 8)
		{
			for (int i = x - k; i <= k + x; i++)
			{
				if (i < 0 || i>8) continue;
				if (Check_available(i, y + k, 1))
				{
					int mark_now = alphabeta(i, y + k, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i = i;
						ans_j = y + k;
						mmax = mark_now;
					}
				}
			}
		}
		if (tflag == 0)
			break;
		if (x + k >= 0 && x + k <= 8)
		{
			for (int j = y - k + 1; j < k + y; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x + k, j, 1))
				{
					int mark_now = alphabeta(x + k, j, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i = x + k;
						ans_j = j;
						mmax = mark_now;
					}
				}
			}
		}
		if (tflag == 0)
			break;
		if (x - k >= 0 && x - k <= 8)
		{
			for (int j = y - k + 1; j < k + y; j++)
			{
				if (j < 0 || j>8) continue;
				if (Check_available(x - k, j, 1))
				{
					int mark_now = alphabeta(x - k, j, depth, mmax, 99999, 1, situation);
					if (mark_now == -100000)
					{
						tflag = 0;
						break;
					}
					if (mark_now > mmax)
					{
						ans_i = x - k;
						ans_j = j;
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
		ans_x = ans_i, ans_y = ans_j;
	}
	else if (tflag == 0)
	{
		ans_x = ans_i1, ans_y = ans_j1;
	}
	return;
}