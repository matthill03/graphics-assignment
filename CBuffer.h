#pragma once
#include "DirectXCore.h"

// Central CBuffer struct so each node does not have a seperate one.
struct  CBuffer
{
	Matrix		WorldViewProjection;
	Matrix		World;
	Vector4		AmbientLightColour;
	Vector4		DirectionalLightColour;
	Vector4		DirectionalLightVector;
	Vector3		EyePosition;
	float		SpecularPower{ 0.0f };
	Vector4		SpecularColour;
};
