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

namespace Spawner {

#pragma region Database

	class Database {
	public:
		void AddObject(Object object);
		void AddVehicle(Vehicle vehicle);
		void AddPed(Ped ped);

		void SpawnObject(Hash model, Vector3 position, float rotation);
		void SpawnVehicle(Hash model, Vector3 position, float rotation);
		void SpawnPed(Hash model, Vector3 position, float rotation);
	private:
		std::vector<Object> objects;
		std::vector<Vehicle> vehicles;
		std::vector<Ped> peds;
	};

#pragma endregion

#pragma region Camera

	class Camera {
	public:
		Camera();
		~Camera();

		void RespondToMoveControls();
		void RespondToRotateControls();
		void RespondToControls();

		void ResetValues();
		void UpdatePosition();
		void UpdateRotation();
		void Tick();
	private:
		Vector3 nextPositionOffset;
		Vector3 nextRotationOffset;
		Cam cam;
	};

#pragma endregion

#pragma region Spawner

	class Spawner {
	public:
		static void SetFreeCamEnabled(bool enabled);
		static void ShowSelectedObjectOnGround();
		static void Tick();

		static std::unique_ptr<Database> database;
		static inline std::optional<std::unique_ptr<Camera>> camera = std::nullopt;
	private:
		static inline bool isFreeCamEnabled = false;
	};

#pragma endregion
}
