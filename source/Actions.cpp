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
#include "Actions.h"
#include "Routine.h"
#include "Player.h"
#include "Toggles.h"
#include "PedSpawner.h"
#include "WeaponManager.h"

#pragma region Player

void Actions::SetPlayerMaxHealth(json params)
{
	Player player = Player();
	player.ped.SetHealth(player.ped.MaxHealth());
	player.RestoreStamina(100.0);
	player.RestoreSpecialAbility();
	Game::PrintSubtitle("Player healed");
}

void Actions::StopPlayerPursuit(json params)
{
	Player().StopPursuit();
}

void Actions::ClearPlayerBounty(json params)
{
	Player().SetBounty(0);
}

void Actions::ChangeModel(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string model = params[0].get<std::string>();
	Hash modelHash = String::Hash(model);

	Player().SetModel(modelHash);
}

void Actions::ChangeFromInput(json params)
{
	auto model = Game::GetInputWithKeyboard();
	if (!model) return;
	ChangeModel({ model.value() });
}

void Actions::RestorePlayerStamina(json params)
{
	Player().RestoreStamina();
}

void Actions::AddCashFromKeyboard(json params)
{
	try {
		auto input = Game::GetInputWithKeyboard();
		if (!input)
			return;
		int cash = std::stoi(input.value());
		Player().AddCash(cash);
	}
	catch (const std::exception & e) {
		Game::PrintSubtitle("Failed to add cash");
	}
}

void Actions::PlayAnimOnPlayer(json params)
{
	if (!params.is_array() || !params[0].is_string() || !params[1].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string animDict = params[0];
	std::string anim = params[1];
	
	if (!Game::RequestAnimDict(animDict)) return;

	AI::TASK_PLAY_ANIM(Game::playerPedId, (char*)animDict.c_str(), (char*)anim.c_str(), 1.0f, 1.0f, -1, 2, 0, 0, 0, 0, "", 0);
}

void Actions::PlayScenarioOnPlayer(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string scenario = params[0];

	AI::_TASK_START_SCENARIO_IN_PLACE(Game::playerPedId, String::Hash(scenario), 0, false, false, false, 0.0f, false);
}

void Actions::StopPlayerScenario(json params)
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(Game::playerPedId, false, false);
}

#pragma endregion

#pragma region Peds

void Actions::SpawnPed(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string model = params[0].get<std::string>();

	PedSpawner::Spawn(model, model);
}

void Actions::SpawnPedFromInput(json params)
{
	auto model = Game::GetInputWithKeyboard();
	if (!model) return;

	PedSpawner::Spawn(model.value(), model.value());
}

void Actions::GiveSpawnedPedWeapon(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	if (!PedSpawner::IsAnyPedSelected()) {
		Game::PrintSubtitle("Error: No ped selected");
		return;
	}
	std::string weaponModel = params[0].get<std::string>();
	Hash weaponHash = String::Hash(weaponModel);

	PedSpawner::CurrentPed().GiveWeapon(weaponHash);
	WEAPON::SET_PED_AMMO(PedSpawner::CurrentPed().id, weaponHash, 9999);
}

void Actions::TeleportSpawnedPedToPlayer(json params)
{
	if (!PedSpawner::IsAnyPedSelected()) {
		Game::PrintSubtitle("Error: No ped selected");
		return;
	}

	auto teleportTo = Player().ped.GetOffsetInWorldCoords({0, 2.0f, 0});

	PedSpawner::CurrentPed().SetCoords(teleportTo);
}

void Actions::TeleportPlayerToSpawnedPed(json params)
{
	if (!PedSpawner::IsAnyPedSelected()) {
		Game::PrintSubtitle("Error: No ped selected");
		return;
	}

	auto teleportTo = PedSpawner::CurrentPed().GetOffsetInWorldCoords({ 0, 2.0f, 0 });

	Player().ped.SetCoords(teleportTo);
}

void Actions::DeleteSpawnedPed(json params)
{
	if (!PedSpawner::IsAnyPedSelected()) {
		Game::PrintSubtitle("Error: No ped selected");
		return;
	}

	PedSpawner::DeleteCurrent();
}

void Actions::GiveAllSpawnedPedsWeapon(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string weaponModel = params[0].get<std::string>();
	Hash weaponHash = String::Hash(weaponModel);

	for (auto&& ped : PedSpawner::peds) {
		ped->ped.GiveWeapon(weaponHash);
		ped->ped.SetAmmo(weaponHash, 9999);
	}
}

