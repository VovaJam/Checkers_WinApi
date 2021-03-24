#pragma once

#include "Checkers.h"
#define IS_DEBUG 0
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
	
#if IS_DEBUG


	board[4][5] = { NONE, new King{WHITE, 4, 5}};
	board[0][1] = { NONE, new Checker{WHITE, 0, 1} };
	board[7][8] = { NONE, new Checker{BLACK, 7, 8} };
	board[1][8] = { NONE, new Checker{BLACK, 1, 8} };
	board[7][2] = { NONE, new Checker{BLACK, 7, 2} };
		
	amount[BLACK] = 3;
	amount[WHITE] = 2;
#else
	amount[BLACK] = 20;
	amount[WHITE] = 20;

	for (int i = 0; i < 10; i++)
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
	}
#endif
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
	if (board[x2][y2].state != ATTACKABLE) return;
	

	board[x2][y2] = { NONE, board[x1][y1].checker };
	board[x1][y1].checker->x = x2;
	board[x1][y1].checker->y = y2;
	board[x1][y1] = { NONE, nullptr };
	int length = abs(x1 - x2);//sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));

	if (length >= 2)
	{
		int dx = ((x1 - x2) > 0 ? 1 : -1);
		int dy = ((y1 - y2) > 0 ? 1 : -1);
		int x = x2 + dx;//(x1 + x2) / length;
		int y = y2 + dy; // (y1 + y2) / length;
		if (board[x][y].checker)
		{
			amount[board[x][y].checker->color]--;
			delete board[x][y].checker;
		}
			
		board[x][y] = { NONE, nullptr };
		if (board[x2][y2].checker->canAttack())
			return;	
	}

	playerColor = !playerColor;

	if (board[x2][y2].checker->color == BLACK && y2 == 9)
	{
		delete board[x2][y2].checker;
		board[x2][y2] = { NONE, new King{BLACK, x2, y2} };
	}
	else if (board[x2][y2].checker->color == WHITE && y2 == 0)
	{
		delete board[x2][y2].checker;
		board[x2][y2] = { NONE, new King{WHITE, x2, y2} };
	}
	//playerColor = !playerColor;

}

bool Game::IsGameOver()
{
	return !(amount[WHITE] && amount[BLACK]);
		
}

void Game::Restart()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			if (board[i][j].isFree())
			{
				delete board[i][j].checker;
				board[i][j].checker = nullptr;
			}
				
	Game();
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
				if (y + k * dy < 0 || y + k * dy > 9) continue;

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
		/*for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (!board[i][j].isFree() && board[i][j].checker->color == color)
					if (board[i][j].checker->canAttack())
					return points;

			}
		}*/

		for (int k = 1; k < 10; k++)
		{
			for (int dx = -1; dx <= 1; dx += 2)
			{
				if (x + k * dx < 0 || x + k * dx > 9) continue;
				if (!(dx + x >= 0 && dx + x < 10)) continue;
				for (int dy = -1; dy <= 1; dy += 2)
				{
					if (y + k * dy < 0 || y + k * dy > 9) continue;

					if (!(dy + y >= 0 && dy + y < 10)) continue;
					if (board[x + k * dx][y + k * dy].isFree())
						points.push_back(Coordinates(x + k * dx, y + k * dy));
				}
			}
		}
		//int dy = (color ? -1 : 1); // Исправить для дамки

		//for (int dx = -1; dx <= 1; dx += 2)
		//{
		//	
		//}
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

