#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"

#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"

class CubeNode : public SceneNode
{
public:
	CubeNode() : SceneNode(L"Root") {};
	CubeNode(wstring name, Color cube_colour) : SceneNode(name) { _ambientLightColour = cube_colour; };
	~CubeNode(void) {};

private:

	Vertex vertices[24] =
	{
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector3(0, 0, 0) },    // side 1
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector3(0, 0, 0) },
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector3(0, 0, 0) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0, 0, 0) },    // side 2
		{ Vector3(-1.0f, +1.0f, -1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, -1.0f, -1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, +1.0f, -1.0f), Vector3(0, 0, 0) },

		{ Vector3(-1.0f, +1.0f, -1.0f), Vector3(0, 0, 0) },    // side 3
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, +1.0f, -1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector3(0, 0, 0) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0, 0, 0) },    // side 4
		{ Vector3(+1.0f, -1.0f, -1.0f), Vector3(0, 0, 0) },
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector3(0, 0, 0) },

		{ Vector3(+1.0f, -1.0f, -1.0f), Vector3(0, 0, 0) },    // side 5
		{ Vector3(+1.0f, +1.0f, -1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, -1.0f, +1.0f), Vector3(0, 0, 0) },
		{ Vector3(+1.0f, +1.0f, +1.0f), Vector3(0, 0, 0) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0, 0, 0) },    // side 6
		{ Vector3(-1.0f, -1.0f, +1.0f), Vector3(0, 0, 0) },
		{ Vector3(-1.0f, +1.0f, -1.0f), Vector3(0, 0, 0) },
		{ Vector3(-1.0f, +1.0f, +1.0f), Vector3(0, 0, 0) }
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

	UINT polygonCount[ARRAYSIZE(vertices)] {0};

	virtual bool Initialise();

	virtual void Render();
	virtual void Shutdown();
	void Update();
	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRasteriserState();
	void CalculateVertexNormals();
	Vector3 CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3);


};

