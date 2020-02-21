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
#include "Spawner.h"

class SpawnerManageEntity
	: public Submenu
{
public:
	SpawnerManageEntity(MenuController* menuController, EntityType type, std::shared_ptr<Spawner::DatabaseItem> dbItem);

	void Draw() override;

	bool IsPed();
	bool IsVehicle();
	bool IsObject();

	Ped GetEntityAsPed();
	Vehicle GetEntityAsVehicle();

	Object GetEntityAsObject();

	Entity GetEntity();

private:
	Entity entity;
	EntityType type;
	std::shared_ptr<Spawner::DatabaseItem> dbItem;
};