void Actions::TeleportAllSpawnedPedsToPlayer(json params)
{
	auto teleportTo = Player().ped.GetOffsetInWorldCoords({ 0, 2.0f, 0 });

	for (auto&& ped : PedSpawner::peds) {
		ped->ped.SetCoords(teleportTo);
	}
}

void Actions::DeleteAllSpawnedPeds(json params)
{
	for (int i = 0; i < PedSpawner::peds.size(); i++) {
		PedSpawner::peds[i]->ped.Delete();
	}
	PedSpawner::peds.clear();
	PedSpawner::currentPedIndex = -1;
}

void Actions::ReviveNearestHorse(json params)
{

}

#pragma endregion

#pragma region Horse

void Actions::SetHorseMaxHealth(json params)
{
	Ped horse = Player().ped.Mount();
	horse.SetHealth(horse.MaxHealth());
	horse.SetStamina(100.0);
	Game::PrintSubtitle("Horse healed");
}

void Actions::SpawnHorse(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string model = params[0].get<std::string>();
	Player player;
	Vector3 spawnPosition = player.ped.GetOffsetInWorldCoords({ 0.0, 2.0, 0.0 });
	float heading = player.ped.Heading() + 90.0f;

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(String::Hash(model))) {
		Game::PrintSubtitle("Invalid model");
		return;
	}

	Ped::Create(String::Hash(model), spawnPosition, heading);
}

void Actions::SpawnHorseFromInput(json params)
{
	auto horse = Game::GetInputWithKeyboard();
	if (!horse) return;
	SpawnHorse({ *horse });
}

#pragma endregion

#pragma region Vehicle

void Actions::SpawnVehicle(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string vehicleModel = params[0].get<std::string>();
	Hash vehicleHash = String::Hash(vehicleModel);
	if (!STREAMING::IS_MODEL_IN_CDIMAGE(vehicleHash)) {
		Game::PrintSubtitle("Error: Invalid model");
		return;
	}
	Player player = Player();
	Vector3 spawnPosition = player.ped.Position();
	float vehicleSpawnHeading = player.ped.Heading();

	if (!(Toggles::spawnInsideVehicle)) {
		spawnPosition = player.ped.GetOffsetInWorldCoords({0.0, 2.0, 0.0});
		vehicleSpawnHeading += 90;
	}

	if (player.ped.IsInVehicle()) {
		auto vehicle = player.ped.CurrentVehicle();
		vehicle.Delete();
	}

	auto spawnedVehicle = Vehicle::Create(vehicleHash, spawnPosition, vehicleSpawnHeading);

	if (Toggles::spawnInsideVehicle) {
		player.ped.SetIntoVehicle(spawnedVehicle.id);
	}
}

void Actions::SpawnVehicleFromInput(json params)
{
	auto vehicle = Game::GetInputWithKeyboard();
	if (!vehicle) return;
	SpawnVehicle({ *vehicle });
}

void Actions::TeleportIntoClosestVehicle(json params)
{
	Player().ped.SetIntoClosestVehicle();
}

void Actions::RepairVehicle(json params)
{
	auto currentVehicle = Player().ped.CurrentVehicle();
	if (currentVehicle.Exists()) {
		currentVehicle.Repair();
	}
}

void Actions::BoostVehicle(json params)
{
	auto currentVehicle = Player().ped.CurrentVehicle();
	if (currentVehicle.Exists()) {
		currentVehicle.SetForwardSpeed(16.66);
	}
}

void Actions::DeleteCurrentVehicle(json params)
{
	Vehicle currentVehicle = Player().ped.CurrentVehicle();
	if (currentVehicle.Exists()) {
		currentVehicle.Delete();
		Game::PrintSubtitle("Deleted");
	}
}

void Actions::RepairEngine(json params)
{
	Player player;
	if (!player.ped.IsInVehicle()) return;

	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(player.ped.CurrentVehicle().id, 1250.0f);
	VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(player.ped.CurrentVehicle().id, 1250.0f);
}

#pragma endregion

#pragma region Weapons

