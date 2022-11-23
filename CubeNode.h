#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"

class CubeNode : public SceneNode
{
public:
	CubeNode(wstring name, Color cube_colour) : SceneNode(name) { _ambientLightColour = cube_colour; };
	~CubeNode(void) {};

protected:
	Vector4 _ambientLightColour;
};

