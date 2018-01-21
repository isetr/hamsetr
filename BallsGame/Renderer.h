#pragma once

#include <glm/glm.hpp>

#include <Hamsetr/GLSLProgram.h>
#include <Hamsetr/SpriteBatch.h>

#include <vector>
#include <memory>

#include "Ball.h"

class Renderer {
public:
	virtual void renderBatch(Hamsetr::SpriteBatch& spriteBatch, std::vector<Ball>& balls, glm::mat4& cameraMatrix);
protected:
	std::unique_ptr<Hamsetr::GLSLProgram> m_program = nullptr;
};

class MomentumRenderer : public Renderer {
public:
	virtual void renderBatch(Hamsetr::SpriteBatch& spriteBatch, std::vector<Ball>& balls, glm::mat4& cameraMatrix) override;
protected:
	std::unique_ptr<Hamsetr::GLSLProgram> m_program = nullptr;
};

class SinCosRenderer : public Renderer {
public:
	SinCosRenderer(int width, int height);

	virtual void renderBatch(Hamsetr::SpriteBatch& spriteBatch, std::vector<Ball>& balls, glm::mat4& cameraMatrix) override;
protected:
	std::unique_ptr<Hamsetr::GLSLProgram> m_program = nullptr;
	float m_time;
	int width, height;
};
