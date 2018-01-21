#include "App.h"



App::App()
{
}


App::~App()
{
}

void App::onInit() {

}

void App::onExit() {

}

void App::addScreens() {
	m_gameplayScreen = std::make_unique<GameplayScreen>(&m_window);

	m_screenList->addScreen(m_gameplayScreen.get());
	m_screenList->setScreen(m_gameplayScreen->getScreenIndex());
}
