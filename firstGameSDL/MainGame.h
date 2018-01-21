#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>

#include <Hamsetr/Hamsetr.h>
#include <Hamsetr/Sprite.h>
#include <Hamsetr/GLSLProgram.h>
#include <Hamsetr/GLTexture.h>
#include <Hamsetr/Window.h>
#include <Hamsetr/Camera2D.h>
#include <Hamsetr/SpriteBatch.h>
#include <Hamsetr/InputManager.h>
#include <Hamsetr/Timing.h>
#include <Hamsetr/SpriteFont.h>
#include "Bullet.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void gameLoop();
	void processInput();
	void drawGame();
	void drawHud();
	void initShaders();

	Hamsetr::Window _window;
	int _screenWidth, _screenHeight;
	GameState _gameState;

	Hamsetr::GLSLProgram _colorProgram;
	Hamsetr::Camera2D _camera;

	Hamsetr::SpriteBatch _spriteBatch;
	Hamsetr::SpriteBatch _hudSpriteBatch;

	Hamsetr::InputManager _inputManager;
	Hamsetr::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	Hamsetr::SpriteFont* _spriteFont;

	float _maxFPS;
	float _fps;
	float _time;
};

