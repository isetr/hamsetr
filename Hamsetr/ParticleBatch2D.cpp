#include "ParticleBatch2D.h"

namespace Hamsetr {


	ParticleBatch2D::ParticleBatch2D()
	{
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc) {
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];

		m_decayRate = decayRate;
		m_texture = texture;
		m_updateFunc = updateFunc;
	}

	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; i++) {
			if (m_particles[i].m_life > 0.0f) {
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].m_life -= m_decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		for (int i = 0; i < m_maxParticles; i++) {
			auto& p = m_particles[i];
			if (m_particles[i].m_life > 0.0f) {
				glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.m_color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const ColorRGBA8& color, const glm::vec2& velocity, float width) {
		int particleIndex = findFreeParticle();

		auto& p = m_particles[particleIndex];

		p.m_life = 1.0f;
		p.m_position = position;
		p.m_color = color;
		p.m_velocity = velocity;
		p.m_width = width;
	}

	int ParticleBatch2D::findFreeParticle() {
		for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
			if (m_particles[i].m_life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}
		for (int i = 0; i < m_lastFreeParticle; i++) {
			if (m_particles[i].m_life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		return 0;
	}
}