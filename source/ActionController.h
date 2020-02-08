#pragma once
#include "pch.h"

class ActionController
{
public:
	static void RegisterAction(string key, std::function<void(json params)> action);
	static void RegisterActions();
	static void RunActionForKey(string key, json params);
	static bool ActionExistsForKey(string key);
	static std::function<void (json params)> GetActionForKey(string key);
	static std::vector<string> Keys();
	static std::vector<MenuOptionParameter> GetParameterForKey(string key);
private:
	static inline std::map<string, std::function<void(json params)>> actions;
};

