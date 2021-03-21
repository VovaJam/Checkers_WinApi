#pragma once

#ifndef DRAWCALL_H 
#define DRAWCALL_H
#endif

enum CellState { NONE, CHOOSEN, ATTACKABLE };

struct Cell
{
	bool isFree = true;
	bool CheckColor;
	CellState state = NONE;
};

extern Cell board[10][10];


class Game
{
public:
	bool PlayerColor;
	Game();
	void Select(int, int);
	bool Move(int, int, int, int);
	void Deselect();

};

