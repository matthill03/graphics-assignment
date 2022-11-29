#pragma once
#include "DirectXFramework.h"

class DirectXApp : public DirectXFramework
{
public:
	void CreateSceneGraph();
	void UpdateSceneGraph();

private:
	int				_rotationAngle {0};
	float			_leftArmRotation{ 0.0f };
	float			_rightArmRotation   { 0.0f };
	bool			_swingDirection{ false };

};

