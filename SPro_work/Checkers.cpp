#pragma once

#include "Checkers.h"

Cell board[10][10];

bool Cell::isFree()
{
	return checker == nullptr;
}

Checker::Checker(Color color, int x, int y)
{
	this->color = color;
	this->x = x;
	this->y = y;
}

Game::Game()
{
	playerColor = WHITE;

	board[4][5] = { NONE, new King{WHITE, 4, 5}};
	board[1][2] = { NONE, new Checker{BLACK, 1, 2} };
	board[8][9] = { NONE, new Checker{BLACK, 8, 9} };
	board[1][8] = { NONE, new Checker{BLACK, 1, 8} };
	board[1][8] = { NONE, new Checker{BLACK, 7, 2} };


	/*for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i + j) & 0b1)
			{
				board[i][j] = { NONE, new Checker{BLACK, i, j} };
			}
			else
			{
				board[i][9 - j] = { NONE, new Checker{WHITE, i, 9 - j} };
			}
		}
	}*/
}

void Game::Select(int x, int y)
{
	if (!board[x][y].isFree())
	{ 
		board[x][y].state = CHOOSEN;
		
	
	 auto points = board[x][y].checker->getAttackablePoints();
		for (auto p: points)
		{
			board[p.first][p.second].state = ATTACKABLE;
		}
	}
}

bool Checker::canAttack()
{
	for (int dx = -1; dx <= 1; dx += 2)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			if (!(dx + x >= 0 && dx + x < 10)) continue;
			if (!(2 * dx + x >= 0 && 2 * dx + x < 10)) continue;
			if (!(dy + y >= 0 && dy + y < 10)) continue;
			if (!(2 * dy + y >= 0 && 2 * dy + y < 10)) continue;

			if (board[x + 2 * dx][y + 2 * dy].isFree()
				&& !board[x + dx][y + dy].isFree()
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

			if (board[x + 2 * dx][y + 2 * dy].isFree()
				&& !board[x + dx][y + dy].isFree()
				&& board[x + dx][y + dy].checker->color != color)
				points.push_back(Coordinates(x + 2 * dx, y + 2 * dy));

		}
	}

	if (!points.size())
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (!board[i][j].isFree() && board[i][j].checker->color == color)
					if (board[i][j].checker->canAttack())
						return points;

			}
		}

		int dy = (color ? -1 : 1);
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (board[x + dx][y + dy].isFree())
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

void Game::Move(int x1, int y1, int x2, int y2)
{
	//if (typeid(*zaklad) == typeid (ConcertHall))

	board[x2][y2] = { NONE, board[x1][y1].checker };
	board[x1][y1].checker->x = x2;
	board[x1][y1].checker->y = y2;
	board[x1][y1] = { NONE, nullptr };

	if (abs(x1 - x2) >= 2)
	{
		int x = (x1 + x2) / 2;
		int y = (y1 + y2) / 2;
		if (board[x][y].checker)
			delete board[x][y].checker;
		board[x][y] = { NONE, nullptr };
		if (board[x2][y2].checker->canAttack())
			playerColor = !playerColor;
	}
	playerColor = !playerColor;

}

std::vector<Coordinates> King::getAttackablePoints()
{
	std::vector<Coordinates> points;

	for (int k = 2; k < 10; k++)
	{
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (x + k * dx < 0 || x + k * dx > 9) continue;
			if (!(dx + x >= 0 && dx + x < 10)) continue;
			for (int dy = -1; dy <= 1; dy += 2)
			{
				if (y + k * dx < 0 || y + k * dx > 9) continue;

				if (!(dy + y >= 0 && dy + y < 10)) continue;


				if (board[x + k * dx][y + k * dy].isFree()
					&& !board[x + (k - 1) * dx][y + (k - 1) * dy].isFree()
					&& board[x + (k - 1) * dx][y + (k - 1) * dy].checker->color != color)
					points.push_back(Coordinates(x + k * dx, y + k * dy));
			}
		}
	}

	if (!points.size())
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (!board[i][j].isFree() && board[i][j].checker->color == color)
					if (board[i][j].checker->canAttack())
						return points;

			}
		}

		int dy = (color ? -1 : 1);
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (board[x + dx][y + dy].isFree())
				points.push_back(Coordinates(x + dx, y + dy));
		}
	}
	return points;
}

bool King::canAttack()
{
	for (int k = 2; k < 10; k++)
	{
		for (int dx = -1; dx <= 1; dx += 2)
			{
			if (x + k * dx < 0 || x + k * dx > 9) continue;
			if (!(dx + x >= 0 && dx + x < 10)) continue;
				for (int dy = -1; dy <= 1; dy += 2)
				{
					if (y + k * dx < 0 || y + k * dx > 9) continue;
					
					if (!(dy + y >= 0 && dy + y < 10)) continue;
					

					if (board[x + k * dx][y + k * dy].isFree()
						&& !board[x + (k - 1) * dx][y + (k - 1) * dy].isFree()
						&& board[x + (k - 1) * dx][y + (k - 1) * dy].checker->color != color)
						return true;
				}
			}
	}
	
	return false;
}
//{
//	for ()
//	{
//		for ()
//		{
//			if (!(dx + x >= 0 && dx + x < 10)) continue;
//			if (!(2 * dx + x >= 0 && 2 * dx + x < 10)) continue;
//			if (!(dy + y >= 0 && dy + y < 10)) continue;
//			if (!(2 * dy + y >= 0 && 2 * dy + y < 10)) continue;
//
//			
//		}
//	}
//	return false;
//}

