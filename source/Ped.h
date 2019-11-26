#pragma once
#include "pch.h"
#include "Vehicle.h"

class Ped
{
public:
	Ped(int pedId);
	void SetHealth(int health);
	void SetVisible(bool visible);
	void SetIntoVehicle(VehicleId vehicle, int seat = -1);
	void SetCoords(Vector3 coords);
	void SetStamina(float stamina);
	void SetInvincible(bool invincible);
	void ClearWetness();
	void GiveWeapon(Hash model);
	// MARK: Booleans
	bool IsInVehicle();
	bool IsOnMount();
	// MARK: Getters
	PedId GetPedId();
	int GetMaxHealth();
	Vector3 GetPosition();
	Vector3 GetOffsetInWorldCoords(Vector3 offset);
	float GetHeading();
	Vehicle GetCurrentVehicle();
	Ped GetMount();
	// MARK: Static functions
	static Ped Spawn(Hash model, Vector3 position, float heading = 0);
private:
	int pedId;
};
