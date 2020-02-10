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
#include "DynamicSubmenu.h"
#include "AddOptionSub.h"
#include "Routine.h"
#include "Controls.h"
#include "ActionController.h"
#include "ToggleController.h"
#include "NumberController.h"
#include "TextController.h"
#include "MenuSettings.h"
#include "JsonData.h"

DynamicSubmenu::DynamicSubmenu(SubmenuData submenuData, MenuController* menuController)
	: Submenu(menuController)
{
	this->title = submenuData.title;
	this->key = submenuData.key;
	this->options = submenuData.options;
	isEditModeActive = false;
	isMoveOptionActive = false;
}

#pragma region Draw

void DynamicSubmenu::Draw()
{
	Submenu::Draw();
	DrawTitle(title);

	optionAddIndex = scrollPosition - 1;
	for (int i = scrollPosition; i < ((OptionCount() > 8) ? (scrollPosition + 8) : OptionCount()); i++) {
		auto option = options[i];
		switch (option.type) {
		case MenuOptionType::Sub:
			DrawSub(option.text, option.key);
			break;
		case MenuOptionType::Action:
			DrawAction(option.text, option.key, option.params);
			break;
		case MenuOptionType::Toggle:
			DrawToggle(option.text, option.key);
			break;
		case MenuOptionType::Number:
			DrawNumber(option.text, option.key);
			break;
		case MenuOptionType::Text:
			DrawTextList(option.text, option.key);
			break;
		}
	}
}

void DynamicSubmenu::DrawMenuBase()
{
	Submenu::DrawMenuBase();
	auto& menuPos = menuController->position;

	// Edit mode indicator
	if (isEditModeActive) {
		std::string statusText = "";

		if (isMoveOptionActive)
			statusText = "Moving option";
		else
			statusText = "Edit mode";

		Game::DrawText("<font face='$body2' >" + statusText + "</font>", { menuPos.x + 10.0f, menuPos.y + 47.0f }, 30.0f, { 100, 100, 100, 255 }, true);
	}
}

#pragma endregion

#pragma region Draw option

void DynamicSubmenu::DrawSub(std::string text, std::string subKey)
{
	Submenu::DrawSub(text, subKey, !isEditModeActive);
}

void DynamicSubmenu::DrawAction(std::string text, std::string actionKey, json actionParams)
{
	Submenu::DrawAction(text, [this, actionKey, actionParams] () {
		if (!isEditModeActive) {
			ActionController::RunActionForKey(actionKey, actionParams);
		}
	});
}

void DynamicSubmenu::DrawToggle(std::string text, std::string toggleKey)
{
	if (!ToggleController::ToggleExistsForKey(toggleKey)) return;
	bool isToggled = *ToggleController::GetToggleForKey(toggleKey).value();

	Submenu::DrawToggle(text, isToggled, [this, toggleKey] {
		if (!isEditModeActive) {
			ToggleController::Toggle(toggleKey);
			SaveIfSavedOption(toggleKey);
		}
	});
}

void DynamicSubmenu::DrawNumber(std::string text, std::string numberKey)
{
	if (!NumberController::NumberExistsForKey(numberKey)) return;
	std::string numberStrValue = NumberController::GetNumberStringValueForKey(numberKey).value_or("Invalid");

	Submenu::DrawNumber(text, numberStrValue, [this, numberKey, numberStrValue] {
		if (!NumberController::NumberVariableExistsForKey(numberKey)) return;
		std::string input = Game::GetInputWithKeyboard(numberStrValue);
		NumberController::SetNumberValueForKey(numberKey, input);
		SaveIfSavedOption(numberKey);
	}, [this, numberKey](bool direction) {
		if (isEditModeActive) return;
		NumberController::Adjust(numberKey, direction);
		SaveIfSavedOption(numberKey);
	});
}

void DynamicSubmenu::DrawTextList(std::string text, std::string textKey)
{
	if (!TextController::TextExistsForKey(textKey)) return;
	std::string textValue = TextController::GetTextValueForKey(textKey)
		.value_or("Invalid");

	Submenu::DrawTextList(text, textValue,
		[this, textKey](bool direction) {
			if (isEditModeActive) return;

			TextController::Adjust(textKey, direction);
			SaveIfSavedOption(textKey);
	});
}

#pragma endregion

#pragma region Events

void DynamicSubmenu::SubWillDraw()
{
	Submenu::SubWillDraw();
}

void DynamicSubmenu::SelectionDidChange(int to, int from)
{
	Submenu::SelectionDidChange(to, from);

	if (isEditModeActive && isMoveOptionActive) {
		if (to == 0 && from == OptionCount() - 1) { // From bottom to top
			options.insert(options.begin(), options[from]);
			options.erase(options.begin() + from + 1);
		}
		else if (to == OptionCount() - 1 && from == 0) { // From top to bottom
			options.push_back(options[from]);
			options.erase(options.begin());
		}
		else {
			std::swap(options[from], options[to]);
		}
	}
}

#pragma endregion
 
#pragma region Controls

void DynamicSubmenu::RespondToControls()
{
	Submenu::RespondToControls();

	// Edit mode
	if (!isEditModeActive && Controls::IsMenuControlPressed(MenuControl::MenuEditModeEnter)) {
		isEditModeActive = true;
		Controls::CanceMenuControlslForThisFrame();
	}
	if (isEditModeActive) {
		if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeMoveOptionn)) {
			isMoveOptionActive = !isMoveOptionActive;
		}
		if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeExitAndSave)) {
			isMoveOptionActive = false;
			isEditModeActive = false;
			SubmenuData updatedSubmenuData = { title, key, options };
			menuController->UpdateSubmenuData(key, updatedSubmenuData);
			Game::PrintSubtitle("Saved");
		}
		if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeExit)) {
			isMoveOptionActive = false;
			isEditModeActive = false;
		}
		if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeAddOption)) {
			auto addOptionSub = new EditAddOptionSub(std::nullopt, menuController);
			addOptionSub->onAddOption = [this](MenuOption optionToAdd) {
				options.push_back(optionToAdd);
			};
			menuController->AddSubmenuToStack(addOptionSub);
			Controls::CanceMenuControlslForThisFrame();
		}
		if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeEditOption)) {
			auto addOptionSub = new EditAddOptionSub({ options[selection] }, menuController);
			addOptionSub->onAddOption = [this](MenuOption optionToEdit) {
				options[selection] = optionToEdit;
			};
			menuController->AddSubmenuToStack(addOptionSub);
			Controls::CanceMenuControlslForThisFrame();
		}
		if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeDeleteOption)) {
			options.erase(options.begin() + selection);
			if (selection > OptionCount() - 1) {
				if (scrollPosition != 0) scrollPosition--;
				selection--;
			}
		}
	}
}

#pragma endregion

#pragma region Getters

int DynamicSubmenu::OptionCount()
{
	return options.size();
}

#pragma endregion

#pragma region Booleans

bool DynamicSubmenu::IsSavedOption(std::string key)
{
	for each (auto optionToSave in MenuSettings::optionsToSave) {
		if (optionToSave.key == key)
			return true;
	}

	return false;
}

#pragma endregion

void DynamicSubmenu::SaveIfSavedOption(std::string key)
{
	if (IsSavedOption(key))
		JSONData::SaveOptionStates();
}