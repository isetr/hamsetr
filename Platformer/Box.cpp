#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}

void Box::init(b2World* world, glm::vec2& pos, glm::vec2& dim, Hamsetr::GLTexture texture, 
				Hamsetr::ColorRGBA8& color, bool fixedRotation, 
				glm::vec4 uvRect) {
	m_dimensions = dim;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dim.x / 2.0f, dim.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::draw(Hamsetr::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(m_body->GetPosition().x - m_dimensions.x / 2.0f,
						m_body->GetPosition().y - m_dimensions.y / 2.0f,
						m_dimensions.x,
						m_dimensions.y);

	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());
}
