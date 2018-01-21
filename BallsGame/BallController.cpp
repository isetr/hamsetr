#include "BallController.h";

#include "Grid.h"

void BallController::updateBalls(std::vector<Ball>& balls, Grid* grid, float deltaTime, int width, int height) {
	const float FRICTION = 0.01f;

	if (m_grabbedBall != -1) {
		balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
	}

	glm::vec2 gravity = getGravity();

	for (auto i = 0; i < balls.size(); i++) {
		if (i != m_grabbedBall) {
			balls[i].position += balls[i].velocity * deltaTime;

			glm::vec2 momentumVec = balls[i].velocity * balls[i].mass;

			if (momentumVec.x != 0 || momentumVec.y != 0) {
				if (FRICTION < glm::length(momentumVec)) {
					balls[i].velocity -= deltaTime * FRICTION * glm::normalize(momentumVec) / balls[i].mass;
				} else {
					balls[i].velocity = glm::vec2(0.0f);
				}
			}

			balls[i].velocity += gravity * deltaTime;
		}

		collideWithWall(balls[i], width, height);

		Cell* newCell = grid->getCell(balls[i].position);
		if (newCell != balls[i].cell) {
			grid->removeBall(&balls[i]);
			grid->addBallToCell(&balls[i], newCell);
		}
	}

	updateCollision(grid);
	
	if (m_grabbedBall != -1) {
		balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
		m_prevPos = balls[m_grabbedBall].position;
	}
}

void BallController::onMouseDown(std::vector<Ball>& balls, float mouseX, float mouseY) {
	for (size_t i = 0; i < balls.size(); i++) {
		if (isMouseOnBall(balls[i], mouseX, mouseY)) {
			m_grabbedBall = i;
			m_grabOffset = glm::vec2(mouseX, mouseY) - balls[i].position;
			m_prevPos = balls[i].position;
			balls[i].velocity = glm::vec2(0.0f);
		}
	}
}

void BallController::onMouseMove(std::vector<Ball>& balls, float mouseX, float mouseY) {
	if (m_grabbedBall != -1) {
		balls[m_grabbedBall].position = glm::vec2(mouseX, mouseY) - m_grabOffset;
	}
}

void BallController::onMouseUp(std::vector<Ball>& balls) {
	if (m_grabbedBall != -1) {
		balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
		m_grabbedBall = -1;
	}
}

void BallController::updateCollision(Grid* grid) {
	for (int i = 0; i < grid->m_cells.size(); i++) {
		int x = i % grid->m_numXCells;
		int y = i / grid->m_numXCells;

		Cell& cell = grid->m_cells[i];

		for (int j = 0; j < cell.balls.size(); j++) {
			Ball* ball = cell.balls[j];
			
			collideInCell(ball, cell.balls, j + 1);

			if (x > 0) {
				collideInCell(ball, grid->getCellByPos(x - 1, y)->balls, 0);
				if (y > 0) {
					collideInCell(ball, grid->getCellByPos(x - 1, y - 1)->balls, 0);
				}
				if (y < grid->m_numYCells - 1) {
					collideInCell(ball, grid->getCellByPos(x - 1, y + 1)->balls, 0);
				}
			}
			if (y > 0) {
				collideInCell(ball, grid->getCellByPos(x, y - 1)->balls, 0);
			}
		}
	}
}

void BallController::collideWithWall(Ball& ball, int width, int height) {
	if (ball.position.x < ball.radius) {
		ball.position.x = ball.radius;
		if (ball.velocity.x < 0) ball.velocity.x *= -1;
	}
	else if (ball.position.x + ball.radius >= width) {
		ball.position.x = width - ball.radius - 1;
		if (ball.velocity.x > 0) ball.velocity.x *= -1;
	}
	if (ball.position.y < ball.radius) {
		ball.position.y = ball.radius;
		if (ball.velocity.y < 0) ball.velocity.y *= -1;
	}
	else if (ball.position.y + ball.radius >= height) {
		ball.position.y = height - ball.radius - 1;
		if (ball.velocity.y > 0) ball.velocity.y *= -1;
	}
}

void BallController::collideInCell(Ball* ball, std::vector<Ball*>& balls, int offset) {
	for (auto i = offset; i < balls.size(); i++) {
		collideWithBall(*ball, *balls[i]);
	}
}

void BallController::collideWithBall(Ball& ball1, Ball& ball2) {
	glm::vec2 distVec = ball2.position - ball1.position;
	glm::vec2 distDir = glm::normalize(distVec);

	float dist = glm::length(distVec);
	float totalRadius = ball1.radius + ball2.radius;

	float collisionDepth = totalRadius - dist;

	if (collisionDepth > 0) {
		ball1.position -= distDir * collisionDepth * (ball2.mass / ball1.mass) * 0.5f;
		ball2.position += distDir * collisionDepth * (ball1.mass / ball2.mass) * 0.5f;

		float aci = glm::dot(ball1.velocity, distDir);
		float bci = glm::dot(ball2.velocity, distDir);

		float acf = (aci * (ball1.mass - ball2.mass) + 2 * ball2.mass * bci) / (ball1.mass + ball2.mass);
		float bcf = (bci * (ball2.mass - ball1.mass) + 2 * ball1.mass * aci) / (ball1.mass + ball2.mass);

		ball1.velocity += (acf - aci) * distDir;
		ball2.velocity += (bcf - bci) * distDir;

		if (glm::length(ball1.velocity + ball2.velocity) > 0.5f) {
			bool choice = glm::length(ball1.velocity) < glm::length(ball2.velocity);

			choice ? ball2.color = ball1.color : ball1.color = ball2.color;
		}
	}
}

bool BallController::isMouseOnBall(Ball& ball, float mouseX, float mouseY) {
	return (mouseX >= ball.position.x - ball.radius && mouseX < ball.position.x + ball.radius &&
		mouseY >= ball.position.y - ball.radius && mouseY < ball.position.y + ball.radius);
}

glm::vec2 BallController::getGravity() {
	const float GRAVITY_FORCE = 0.02f;
	glm::vec2 gravity;

	switch (m_gravity) {
	case Gravity::DOWN:
		gravity = glm::vec2(0.0f, -GRAVITY_FORCE);
		break;
	case Gravity::LEFT:
		gravity = glm::vec2(-GRAVITY_FORCE, 0.0f);
		break;
	case Gravity::RIGHT:
		gravity = glm::vec2(GRAVITY_FORCE, 0.0f);
		break;
	case Gravity::UP:
		gravity = glm::vec2(0.0f, GRAVITY_FORCE);
		break;
	default:
		gravity = glm::vec2(0.0f);
		break;
	}
	return gravity;
}