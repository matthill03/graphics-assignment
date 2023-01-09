#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"
#include "WICTextureLoader.h"

#define TextureShaderFileName		L"textureShader.hlsl"
#define VertexShaderName			"VS"
#define PixelShaderName				"PS"

class TexturedCubeNode : public SceneNode
{
public:
	/* Defining Constructors */
	TexturedCubeNode() : SceneNode(L"Root") {};
	TexturedCubeNode(wstring name, Vector4 cube_colour) : SceneNode(name) { _ambientLightColour = cube_colour; };
	TexturedCubeNode(wstring name, Vector4 cube_colour, wstring texture_file) : SceneNode(name) { _ambientLightColour = cube_colour; _textureName = texture_file; };
	~TexturedCubeNode(void) {};

private:
	// Member variables
	wstring								_textureName;
	ComPtr<ID3D11ShaderResourceView>	_texture;

	// Private overide methods
	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

	// Private methods
	void BuildTexture();
};

