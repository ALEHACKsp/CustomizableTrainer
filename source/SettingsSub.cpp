#include "pch.h"
#include "SettingsSub.h"

SettingsSub::SettingsSub(MenuController* menuController) : FixedSubmenu(menuController)
{
}

void SettingsSub::Draw()
{
	FixedSubmenu::Draw();

	DrawTitle("Settings");
	DrawSub("Color scheme", "builtin_sub_settingsColorScheme");
	DrawSub("Controls", "builtin_sub_settingsControls");
	DrawSub("Hotkeys", "builtin_sub_settingsHotkeys");
}

int SettingsSub::OptionCount()
{
	return 3;
}