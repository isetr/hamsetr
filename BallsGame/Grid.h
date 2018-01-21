#pragma once

#include <vector>

#include "Ball.h"

struct Cell {
	std::vector<Ball*> balls;
};

class Grid {
	friend class BallController;
public:
	Grid(int width, int height, int cellSize, int distrib);
	~Grid();

	void addBall(Ball* ball);
	void addBallToCell(Ball* ball, Cell* cell);

	Cell* getCell(const glm::vec2& pos);
	Cell* getCellByPos(float x, float y);

	void removeBall(Ball* ball);
private:
	std::vector<Cell> m_cells;
	int m_width, m_height, m_cellSize, m_numXCells, m_numYCells;
};