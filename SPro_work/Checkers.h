#pragma once

#ifndef DRAWCALL_H 
#define DRAWCALL_H
#endif

#include <vector>
typedef std::pair<int, int> Coordinates;

enum CellState { NONE, CHOOSEN, ATTACKABLE };
enum Color { BLACK, WHITE };

struct Cell
{
	bool isFree = true;
	CellState state = NONE;
	Checker* checker = nullptr;
};

extern Cell board[10][10];

class Checker
{
	Color color;
	int x, y;
public:
	bool CanAttack();
	std::vector<Coordinates> getAttackablePoints();

};

class Game
{
public:
	bool PlayerColor;
	Game();
	void Select(int, int);
	bool Move(int, int, int, int);
	void Deselect();

};

