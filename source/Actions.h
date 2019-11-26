#pragma once
#include "pch.h"

namespace Actions {
	void SetPlayerMaxHealth(json params);
	void StopPlayerPursuit(json params);
	void ClearPlayerBounty(json params);
	void ChangeModel(json params);
	void RestorePlayerStamina(json params);
	void SetHorseMaxHealth(json params);
	void SpawnHorse(json params);
	void SpawnVehicle(json params);
	void RepairVehicle(json params);
	void DeleteCurrentVehicle(json params);
	void GivePlayerAllWeapons(json params);
	void TeleportPlayerForward(json params);
	void TeleportPlayerToCoords(json params);
}