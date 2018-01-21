#pragma once

#include <Hamsetr/IMainGame.h>
#include "GameplayScreen.h"

class App : public Hamsetr::IMainGame
{
public:
	App();
	~App();

	virtual void onInit() override;
	virtual void onExit() override;
	virtual void addScreens() override;
private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

