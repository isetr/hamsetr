#include "MainGame.h"

#include <glm/glm.hpp>

#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

#include <Hamsetr/ResourceManager.h>

const float MAX_FPS = 60.0f;
const float DESIRED_FPS = 60.0f;
const int MAX_STEPS = 6;
const float MS_PER_SEC = 1000;
const float DESIRED_FRAMERATE = MS_PER_SEC / DESIRED_FPS;
const float MAX_DELTA_TIME = 1.0f;

const int NUM_BALLS = 10000;
const int CELL_SIZE = 6;

const float MIN_RADIUS = 1.0f;
const float MAX_RADIUS = 5.0f;

MainGame::MainGame() {

}

MainGame::~MainGame() {
	for (auto& renderer : m_renderers) {
		delete renderer;
	}

	m_renderers.clear();
}

void MainGame::run() {
	init();

	GLuint prevTicks = SDL_GetTicks();

	while (m_gameState == GameState::PLAY) {
		gameLoop(prevTicks);
	}
	
}

void MainGame::init() {
	Hamsetr::init();

	m_window.create("Ball Game", m_screenWidth, m_screenHeight, 0);

	m_camera.init(m_screenWidth, m_screenHeight);
	m_camera.setPosition(glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f));

	m_spriteBatch.init();

	m_spriteFont = new Hamsetr::SpriteFont("Fonts/font.ttf", 40);

	m_fpsLimiter.init(MAX_FPS);

	initShaders();
	initRenderers();
	initBalls();
}

void MainGame::initShaders() {
	m_program.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_program.addAttribute("vertexPosition");
	m_program.addAttribute("vertexColor");
	m_program.addAttribute("vertexUV");
	m_program.linkShaders();
}

void MainGame::initRenderers() {
	m_renderers.push_back(new Renderer);
	m_renderers.push_back(new MomentumRenderer);
	m_renderers.push_back(new SinCosRenderer(m_screenWidth, m_screenHeight));
}

void MainGame::initBalls() {

	m_grid = std::make_unique<Grid>(m_screenWidth, m_screenHeight, CELL_SIZE, (int)(NUM_BALLS / CELL_SIZE));

	m_balls.reserve(NUM_BALLS);

	std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_int_distribution<int> randomColor(0,255);
	std::uniform_real_distribution<float> randomFloat(MIN_RADIUS, MAX_RADIUS);
	std::uniform_real_distribution<float> randomX(0.0f, (float)m_screenWidth);
	std::uniform_real_distribution<float> randomY(0.0f, (float)m_screenHeight);
	std::uniform_real_distribution<float> randomVelocity(-1.0f, 1.0f);
	std::uniform_real_distribution<float> randomSpeed(0.0f, 0.5f);

	for (int i = 0; i < NUM_BALLS; i++) {
		float SizeAndMass = randomFloat(randomEngine);
					
		glm::vec2 velocity(randomVelocity(randomEngine), randomVelocity(randomEngine));
		if (velocity.x != 0.0f || velocity.y != 0.0f) {
			velocity = glm::normalize(velocity);
		}
		else {
			velocity = glm::vec2(1.0f, 0.0f);
		}

		m_balls.emplace_back(SizeAndMass, SizeAndMass, glm::vec2(randomX(randomEngine), randomY(randomEngine)), velocity * randomSpeed(randomEngine),
			Hamsetr::ColorRGBA8(randomColor(randomEngine), randomColor(randomEngine), randomColor(randomEngine), 255),
			Hamsetr::ResourceManager::getTexture("Textures/circle.png").id);
		m_grid->addBall(&m_balls.back());
	}
}

void MainGame::gameLoop(GLuint& prevTicks) {
	m_fpsLimiter.begin();

	processInput();

	Uint32 newTicks = SDL_GetTicks();
	Uint32 frameTime = newTicks - prevTicks;
	prevTicks = newTicks;

	float totalDeltaTime = (float)frameTime / DESIRED_FRAMERATE;

	int i = 0;

	while (i < MAX_STEPS && totalDeltaTime > 0.0f) {
		float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

		update(deltaTime);

		totalDeltaTime -= deltaTime;

		i++;
	}

	m_camera.update();

	draw();

	m_fps = m_fpsLimiter.end();
}

void MainGame::update(float deltaTime) {
	m_ballController.updateBalls(m_balls, m_grid.get(), deltaTime, m_screenWidth, m_screenHeight);
}

void MainGame::draw() {
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

	m_renderers[m_currentRenderer]->renderBatch(m_spriteBatch, m_balls, cameraMatrix);

	m_program.use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint pUniform = m_program.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	drawHud();

	m_program.unUse();

	m_window.swapBuffer();
}

void MainGame::drawHud() {
	const Hamsetr::ColorRGBA8 fontColor(255, 0, 0, 255);

	char buffer[64];
	sprintf_s(buffer, "%.1f", m_fps);

	m_spriteBatch.begin();

	m_spriteFont->draw(m_spriteBatch, buffer, glm::vec2(0.0f, m_screenHeight - 45.0f), glm::vec2(1.0f), 0.0f, fontColor);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void MainGame::processInput() {
	m_inputManager.update();

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			m_ballController.onMouseMove(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_ballController.onMouseDown(m_balls, (float)evnt.button.x, (float)m_screenHeight - (float)evnt.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			m_ballController.onMouseUp(m_balls);
			break;
		}
	}

	if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) m_gameState = GameState::EXIT;
	if (m_inputManager.isKeyPressed(SDLK_LEFT)) {
		m_ballController.setGravity(Gravity::LEFT);
	}
	else if (m_inputManager.isKeyPressed(SDLK_RIGHT)) {
		m_ballController.setGravity(Gravity::RIGHT);
	}
	else if (m_inputManager.isKeyPressed(SDLK_UP)) {
		m_ballController.setGravity(Gravity::UP);
	}
	else if (m_inputManager.isKeyPressed(SDLK_DOWN)) {
		m_ballController.setGravity(Gravity::DOWN);
	}
	else if (m_inputManager.isKeyPressed(SDLK_SPACE)) {
		m_ballController.setGravity(Gravity::NONE);
	}
	if (m_inputManager.isKeyPressed(SDLK_r)) {
		m_currentRenderer++;
		if (m_currentRenderer >= m_renderers.size()) m_currentRenderer = 0;
	}
}