void Actions::GivePlayerAllWeapons(json params)
{
	Player player;
	std::string weaponModels[] = { "WEAPON_KIT_CAMERA", "WEAPON_MOONSHINEJUG", "WEAPON_MELEE_LANTERN_ELECTRIC", "WEAPON_MELEE_TORCH", "WEAPON_MELEE_BROKEN_SWORD", "WEAPON_FISHINGROD",
							"WEAPON_MELEE_HATCHET", "WEAPON_MELEE_CLEAVER", "WEAPON_MELEE_ANCIENT_HATCHET", "WEAPON_MELEE_HATCHET_VIKING", "WEAPON_MELEE_HATCHET_HEWING",
							"WEAPON_MELEE_HATCHET_DOUBLE_BIT", "WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED", "WEAPON_MELEE_HATCHET_HUNTER", "WEAPON_MELEE_HATCHET_HUNTER_RUSTED",
							"WEAPON_MELEE_KNIFE_JOHN", "WEAPON_MELEE_KNIFE", "WEAPON_MELEE_KNIFE_JAWBONE", "WEAPON_THROWN_THROWING_KNIVES", "WEAPON_MELEE_KNIFE_MINER", "WEAPON_MELEE_KNIFE_CIVIL_WAR",
							"WEAPON_MELEE_KNIFE_BEAR", "WEAPON_MELEE_KNIFE_VAMPIRE", "WEAPON_LASSO", "WEAPON_MELEE_MACHETE", "WEAPON_THROWN_TOMAHAWK", "WEAPON_THROWN_TOMAHAWK_ANCIENT",
							"WEAPON_PISTOL_M1899", "WEAPON_PISTOL_MAUSER", "WEAPON_PISTOL_MAUSER_DRUNK", "WEAPON_PISTOL_SEMIAUTO", "WEAPON_PISTOL_VOLCANIC", "WEAPON_REPEATER_CARBINE",
							"WEAPON_REPEATER_EVANS", "WEAPON_REPEATER_HENRY", "WEAPON_RIFLE_VARMINT", "WEAPON_REPEATER_WINCHESTER", "WEAPON_REVOLVER_CATTLEMAN", "WEAPON_REVOLVER_CATTLEMAN_JOHN",
							"WEAPON_REVOLVER_CATTLEMAN_MEXICAN", "WEAPON_REVOLVER_CATTLEMAN_PIG", "WEAPON_REVOLVER_DOUBLEACTION", "WEAPON_REVOLVER_DOUBLEACTION_EXOTIC", "WEAPON_REVOLVER_DOUBLEACTION_GAMBLER",
							"WEAPON_REVOLVER_DOUBLEACTION_MICAH", "WEAPON_REVOLVER_LEMAT", "WEAPON_REVOLVER_SCHOFIELD", "WEAPON_REVOLVER_SCHOFIELD_GOLDEN", "WEAPON_REVOLVER_SCHOFIELD_CALLOWAY",
							"WEAPON_RIFLE_BOLTACTION", "WEAPON_SNIPERRIFLE_CARCANO", "WEAPON_SNIPERRIFLE_ROLLINGBLOCK", "WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC", "WEAPON_RIFLE_SPRINGFIELD",
							"WEAPON_SHOTGUN_DOUBLEBARREL", "WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC", "WEAPON_SHOTGUN_PUMP", "WEAPON_SHOTGUN_REPEATING", "WEAPON_SHOTGUN_SAWEDOFF", "WEAPON_SHOTGUN_SEMIAUTO",
							"WEAPON_BOW", "WEAPON_THROWN_DYNAMITE", "WEAPON_THROWN_MOLOTOV" };

	std::string ammoTypes[] = { "AMMO_PISTOL", "AMMO_PISTOL_EXPRESS", "AMMO_PISTOL_HIGH_VELOCITY", "AMMO_PISTOL_SPLIT_POINT", "AMMO_PISTOL_EXPRESS_EXPLOSIVE", "AMMO_REVOLVER",
							"AMMO_REVOLVER_EXPRESS", "AMMO_REVOLVER_HIGH_VELOCITY", "AMMO_REVOLVER_SPLIT_POINT", "AMMO_REVOLVER_EXPRESS_EXPLOSIVE",
							"AMMO_REPEATER", "AMMO_REPEATER_EXPRESS", "AMMO_REPEATER_HIGH_VELOCITY", "AMMO_REPEATER_SPLIT_POINT", "AMMO_REPEATER_EXPRESS_EXPLOSIVE", "AMMO_RIFLE",
							"AMMO_RIFLE_EXPRESS", "AMMO_RIFLE_HIGH_VELOCITY", "AMMO_RIFLE_SPLIT_POINT", "AMMO_RIFLE_EXPRESS_EXPLOSIVE", "AMMO_RIFLE_VARMINT", "AMMO_SHOTGUN", "AMMO_SHOTGUN_SLUG",
							"AMMO_SHOTGUN_BUCKSHOT_INCENDIARY", "AMMO_SHOTGUN_EXPRESS_EXPLOSIVE", "AMMO_ARROW", "AMMO_ARROW_SMALL_GAME", "AMMO_ARROW_IMPROVED", "AMMO_ARROW_FIRE", "AMMO_ARROW_POISON", 
							"AMMO_ARROW_DYNAMITE", "AMMO_THROWING_KNIVES", "AMMO_THROWING_KNIVES_IMPROVED", "AMMO_THROWING_KNIVES_POISON" };

	for (std::string& ammoType : ammoTypes) {
		Hash ammoTypeHash = String::Hash(ammoType);

		player.ped.SetAmmoByType(ammoTypeHash, 9999);
	}

	for (std::string& weapon : weaponModels) {
		player.ped.GiveWeapon(String::Hash(weapon));
		player.ped.SetAmmo(String::Hash(weapon), 9999);
	}
}

