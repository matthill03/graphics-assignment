#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"

#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"

class TeapotNode : public SceneNode
{
public:
	/*Defining Constructors*/
	// Default constructor
	TeapotNode() : SceneNode(L"Root") {};
	// Constructor to specify name and colour of cube
	TeapotNode(wstring name, Vector4 cube_colour) : SceneNode(name) { _ambientLightColour = cube_colour; };
	// Destructor
	~TeapotNode(void) {};

private:
    // Core methods, to override base methods
    virtual bool Initialise();
    virtual void Render();
    virtual void Shutdown();
};

