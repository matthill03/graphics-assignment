#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"
#include "CBuffer.h"
#include "WICTextureLoader.h"

#define ShaderFileName		L"shader.hlsl"
#define VertexShaderName	"VS"
#define PixelShaderName		"PS"

// Create derived class from scene node for models 
class ModelNode : public SceneNode
{
	// Setting public methods and variables
public:
	/*Defining Constructors*/
	// Default constructor
	ModelNode() : SceneNode(L"Root") {};
	// Constructor to specify name and colour of cube
	ModelNode(wstring name, Vector4 colour, wstring model_name) : SceneNode(name) { _ambientLightColour = colour; _modelName = model_name; };
	// Destructor
	~ModelNode(void) {};

private:
	// Core methods, to override base methods
	virtual bool Initialise();
	virtual void Render();
	virtual void Shutdown();

	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11DeviceContext>		_deviceContext;
	Matrix							_viewTransformation;
	Matrix							_projectionTransformation;

	wstring								_modelName;
	wstring								_textureName;

	ComPtr<ID3D11ShaderResourceView>	_texture;

	shared_ptr<ResourceManager>			_resourceManager;
	shared_ptr<Mesh>					_mesh;
	size_t								_meshCount;
	shared_ptr<SubMesh>					_currentMesh;
	shared_ptr<Material>				_currentMaterial;

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

	//void BuildGeometryBuffers();
	void BuildShaders(LPCWSTR pixelShaderName);
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRasteriserState();
	//void CalculateVertexNormals();
	//Vector3 CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3);

	// Defines the Vertex struct for CubeNode, with a Position and Normal
	typedef struct {
		Vector3 Position;
		Vector3 Normal;
		Vector2 Texture;
	}Vertex;

	// Defines the Vertex Description for when vertices is passed into shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURE" , 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
};

