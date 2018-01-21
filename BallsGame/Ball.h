#pragma once

#include <glm/glm.hpp>

#include <Hamsetr/Vertex.h>

struct Cell;

struct Ball {
	Ball(float Radius, float Mass, glm::vec2& Position, glm::vec2& Velocity,
	     Hamsetr::ColorRGBA8& Color, GLuint& TextureID);

	float radius;
	float mass;
	glm::vec2 position;
	glm::vec2 velocity;
	Hamsetr::ColorRGBA8 color;
	GLuint textureID = 0;
	Cell* cell = nullptr;
	int placeInCell = -1;
};