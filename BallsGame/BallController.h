#pragma once

#include <vector>

#include "Ball.h"

class Grid;

enum class Gravity { NONE, UP, DOWN, LEFT, RIGHT };

class BallController {
public:
	void updateBalls(std::vector<Ball>& balls, Grid* grid, float deltaTime, int width, int height);

	void onMouseDown(std::vector<Ball>& balls, float mouseX, float mouseY);
	void onMouseMove(std::vector<Ball>& balls, float mouseX, float mouseY);
	void onMouseUp(std::vector<Ball>& balls);

	void setGravity(Gravity direction) { m_gravity = direction; };
private:
	void updateCollision(Grid* grid);
	void collideWithWall(Ball& ball, int width, int height);
	void collideInCell(Ball* ball, std::vector<Ball*>& balls, int offset);
	void collideWithBall(Ball& ball1, Ball& ball2);

	bool isMouseOnBall(Ball& ball, float mouseX, float mouseY);

	glm::vec2 getGravity();

	int m_grabbedBall = -1;
	glm::vec2 m_prevPos = glm::vec2(0.0f);
	glm::vec2 m_grabOffset = glm::vec2(0.0f);

	Gravity m_gravity = Gravity::NONE;

};