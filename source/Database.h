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

class Database
{
public:
	Database();

	std::vector<Ped> peds;
	std::vector<Vehicle> vehicles;
	std::vector<Object> objects;
};

