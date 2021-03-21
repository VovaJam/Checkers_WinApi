#pragma once

#include "Checkers.h"

Cell board[10][10];

Game::Game()
{
	PlayerColor = 0;

	board[2][3] = { false, 0, NONE };
	board[5][6] = { false, 1, NONE };
	board[5][2] = { false, 0, NONE };
	board[3][4] = { false, 0, NONE };
	board[4][5] = { false, 0, NONE };


	/*for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i + j) & 0b1)
			{
				board[i][j] = { false, 0, NONE };	
			}
			else
			{
				board[i][9 - j] = { false, 1, NONE };
			}
		}
	}*/
}

void Game::Select(int x, int y)
{
	int dy = (board[x][y].CheckColor ? -1 : 1);
	bool beat = false;
	for (int dx = -1; dx <= 1; dx += 2) 
	{
		if (!board[x][y].isFree)
		{
			board[x][y].state = CHOOSEN;

			if (y > 1 && board[x + dx][y + dy].CheckColor == !board[x][y].CheckColor && board[x + dx * 2][y + 2 * dy].isFree)
			{
				board[x + 2 * dx][y + 2 * dy].state = ATTACKABLE;
				beat = true;
			}
			else
			{
				if (board[x + dx][y + dy].isFree && y > 0 && beat)
				{
					board[x + dx][y + dy].state = ATTACKABLE;
				}
			}
		}
	}
}

void Game::Deselect()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j].state = NONE;
		}
	}
}

