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

	Checker(Color, int, int);
	virtual bool canAttack();
	virtual std::vector<Coordinates> getAttackablePoints();
};


struct King : public Checker
{
	King(Color c, int x, int y) :Checker(c, x, y) {}
	bool canAttack() override;
	std::vector<Coordinates> getAttackablePoints() override;
};


class Game
{
	int amount[2];
public:
	
	bool playerColor;
	Game();
	void Select(int, int);
	void Move(int, int, int, int);
	void Deselect();
	void Restart();
	bool IsGameOver();
};

