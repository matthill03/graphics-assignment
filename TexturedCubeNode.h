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
	TexturedCubeNode() : SceneNode(L"Root") {};
	TexturedCubeNode(wstring name, Vector4 cube_colour) : SceneNode(name) { _ambientLightColour = cube_colour; };
	TexturedCubeNode(wstring name, Vector4 cube_colour, wstring texture_file) : SceneNode(name) { _ambientLightColour = cube_colour; _textureName = texture_file; };
	~TexturedCubeNode(void) {};

private:
	wstring								_textureName;
	ComPtr<ID3D11ShaderResourceView>	_texture;

	typedef struct {
		Vector3 Position;
		Vector3 Normal;
		Vector2 Texture;
	}Vertex;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE" , 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	Vertex vertices[24] =
	{
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)},    // side 1
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)},
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)},
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)},

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },    // side 2
		{ Vector3(-1.0f, +1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(+1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(+1.0f, +1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },

		{ Vector3(-1.0f, +1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },    // side 3
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(+1.0f, +1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },    // side 4
		{ Vector3(+1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },

		{ Vector3(+1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },    // side 5
		{ Vector3(+1.0f, +1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },    // side 6
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-1.0f, +1.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) }
	};

	UINT indices[36] = {
				0, 1, 2,       // side 1
				2, 1, 3,
				4, 5, 6,       // side 2
				6, 5, 7,
				8, 9, 10,      // side 3
				10, 9, 11,
				12, 13, 14,    // side 4
				14, 13, 15,
				16, 17, 18,    // side 5
				18, 17, 19,
				20, 21, 22,    // side 6
				22, 21, 23,
	};

	float polygonCount[ARRAYSIZE(vertices)]{ 0 };

	virtual bool Initialise();

	virtual void Render();
	virtual void Shutdown();
	void Update();

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRasteriserState();
	void BuildTexture();

	void CalculateVertexNormals();
	Vector3 CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3);

};

