#pragma once
#include "Core.h"
#include "HelperFunctions.h"
#include "CBuffer.h"

#define TextureShaderFileName		L"textureShader.hlsl"
#define VertexShaderName			"VS"
#define PixelShaderName				"PS"

class TexturedCubeRenderer
{
public:
	/* Defining Constructor */
	TexturedCubeRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Matrix viewTransformation, Matrix projectionTransformation) { _device = device; _deviceContext = deviceContext; _viewTransformation = viewTransformation; _projectionTransformation = projectionTransformation; };

	// Public methods
	void Render(CBuffer constantBuffer, ComPtr<ID3D11ShaderResourceView> _texture);
	bool Initialise();
private:
	// Member variables
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

	// Private methods for initialising
	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRasteriserState();
	void CalculateVertexNormals();
	Vector3 CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3);

	// Vertex struct
	typedef struct {
		Vector3 Position;
		Vector3 Normal;
		Vector2 Texture;
	}Vertex;

	// Vertex description for graphics card.
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

	// array for the polygon count of verticies
	float polygonCount[ARRAYSIZE(vertices)]{ 0 };
};

// Creating shorthand for creating a textured cube renderer pointer
typedef shared_ptr<TexturedCubeRenderer>	TexturedCubeRendererPointer;

