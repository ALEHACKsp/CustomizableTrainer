/*
* Customizable Trainer
* Copyright (C) 2020  Gu�mundur �li
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*/

#include "pch.h"
#include "SettingsControlsSub.h"

SettingsControlsSub::SettingsControlsSub(MenuController* menuController) 
	: Submenu(menuController)
{}

void SettingsControlsSub::Draw()
{
	Submenu::Draw();

	DrawTitle("Controls");
	DrawSub("Keyboard", "builtin_sub_settingsControlsKeyboard");
	DrawSub("Controller", "builtin_sub_settingsControlsController");
}
