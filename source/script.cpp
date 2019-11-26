/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "pch.h"
#include "Script.h"
#include "Menu.h"
#include "ControlManager.h"
#include "Routine.h"
#include "ActionManager.h"
#include "ToggleManager.h"

void setup()
{
	ActionManager::RegisterActions();
	ToggleManager::RegisterToggles();
	Game::RequestTextureDict("menu_textures");
	Game::RequestTextureDict("boot_flow");
}

void main()
{
	setup();
	Menu* menu = new Menu();
	while (true) {
		Routine::RunAll();
		ControlManager::Tick();
		menu->Tick();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}