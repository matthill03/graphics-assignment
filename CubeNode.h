#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"
#include "CBuffer.h"

#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"

// Creating a derived class of SceneNode called CubeNode
class CubeNode : public SceneNode
{
// Setting public methods and variables
public:
	/*Defining Constructors*/
	// Default constructor
	CubeNode() : SceneNode(L"Root") {};
	// Constructor to specify name and colour of cube
	CubeNode(wstring name, Vector4 cube_colour) : SceneNode(name) { _ambientLightColour = cube_colour;};
	// Destructor
	~CubeNode(void) {};

private:
	// Core methods, to override base methods
	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

};

