#pragma once

#include <memory>
#include <vector>

#include <Box2D/Box2D.h>
#include <Hamsetr/IGameScreen.h>
#include <Hamsetr/SpriteBatch.h>
#include <Hamsetr/GLSLProgram.h>
#include <Hamsetr/Camera2D.h>
#include <Hamsetr/GLTexture.h>
#include <Hamsetr/Window.h>

#include "Box.h"
#include "Player.h"

class GameplayScreen : public Hamsetr::IGameScreen
{
public:
	GameplayScreen(Hamsetr::Window* m_window);
	~GameplayScreen();

	virtual int getNextScreenIndex() const override;


	virtual int getPreviousScreenIndex() const override;


	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

private:
	void checkInput();
	std::unique_ptr<b2World> m_world;
	std::vector<Box> m_boxes;

	Hamsetr::GLSLProgram m_program;
	Hamsetr::Camera2D m_camera;
	Hamsetr::SpriteBatch m_spriteBatch;
	Hamsetr::GLTexture m_texture;
	Hamsetr::Window* m_window;

	Player m_player;
};

