#pragma once

#ifndef DRAWCALL_H 
#define DRAWCALL_H
#endif

#include <vector>
typedef std::pair<int, int> Coordinates;

enum CellState { NONE, CHOOSEN, ATTACKABLE };
enum Color { BLACK, WHITE };

struct Checker;

struct Cell
{
	CellState state = NONE;
	Checker* checker = nullptr;
	bool isFree();
};

extern Cell board[10][10];

struct Checker
{
	Color color;
	int x, y;

	bool canAttack();
	std::vector<Coordinates> getAttackablePoints();

};

class Game
{
public:
	bool playerColor;
	Game();
	void Select(int, int);
	void Move(int, int, int, int);
	void Deselect();

};

