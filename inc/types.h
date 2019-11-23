/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once

#include <windows.h>
#include "pch.h"
#include "enums.h"
#include "json.hpp"

using json = nlohmann::json;

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;
typedef std::string string;

#define ALIGN8 __declspec(align(8))

struct Vector3
{
	ALIGN8 float x;
	ALIGN8 float y;
	ALIGN8 float z;
};

struct Vector2
{
	ALIGN8 float x;
	ALIGN8 float y;
};

struct Color
{
	int r;
	int g;
	int b;
	int a;
};

static_assert(sizeof(Vector3) == 24, "");

// MARK: Custom

struct MenuOption
{
	MenuOptionType type;
	string text;
	string key;
	json params;
};

struct SubmenuData
{
	string title;
	string key;
	std::vector<MenuOption> options;
};