void Actions::RemoveAllWeaponsFromPlayer(json params)
{
	Player().ped.RemoveAllWeapons();
}

void Actions::GivePlayerWeapon(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string weaponModel = params[0].get<std::string>();
	Hash weaponHash = String::Hash(weaponModel);
	
	Player().ped.GiveWeapon(weaponHash);
	WEAPON::SET_PED_AMMO(Game::playerPedId, weaponHash, 9999);
}

void Actions::EquipSelectedWeapon(json params)
{
	if (WeaponManager::currentWeapon.model == "") {
		Game::PrintSubtitle("No weapon selected");
		return;
	}
	Hash weaponHash = String::Hash(WeaponManager::currentWeapon.model);
	Player player;

	if (!player.ped.HasWeapon(weaponHash))
		player.ped.GiveWeapon(weaponHash);
	else
		player.ped.SetCurrentWeapon(weaponHash);
	player.ped.SetAmmo(weaponHash, 9999);
}

void Actions::RemoveSelectedWeapon(json params)
{
	if (WeaponManager::currentWeapon.model == "") {
		Game::PrintSubtitle("No weapon selected");
		return;
	}
	Hash weaponHash = String::Hash(WeaponManager::currentWeapon.model);

	Player().ped.RemoveWeapon(weaponHash);
}

void Actions::FillAmmoInSelectedWeapon(json params)
{
	if (WeaponManager::currentWeapon.model == "") {
		Game::PrintSubtitle("No weapon selected");
		return;
	}
	Hash weaponHash = String::Hash(WeaponManager::currentWeapon.model);

	Player().ped.SetAmmo(weaponHash, 9999);
}

void Actions::GivePlayerMaxAmmo(json params)
{
	Hash currentWeapon;
	if (WEAPON::GET_CURRENT_PED_WEAPON(Game::playerPedId, &currentWeapon, 0, 0, 0) && WEAPON::IS_WEAPON_VALID(currentWeapon))
	{
		int maxAmmo;
		if (WEAPON::GET_MAX_AMMO(Game::playerPedId, &maxAmmo, currentWeapon))
			WEAPON::SET_PED_AMMO_BY_TYPE(Game::playerPedId, currentWeapon, maxAmmo);
		int maxAmmoInClip = WEAPON::GET_MAX_AMMO_IN_CLIP(Game::playerPedId, currentWeapon, 1);
		if (maxAmmoInClip > 0)
			WEAPON::SET_AMMO_IN_CLIP(Game::playerPedId, currentWeapon, maxAmmoInClip);
	}
}

void Actions::GivePlayerMAaxAmmoOfType(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string ammoType = params[0];
	Hash ammoTypeHash = String::Hash(ammoType);

	if (ammoType == "ammo_repeater_explosive_nearc") {
		ammoTypeHash = 0x9C8B6796;
	}

	Player().ped.SetAmmoByType(ammoTypeHash, 9999);

}

#pragma endregion

#pragma region Weather

