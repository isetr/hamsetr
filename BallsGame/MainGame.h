#pragma once

#include <Hamsetr/Hamsetr.h>
#include <Hamsetr/Window.h>
#include <Hamsetr/Camera2D.h>
#include <Hamsetr/GLSLProgram.h>
#include <Hamsetr/InputManager.h>
#include <Hamsetr/SpriteBatch.h>
#include <Hamsetr/SpriteFont.h>
#include <Hamsetr/Timing.h>

#include <vector>
#include <memory>

#include "Renderer.h"
#include "BallController.h"
#include "Grid.h"

enum class GameState { PLAY, EXIT };

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();

private:
	void init();
	void initShaders();
	void initRenderers();
	void initBalls();
	void gameLoop(GLuint& prevTicks);
	void update(float deltaTime);
	void draw();
	void drawHud();
	void processInput();

	Hamsetr::Window m_window;	
	Hamsetr::Camera2D m_camera;
	Hamsetr::InputManager m_inputManager;
	Hamsetr::SpriteBatch m_spriteBatch;
	Hamsetr::SpriteFont* m_spriteFont;
	Hamsetr::FpsLimiter m_fpsLimiter;
	Hamsetr::GLSLProgram m_program;

	GameState m_gameState = GameState::PLAY;
	int m_screenWidth = 1280;
	int m_screenHeight = 720;
	float m_fps;

	int m_currentRenderer = 0;

	std::vector<Renderer*> m_renderers;
	std::vector<Ball> m_balls;
	BallController m_ballController;
	std::unique_ptr<Grid> m_grid;
};
