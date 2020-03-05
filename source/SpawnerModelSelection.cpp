#include "pch.h"
#include "SpawnerModelSelection.h"
#include "Spawner.h"
#include "VehicleSelection.h"
#include "PedSelection.h"

#pragma region Objects

SpawnerObjectsSub::SpawnerObjectsSub(MenuController *menuController)
	: Submenu(menuController)
{
	Spawner::Spawner::SetEntityForSpawner("p_door_val_bankvault", EntityType::Object);
}

void SpawnerObjectsSub::Draw()
{
	Submenu::Draw();

	DrawTitle("Objects");
	DrawObject("Test 1", "");
	DrawObject("Test 2", "");
}

void SpawnerObjectsSub::DrawObject(std::string title, std::string model)
{
	DrawAction(title, [] {
		Spawner::Spawner::SpawnSelectedEntity();
	});
}

void SpawnerObjectsSub::SelectionDidChange(int to, int from)
{
	Submenu::SelectionDidChange(to, from);

	std::string selectedModel;

	switch (to) {
	case 0:
		selectedModel = "p_door_val_bankvault";
		break;
	case 1:
		selectedModel = "P_CHAIR20X";
		break;
	}
	Spawner::Spawner::SetEntityForSpawner(selectedModel, EntityType::Object);
}

#pragma endregion

#pragma region Categories

SpawnerSelectCatSub::SpawnerSelectCatSub(MenuController* menuController)
	: Submenu(menuController)
{
	if (!Spawner::Spawner::IsFreeCamEnabled())
		Spawner::Spawner::SetSpawnOnlyModeEnabled(true);
}

SpawnerSelectCatSub::~SpawnerSelectCatSub()
{
	if (!Spawner::Spawner::IsFreeCamEnabled())
		Spawner::Spawner::SetSpawnOnlyModeEnabled(false);
}

void SpawnerSelectCatSub::SubDidAppear()
{
	Spawner::Spawner::DisableSpawnerMode();
}

void SpawnerSelectCatSub::Draw()
{
	Submenu::Draw();

	DrawTitle("Spawn");
	DrawSubAction("Peds", [this] {
		auto pedsSub = new PedCatSelectionSub(menuController, PedSelectionMode::SpawnerMode, [] (PedData pedData) {});
		menuController->AddSubmenuToStack(pedsSub);
	});
	DrawSubAction("Vehicles", [this] {
		auto vehicleSelectionSub = new VehicleSelectionSub(menuController,
			VehicleSelectionMode::SpawnerMode, [this] (VehicleData selected) {});
		menuController->AddSubmenuToStack(vehicleSelectionSub);
	});
	DrawSubAction("Objects", [this] {
		auto objectsSub = new SpawnerObjectsSub(menuController);
		menuController->AddSubmenuToStack(objectsSub);
	});
}

#pragma endregion