void Actions::SetWeather(json params)
{
	if (!params.is_array() || !params[0].is_string()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	std::string weather = params[0].get<std::string>();
	Game::SetWeather(String::Hash(weather));
}

#pragma endregion 

#pragma region Time

void Actions::AddToClockTime(json params)
{
	if (!params.is_array() || !params[0].is_number_integer() || !params[1].is_number_integer() || !params[2].is_number_integer()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	int hours = params[0].get<int>();
	int minutes = params[0].get<int>();
	int seconds = params[0].get<int>();
	TIME::ADD_TO_CLOCK_TIME(hours, minutes, seconds);
}

#pragma endregion 

#pragma region Teleport

void Actions::TeleportPlayerForward(json params)
{
	auto player = Player();
	auto teleportToCoords = player.ped.GetOffsetInWorldCoords({0, 4.0f, 0});
	player.ped.SetCoords(teleportToCoords);
}

void Actions::TeleportPlayerToWaypoint(json params)
{
	if (!RADAR::IS_WAYPOINT_ACTIVE()) {
		Game::PrintSubtitle("No waypoint set");
		return;
	}

	Player player;
	Vector3 waypointPosition = RADAR::GET_WAYPOINT_COORDS_3D();

	if (!GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(waypointPosition.x, waypointPosition.y, 100.0, &waypointPosition.z, FALSE))
	{
		static float groundCheckHeight[] = {
			100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
			450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
		};

		for (auto&& height : groundCheckHeight) {
			auto coordsToSet = waypointPosition;
			coordsToSet.z = height;

			if (player.ped.IsOnMount())
				player.ped.Mount().SetCoordsNoOffset(coordsToSet);
			else if (player.ped.IsInVehicle())
				player.ped.CurrentVehicle().SetCoordsNoOffset(coordsToSet);
			else
				player.ped.SetCoordsNoOffset(coordsToSet);

			WAIT(100);
			if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(waypointPosition.x, waypointPosition.y, height, &waypointPosition.z, false))
			{
				waypointPosition.z += 3.0;
				break;
			}
		}
	}

	if (player.ped.IsOnMount())
		player.ped.Mount().SetCoordsNoOffset(waypointPosition);
	else if (player.ped.IsInVehicle())
		player.ped.CurrentVehicle().SetCoordsNoOffset(waypointPosition);
	else
		player.ped.SetCoordsNoOffset(waypointPosition);
}

void Actions::TeleportPlayerToCoords(json params)
{
	if (!params.is_array() || !params[0].is_number_float() || !params[1].is_number_float() || !params[2].is_number_float()) {
		Game::PrintSubtitle("Error: Invalid parameters");
		return;
	}
	Player player;
	Vector3 teleportToCoords = { params[0], params[1], params[2] };

	if (player.ped.IsOnMount())
		player.ped.Mount().SetCoords(teleportToCoords);
	else if (player.ped.IsInVehicle())
		player.ped.CurrentVehicle().SetCoords(teleportToCoords);
	else 
		player.ped.SetCoords(teleportToCoords);
}

#pragma endregion

#pragma region Misc

void Actions::RevealFullMap(json params)
{
	RADAR::_SET_MINIMAP_REVEALED(true);
	RADAR::REVEAL_MAP(0);
}

void Actions::KillEveryoneNearby(json params)
{
	auto pos = Player().ped.Position();
	FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 35, 100.0f, true, false, true);
	FIRE::ADD_EXPLOSION(pos.x + 2.0f, pos.y, pos.z, 35, 100.0f, true, false, true);
	FIRE::ADD_EXPLOSION(pos.x - 2.0f, pos.y, pos.z, 35, 100.0f, true, false, true);
	FIRE::ADD_EXPLOSION(pos.x, pos.y + 2.0f, pos.z, 35, 100.0f, true, false, true);
	FIRE::ADD_EXPLOSION(pos.x, pos.y - 2.0f, pos.z, 35, 100.0f, true, false, true);
}

void Actions::ClearWorldOfPeds(json params)
{
	const int arraySize = 1000;
	PedId peds[arraySize];

	int count = worldGetAllPeds(peds, arraySize);
	for (int i = 0; i < count; i++) {
		int pedId = peds[i];
		auto ped = Ped(pedId);
		if (!ped.Exists() || !ped.IsPlayer())
			return;

		ped.Delete();
	}
}

void Actions::TestAction(json params)
{
	//Hash currentWeapon;
	//WEAPON::GET_CURRENT_PED_WEAPON(Game::playerPedId, &currentWeapon, 0, 0, 0);
	//Game::PrintSubtitle(std::to_string(WEAPON::_0x5C2EA6C44F515F34(currentWeapon)));
	WEAPON::SET_PED_AMMO_BY_TYPE(Game::playerPedId, 0x9C8B6796, 150);
}

#pragma endregion
