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
	
}

bool Checker::CanAttack()
{
	for (int dx = -1; dx <= 1; dx += 2)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			if (!(dx + x >= 0 && dx + x < 10)) continue;
			if (!(2 * dx + x >= 0 && 2 * dx + x < 10)) continue;
			if (!(dy + y >= 0 && dy + y < 10)) continue;
			if (!(2 * dy + y >= 0 && 2 * dy + y < 10)) continue;

			if (board[x + 2 * dx][y + 2 * dy].isFree
				&& !board[x + dx][y + dy].isFree
				&& board[x + dx][y + dy].checker->color != color)
				return true;
		}
	}
	return false;
}

std::vector<Coordinates> Checker::getAttackablePoints()
{
	std::vector<Coordinates> points;

	for (int dx = -1; dx <= 1; dx += 2)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			if (!(dx + x >= 0 && dx + x < 10)) continue;
			if (!(2 * dx + x >= 0 && 2 * dx + x < 10)) continue;
			if (!(dy + y >= 0 && dy + y < 10)) continue;
			if (!(2 * dy + y >= 0 && 2 * dy + y < 10)) continue;

			if (board[x + 2 * dx][y + 2 * dy].isFree
				&& !board[x + dx][y + dy].isFree
				&& board[x + dx][y + dy].checker->color != color)
				points.push_back(Coordinates(x + 2 * dx, y + 2 * dy));

		}
	}
	if (!points.size())
	{
		int dy = (color ? 1 : -1);
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (board[x + dx][y + dy].isFree)
				points.push_back(Coordinates(x + dx, y + dy));
		}
	}

	return points;
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

