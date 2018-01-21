#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <Hamsetr/Vertex.h>
#include <Hamsetr/SpriteBatch.h>
#include <Hamsetr/GLTexture.h>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, glm::vec2& pos, glm::vec2& dim, Hamsetr::GLTexture texture, 
				Hamsetr::ColorRGBA8& color, bool fixedRotation = false, 
				glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void draw(Hamsetr::SpriteBatch& spriteBatch);

	b2Body* getBody() const { return m_body; };
	b2Fixture* getFixture() const { return m_fixture; };
	const glm::vec2& getDimensions() const { return m_dimensions; };
	const Hamsetr::ColorRGBA8& getColor() const { return m_color; };

private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	glm::vec4 m_uvRect;
	Hamsetr::ColorRGBA8 m_color;
	Hamsetr::GLTexture m_texture;
};

