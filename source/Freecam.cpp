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
#include "Freecam.h"
#include "Controls.h"
#include "Camera.h"
#include "CameraUtils.h"
#include "keyboard.h"

namespace Spawner {

	FreeCam::FreeCam()
	{
		Player player;
		auto playerPos = player.ped.Position();
		cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
		nextPositionOffset = { 0, 0, 0 };
		nextRotationOffset = { 0, 0, 0 };
		CAM::SET_CAM_ACTIVE(cam, true);
		CAM::RENDER_SCRIPT_CAMS(true, false, 3000, true, false, 0);
		CAM::SET_CAM_COORD(cam, playerPos.x, playerPos.y, playerPos.z + 1.0f);
	}

	FreeCam::~FreeCam()
	{
		CAM::SET_CAM_ACTIVE(cam, false);
		CAM::DESTROY_CAM(cam, false);
	}

	// Controls

	void FreeCam::RespondToMoveControls()
	{
		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_Y) > 0.0f) // Down
			nextPositionOffset.y -= CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_Y) / 2;

		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_Y) < 0.0f) // Up
			nextPositionOffset.y += CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_Y) / 2 * -1;

		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_X) > 0.0f) // Right
			nextPositionOffset.x += CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_X) / 2;

		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_X) < 0.0f) // Left
			nextPositionOffset.x -= CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, XboxControl::INPUT_FRONTEND_AXIS_X) / 2 * -1;
	}

	void FreeCam::RespondToMoveKeyboardControls()
	{
		if (IsKeyDown(0x53)) // Down
			nextPositionOffset.y -= 1.0f;

		if (IsKeyDown(0x57)) // Up
			nextPositionOffset.y += 1.0f;

		if (IsKeyDown(0x44)) // Right
			nextPositionOffset.x += 1.0f;

		if (IsKeyDown(0x41)) // Left
			nextPositionOffset.x -= 1.0f;
	}

	void FreeCam::RespondToRotateControls(float sensitivity)
	{
		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_ud")) > 0.0f) // Down
			nextRotationOffset.x -= CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_ud")) * sensitivity;

		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_ud")) < 0.0f) // Up
			nextRotationOffset.x += CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_ud")) * -1 * sensitivity;

		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_lr")) > 0.0f) // Right
			nextRotationOffset.z += CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_lr")) * -1 * sensitivity;

		if (CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_lr")) < 0.0f) // Left
			nextRotationOffset.z -= CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, String::Hash("input_look_lr")) * sensitivity;
	}

	void FreeCam::RespondToControls()
	{
		CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(0);

		if (Controls::IsUsingController()) {
			RespondToMoveControls();
			RespondToRotateControls(3);
		}
		else {
			RespondToMoveKeyboardControls();
			RespondToRotateControls(6);
		}
	}

	//

	void FreeCam::ResetValues()
	{
		nextRotationOffset = { 0, 0, 0 };
		nextPositionOffset = { 0, 0, 0 };
	}

	void FreeCam::UpdatePosition()
	{
		if (nextPositionOffset.x + nextPositionOffset.y + nextPositionOffset.z == 0) // If they are all zero
			return;

		auto&& nextPosition = CameraUtils::GetOffsetFromCameraInWorldCoords(cam, nextPositionOffset);
		CAM::SET_CAM_COORD(cam, nextPosition.x, nextPosition.y, nextPosition.z);
	}

	void FreeCam::UpdateRotation()
	{
		if (nextRotationOffset.x + nextRotationOffset.y + nextRotationOffset.z == 0) // If they are all zero
			return;

		auto&& currentRotation = CAM::GET_CAM_ROT(cam, 2);
		CAM::SET_CAM_ROT(cam, currentRotation.x + nextRotationOffset.x, currentRotation.y + nextRotationOffset.y, currentRotation.z + nextRotationOffset.z, 2);
	}

	void FreeCam::Tick()
	{
		ResetValues();
		RespondToControls();
		UpdatePosition();
		UpdateRotation();
	}

}