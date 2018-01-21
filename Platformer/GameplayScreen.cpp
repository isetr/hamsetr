#include "GameplayScreen.h"

#include <iostream>
#include <random>
#include <ctime>

#include <SDL/SDL.h>
#include <Hamsetr/IMainGame.h>
#include <Hamsetr/ResourceManager.h>

GameplayScreen::GameplayScreen(Hamsetr::Window* m_window) :
	m_window(m_window)
{
}


GameplayScreen::~GameplayScreen()
{
}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build() {
}

void GameplayScreen::destroy() {
}

void GameplayScreen::onEntry() {
	b2Vec2 gravity(0.0f, -40.0f);
	m_world = std::make_unique<b2World>(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	std::mt19937 rNG((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> xDist(-10.0f, 10.f);
	std::uniform_real_distribution<float> yDist(-10.0f, 15.f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);

	Hamsetr::ColorRGBA8 color(255, 255, 255, 255);
	m_texture = Hamsetr::ResourceManager::getTexture("Textures/bricks_top.png");

	const int NUM_BOXES = 50;

	for (int i = 0; i < NUM_BOXES; i++) {
		Box newBox;
		newBox.init(m_world.get(), glm::vec2(xDist(rNG), yDist(rNG)), glm::vec2(size(rNG), size(rNG)), m_texture, color);
		m_boxes.push_back(newBox);
	}

	m_spriteBatch.init();

	m_program.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_program.addAttribute("vertexPosition");
	m_program.addAttribute("vertexColor");
	m_program.addAttribute("vertexUV");
	m_program.linkShaders();

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f);

	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(1.0f, 2.0f), color);
}

void GameplayScreen::onExit() {
}

void GameplayScreen::update() {
	m_camera.update();
	checkInput();
	m_player.update(m_game->inputManager);

	m_world->Step(1.0f / 60.0f, 6, 2);
}

void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_program.use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_program.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	m_spriteBatch.begin();

	for (auto& b : m_boxes) {
		b.draw(m_spriteBatch);
	}

	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_program.unUse();
}

void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) m_game->onSDLEvent(evnt);
}
