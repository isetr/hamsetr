#include "Renderer.h"

void Renderer::renderBatch(Hamsetr::SpriteBatch& spriteBatch, std::vector<Ball>& balls, glm::mat4& cameraMatrix) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_program == nullptr) {
		m_program = std::make_unique<Hamsetr::GLSLProgram>();
		m_program->compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	for (auto& ball : balls) {
		glm::vec4 destRect(ball.position.x - ball.radius, 
						   ball.position.y - ball.radius,
						   ball.radius * 2.0f, ball.radius * 2.0f);
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		spriteBatch.draw(destRect, uvRect, ball.textureID, 0.0f, ball.color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unUse();
}

void MomentumRenderer::renderBatch(Hamsetr::SpriteBatch& spriteBatch, std::vector<Ball>& balls, glm::mat4& cameraMatrix) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_program == nullptr) {
		m_program = std::make_unique<Hamsetr::GLSLProgram>();
		m_program->compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	for (auto& ball : balls) {
		glm::vec4 destRect(ball.position.x - ball.radius,
			ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		Hamsetr::ColorRGBA8 color;
		GLubyte colorVal = (GLubyte)(glm::clamp(glm::length(ball.velocity) * ball.mass * 12.0f, 0.0f, 255.0f));
		color.r = colorVal;
		color.g = colorVal;
		color.b = colorVal;
		color.a = colorVal;
		spriteBatch.draw(destRect, uvRect, ball.textureID, 0.0f, color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unUse();
}

SinCosRenderer::SinCosRenderer(int width, int height) :
	width(width),
	height(height),
	m_time(0) {

}

void SinCosRenderer::renderBatch(Hamsetr::SpriteBatch& spriteBatch, std::vector<Ball>& balls, glm::mat4& cameraMatrix) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_program == nullptr) {
		m_program = std::make_unique<Hamsetr::GLSLProgram>();
		m_program->compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	const float TIME_STEP = 0.05f;

	m_time += TIME_STEP;

	for (auto& ball : balls) {
		glm::vec4 destRect(ball.position.x - ball.radius,
			ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		Hamsetr::ColorRGBA8 color = ball.color;
		color.r = (GLubyte)(cos (ball.position.x * m_time) + 1.0f) * 0.5f * 255;
		color.g = (GLubyte)(sin (ball.position.x * m_time) + 1.0f) * 0.5f * 255;
		color.b = (GLubyte)(cos (ball.position.y * m_time) + 1.0f) * 0.5f * 255;
		//color.a *= (GLubyte)(glm::clamp(abs(sin(ball.mass)), 0.0f, 255.0f));
		spriteBatch.draw(destRect, uvRect, ball.textureID, 0.0f, color);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unUse();
}