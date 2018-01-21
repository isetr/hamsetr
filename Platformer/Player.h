#pragma once

#include <Hamsetr/SpriteBatch.h>
#include <Hamsetr/GLTexture.h>
#include <Hamsetr/InputManager.h>

#include "Box.h"

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, glm::vec2& pos, glm::vec2& dim, Hamsetr::ColorRGBA8& color);

	void update(Hamsetr::InputManager& inputManager);
	void draw(Hamsetr::SpriteBatch& spriteBatch);

	const Box& getBox() const { return m_collisionBox; };

private:
	Box m_collisionBox;

};

