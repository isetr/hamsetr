#include "Player.h"

#include <SDL/SDL.h>

#include <Hamsetr/ResourceManager.h>

Player::Player()
{
}


Player::~Player()
{
}

void Player::init(b2World* world, glm::vec2& pos, glm::vec2& dim, Hamsetr::ColorRGBA8& color) {
	Hamsetr::GLTexture texture = Hamsetr::ResourceManager::getTexture("Textures/blue_ninja.png");
	m_collisionBox.init(world, pos, dim, texture, color, true, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));
}

void Player::update(Hamsetr::InputManager& inputManager) {
	b2Body* body = m_collisionBox.getBody();

	if (inputManager.isKeyDown(SDLK_a)) {
		m_collisionBox.getBody()->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		m_collisionBox.getBody()->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));
	}

	float maxSpeed = 5.0f;

	if (body->GetLinearVelocity().x < -maxSpeed) {
		body->SetLinearVelocity(b2Vec2(-maxSpeed, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > maxSpeed) {
		body->SetLinearVelocity(b2Vec2(maxSpeed, body->GetLinearVelocity().y));
	}

	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			bool below = false;

			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - m_collisionBox.getDimensions().y / 2.0f + 0.01f) {
					below = true;
					break;
				}
			}
			if (below) {
				if (inputManager.isKeyPressed(SDLK_w)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}

void Player::draw(Hamsetr::SpriteBatch& spriteBatch) {
	m_collisionBox.draw(spriteBatch);
}
