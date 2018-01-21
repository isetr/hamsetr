#include "Grid.h"

#include <cmath>
#include <cstdlib>

Grid::Grid(int width, int height, int cellSize, int distrib) :
	m_width(width),
	m_height(height),
	m_cellSize(cellSize) {

	m_numXCells = ceil((float)m_width / m_cellSize);
	m_numYCells = ceil((float)m_height / m_cellSize);

	m_cells.resize(m_numYCells * m_numXCells);
	for (int i = 0; i < m_cells.size(); i++) {
		m_cells[i].balls.reserve(distrib);
	}

}

Grid::~Grid() {

}

void Grid::addBall(Ball* ball) {
	Cell* cell = getCell(ball->position);
	cell->balls.push_back(ball);
	ball->cell = cell;
	ball->placeInCell = cell->balls.size() - 1;
}

void Grid::addBallToCell(Ball* ball, Cell* cell) {
	cell->balls.push_back(ball);
	ball->cell = cell;
	ball->placeInCell = cell->balls.size() - 1;
}

Cell* Grid::getCell(const glm::vec2& pos) {
	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return getCellByPos(cellX, cellY);
}

Cell* Grid::getCellByPos(float x, float y) {
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;

	return &m_cells[y * m_numXCells + x];
}

void Grid::removeBall(Ball* ball) {
	std::vector<Ball*>& balls = ball->cell->balls;

	balls[ball->placeInCell] = balls.back();
	balls.pop_back();

	if (ball->placeInCell < balls.size()) balls[ball->placeInCell]->placeInCell = ball->placeInCell;

	ball->placeInCell = -1;
	ball->cell = nullptr;
}