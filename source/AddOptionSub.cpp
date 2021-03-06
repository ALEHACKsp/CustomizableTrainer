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
#include "AddOptionSub.h"
#include "AddOptionSetKeySub.h"
#include "AddOptionSetTypeSub.h"
#include "SettingsEditAddHotkeySub.h"
#include "ActionController.h"
#include "HotkeyController.h"
#include "Routine.h"
#include "Controls.h"

EditAddOptionSub::EditAddOptionSub(std::optional<MenuOption> optionToEdit, MenuController* menuController)
	: Submenu(menuController)
{
	if (optionToEdit) {
		title = "Edit option";
		this->optionToAdd = *optionToEdit;
		UpdateParameters();
	}
	else {
		title = "Add option";
		this->optionToAdd;
		UpdateParameters();
	}

}

void EditAddOptionSub::Draw()
{
	Submenu::Draw();

	DrawTitle(title);
	// Add as hotkey
	if (optionToAdd.type != MenuOptionType::Sub)
		DrawAction("Add hotkey for this option >", [this] {
			Hotkey hotkey = {
				optionToAdd.text,
				0,
				0,
				0,
				optionToAdd.type,
				optionToAdd.key,
				0,
				((optionToAdd.type == MenuOptionType::Action) ? optionToAdd.params : "")
			};
			auto hotkeySub = new SettingsEditAddHotkeySub(nullptr, hotkey, menuController);
			menuController->AddSubmenuToStack(hotkeySub);
		});
	// Option type
	DrawTextAction("Type >", OptionTypeToString(optionToAdd.type), [this]() {
		auto setTypeSub = new AddOptionSetTypeSub(menuController);
		setTypeSub->onTypeSet = [this](MenuOptionType type) {
			this->optionToAdd.type = type;
		};
		menuController->AddSubmenuToStack(setTypeSub);
	});
	// Option text
	DrawTextAction("Text", optionToAdd.text, [this]() {
		auto textInput = Game::GetInputWithKeyboard(optionToAdd.text);
		if (!textInput) return;
		optionToAdd.text = *textInput;
	});
	// Option key
	DrawTextAction(OptionTypeToString(optionToAdd.type) + " >", optionToAdd.key, [this]() {
		auto setKeySub = new AddOptionSetKeySub(optionToAdd.type, menuController);
		setKeySub->onKeySet = [this](std::string key) {
			this->optionToAdd.key = key;
			this->optionToAdd.params = json::array();
			UpdateParameters();
		};
		menuController->AddSubmenuToStack(setKeySub);
	});

	// Parameters
	for (int i = 0; i < parameters.size(); i++) {
		auto param = parameters[i];
		std::string value;
		switch (param.type) {
		case MenuOptionParameterType::String:
			value = optionToAdd.params[i].get<std::string>();
			break;
		case MenuOptionParameterType::Int:
			value = std::to_string(optionToAdd.params[i].get<int>());
			break;
		case MenuOptionParameterType::Float:
			value = std::to_string(optionToAdd.params[i].get<int>());
			break;
		}

		DrawTextAction(param.name, value, [this, param, i]() {
			switch (param.type) {
			case MenuOptionParameterType::String: {
				auto inputValue = Game::GetInputWithKeyboard(optionToAdd.params[i].get<std::string>());
				if (!inputValue) return;
				optionToAdd.params[i] = *inputValue;
				break;
			}
			case MenuOptionParameterType::Float: {
				auto inputValue = Game::GetInputWithKeyboard(std::to_string(optionToAdd.params[i].get<float>()));
				if (!inputValue) return;
				optionToAdd.params[i] = std::stof(*inputValue);
				break;
			}
			case MenuOptionParameterType::Int: {
				auto inputValue = Game::GetInputWithKeyboard(std::to_string(optionToAdd.params[i].get<int>()));
				if (!inputValue) return;
				optionToAdd.params[i] = std::stoi(*inputValue);
				break;
			}
			}
		});
	}

	// Add option
	DrawAction("Done", [this]() {
		if (optionToAdd.key == "") {
			Game::PrintSubtitle("Error: Key cannot be empty");
			return;
		}
		if (onAddOption)
			onAddOption(optionToAdd);
		menuController->GoToLastSub();
	});
}

void EditAddOptionSub::UpdateParameters()
{
	if (!(optionToAdd.key != "" && optionToAdd.type == MenuOptionType::Action &&
			ActionController::GetParameterForKey(optionToAdd.key).size() > 0))
		return;

	parameters = ActionController::GetParameterForKey(optionToAdd.key);

	for (auto&& param : parameters) {
		switch (param.type) {
		case MenuOptionParameterType::String:
			optionToAdd.params.push_back("");
			break;
		case MenuOptionParameterType::Int:
			optionToAdd.params.push_back(0);
			break;
		case MenuOptionParameterType::Float:
			optionToAdd.params.push_back(0.00f);
			break;
		}
	}
}

void EditAddOptionSub::RespondToControls()
{
	Submenu::RespondToControls();

	if (Controls::IsMenuControlPressed(MenuControl::MenuEditModeEnter)) {
		Game::PrintSubtitle("Edit mode is not available in this menu");
	}
}