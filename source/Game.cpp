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
#include "Game.h"
#include "Routine.h"
#include "MenuSettings.h"

void Game::DrawText(std::string text, Vector2 position, float scale, Color color, bool center) {
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_SCALE(1.0f, scale / 100.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a);
	char* varString = GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", (char*) text.c_str());
	varString = invoke<char*, int, char*, int, char*>(0xFA925AC00EB830B9, 42, "COLOR_STRING", 0, varString);
	UI::DRAW_TEXT(varString, position.x / 100.0f, position.y / 100.0f);
}

void Game::DrawSprite(std::string textureDict, std::string textureName, Vector3 position, Rect size, float heading, Color color) 
{
	GRAPHICS::DRAW_SPRITE((char*)textureDict.c_str(), (char*)textureName.c_str(), position.x / 100.0f, position.y / 100.0f,
							size.width / 100.0f, size.height / 100.0f, heading, color.r, color.g, color.b, color.a, false);
}

void Game::DrawRect(Vector2 position, Rect size, Color color)
{
	GRAPHICS::DRAW_RECT(position.x / 100.0f, position.y / 100.0f,
						size.width / 100.0f, size.height / 100.0f, color.r, color.g, color.b, color.a, false, false);
}

void Game::PrintSubtitle(std::string text)
{
	char* varString = GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", (char*)text.c_str());
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(varString);
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}

void Game::PrintDebug(std::string text)
{
	if (Settings::General::debug) {
		PrintSubtitle(text);
	}
}

void Game::PlaySoundFrontend(std::string soundCat, std::string soundName)
{
	int soundId = AUDIO::GET_SOUND_ID();

	AUDIO::_0xCE5D0FFE83939AF1(soundId, (char*) soundName.c_str(), (char*) soundCat.c_str(), 1);
}

bool Game::RequestModel(Hash model)
{
	STREAMING::REQUEST_MODEL(model, true);
	auto timeout = GetTickCount() + 1000;
	while (!STREAMING::HAS_MODEL_LOADED(model)) {
		if (GetTickCount() > timeout) {
			Game::PrintSubtitle("Error: Loading model timed out");
			return false;
		}

		TaskQueue::Wait(5);
	}
	return true;
}

void Game::SetModelAsNoLongerNeeded(Hash model)
{
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
}

void Game::SetWeather(Hash weather)
{
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	GAMEPLAY::SET_WEATHER_TYPE(weather, true, true, true, 0.0, true);
	GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
}

void Game::RequestTextureDict(std::string textureDict)
{
	TEXTURE::REQUEST_STREAMED_TEXTURE_DICT((char*)textureDict.c_str(), true);
	while (!TEXTURE::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)textureDict.c_str())) {
		WAIT(5);
	}
}

bool Game::RequestAnimDict(std::string animDict)
{
	if (!STREAMING::DOES_ANIM_DICT_EXIST((char*)animDict.c_str())) {
		Game::PrintSubtitle("Error: Anim dict '" + animDict + "' does not exist");
		return false;
	}

	STREAMING::REQUEST_ANIM_DICT((char*)animDict.c_str());
	auto timeout = GetTickCount() + 1000;
	while (!STREAMING::HAS_ANIM_DICT_LOADED((char*)animDict.c_str())) {
		if (GetTickCount() > timeout) {
			Game::PrintSubtitle("Error: Loading anim dict timed out");
			return false;
		}

		TaskQueue::Wait(5);
	}
}

std::optional<std::string> Game::GetInputWithKeyboard(std::string defaultText)
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "", "", (char*) defaultText.c_str(), "", "", "", 100);
	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
		TaskQueue::Wait(0);

	switch (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD()) {
		case 1: return GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
		case 2: return std::nullopt;
		case 3:
			Game::PrintSubtitle("Error: Unknown keyboard error");
			return std::nullopt;
	}
}

// Doesn't actully work (GET_SCREEN_RESOLUTION just returns 1280 / 720 no matter what)
float Game::AspectRatio()
{
	int resX, resY;
	GRAPHICS::GET_SCREEN_RESOLUTION(&resX, &resY);
	return resX / resY;
}

char* Game::GetVarString(std::string text)
{
	char* varString = GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", (char*)text.c_str());
	return invoke<char*, int, char*, int, char*>(0xFA925AC00EB830B9, 42, "COLOR_STRING", 0, varString);
}

void Game::UpdateData()
{
	playerId = PLAYER::PLAYER_ID();
	playerPedId = PLAYER::PLAYER_PED_ID();
}