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
#include "SettingsGeneralSub.h"
#include "MenuSettings.h"
#include "JsonData.h"

SettingsGeneralSub::SettingsGeneralSub(MenuController* menuController) : Submenu(menuController)
{
}

#pragma region Draw

void SettingsGeneralSub::Draw()
{
	Submenu::Draw();

	DrawTitle("General");
	DrawToggle("UI sounds", Settings::General::playUiSounds, [] {
		Settings::General::playUiSounds = !Settings::General::playUiSounds;
		JSONData::SaveMenuSettings();
	});
}

#pragma endregion
