#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace Hamsetr {

	class Particle2D {
	public:
		glm::vec2 m_position = glm::vec2(0.0f);
		glm::vec2 m_velocity = glm::vec2(0.0f);
		ColorRGBA8 m_color;
		float m_life = 0.0f;
		float m_width = 0.0f;
	};

	inline void defaulParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.m_position += particle.m_velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc = defaulParticleUpdate);

		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);

		void addParticle(const glm::vec2& position, const ColorRGBA8& color, const glm::vec2& velocity, float width);

	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)> m_updateFunc;
		float m_decayRate = 0.1f;
		int m_maxParticles = 0;
		GLTexture m_texture;
		int m_lastFreeParticle = 0;

		Particle2D* m_particles = nullptr;
	};

}