#include "Ball.h"

Ball::Ball(float Radius, float Mass, glm::vec2& Position, glm::vec2& Velocity,
	Hamsetr::ColorRGBA8& Color, GLuint& TextureID) :
	radius(Radius),
	mass(Mass),
	position(Position),
	velocity(Velocity),
	color(Color),
	textureID(TextureID),
	cell(nullptr),
	placeInCell(-1){
}