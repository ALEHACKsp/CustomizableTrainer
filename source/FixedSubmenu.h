/*
* Customizable Trainer
* Copyright (C) 2020  Gu�mundur �li
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*/

#pragma once
#include "pch.h"
#include "Submenu.h"

class FixedSubmenu :
	public Submenu
{
public:
	FixedSubmenu(MenuController* menuController);

protected:
	void DrawSub(string text, string subKey);
	void DrawTextAction(string text, string value, std::function<void()> onPress);

	virtual void SubWillDraw() override;
	virtual void SelectionDidChange(int to, int from) override;

	virtual void RespondToControls() override;
};
