#pragma once
#include "Core.h"
#include "HelperFunctions.h"
#include "CBuffer.h"

#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"

class CubeRenderer
{
public:
	CubeRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Matrix viewTransformation, Matrix projectionTransformation) { _device = device; _deviceContext = deviceContext; _viewTransformation = viewTransformation; _projectionTransformation = projectionTransformation; };
	
	void Render(CBuffer constantBuffer);
	bool Initialise();

private:
	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11DeviceContext>		_deviceContext;
	Matrix							_viewTransformation;
	Matrix							_projectionTransformation;

	ComPtr<ID3D11Buffer>			_vertexBuffer;
	ComPtr<ID3D11Buffer>			_indexBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;
	ComPtr<ID3D11InputLayout>		_layout;
	ComPtr<ID3D11Buffer>			_constantBuffer;

	ComPtr<ID3D11RasterizerState>   _rasteriserState;

	// Methods for passing data from CPU to GPU, also building data to be passes

	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRasteriserState();
	void CalculateVertexNormals();
	Vector3 CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3);

	// Defines the Vertex struct for CubeNode, with a Position and Normal
	typedef struct {
		Vector3 Position;
		Vector3 Normal;
	}Vertex;

	// Defines the Vertex Description for when vertices is passed into shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Defines vertices array for cube
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

	// Defines indicies array for cube
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

	// Defines polygonCount for when working out normals of cube faces
	UINT polygonCount[ARRAYSIZE(vertices)]{ 0 };
};

typedef shared_ptr<CubeRenderer>	CubeRendererPointer;

