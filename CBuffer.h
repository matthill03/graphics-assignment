#pragma once
#include "DirectXCore.h"

typedef struct  CBuffer
{
	Matrix		WorldViewProjection;
	Matrix		World;
	Vector4		AmbientLightColour;
	Vector4		DirectionalLightColour;
	Vector4		DirectionalLightVector;
	Vector3		EyePosition;
	float		SpecularPower{ 0.0f };
	Vector4		SpecularColour;
	Vector4		SpecularDirection;


};
