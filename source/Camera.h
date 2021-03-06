/*
* Menyoo PC - Grand Theft Auto V single-player trainer mod
* Copyright (C) 2019  MAFINS
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*/
/*
* ALTERED SOURCE
* Customizable Trainer
* Copyright (C) 2020 Gu�mundur �li
*/

#pragma once

class Camera
{
public:
	static Vector3 Position(Cam cam);
	static Vector3 Rotation(Cam cam);
	static bool WorldToScreenRel(Cam cam, const Vector3& worldCoords, Vector2& screenCoords);
	static Vector3 ScreenToWorld(Cam cam, const Vector2& screenCoord);
	static Vector3 DirectionFromScreenCentre(Cam cam);
};

