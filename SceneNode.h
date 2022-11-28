#pragma once
#include "core.h"
#include "DirectXCore.h"

using namespace std;

// Abstract base class for all nodes of the scene graph.  
// This scene graph implements the Composite Design Pattern

class SceneNode;

typedef shared_ptr<SceneNode>	SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name) {_name = name;};
	~SceneNode(void) {};

	// Core methods
	virtual bool Initialise() = 0;
	virtual void Update(const Matrix& worldTransformation) { _cumulativeWorldTransformation = _thisWorldTransformation * worldTransformation; }	
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	void SetWorldTransform(const Matrix& worldTransformation) { _thisWorldTransformation = worldTransformation; }
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base for recursive operations

	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

protected:
	Matrix				_thisWorldTransformation;
	Matrix				_cumulativeWorldTransformation;
	wstring				_name;

	typedef struct
	{
		Matrix		WorldViewProjection;
		Matrix		World;
		Vector4		AmbientLightColour;
		Vector4		DirectionalLightColour;
		Vector4		DirectionalLightVector;
	}  CBuffer;

	typedef struct {
		Vector3 Position;
		Vector3 Normal;
	}Vertex;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	Vector4							_ambientLightColour;
	ComPtr<ID3D11RenderTargetView>	_renderTargetView;

	ComPtr<ID3D11Buffer>			_vertexBuffer;
	ComPtr<ID3D11Buffer>			_indexBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;
	ComPtr<ID3D11InputLayout>		_layout;
	ComPtr<ID3D11Buffer>			_constantBuffer;

	ComPtr<ID3D11RasterizerState>   _rasteriserState;

	Vector3							_eyePosition;
	Vector3							_focalPointPosition;
	Vector3							_upVector;
};

