#pragma once
#include "core.h"
#include "DirectXCore.h"
#include <vector>
#include <memory>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoord;
};


// Core material class.  Ideally, this should be extended to include more material attributes that can be
// recovered from Assimp, but this handles the basics.

class Material
{
public:
	Material(wstring materialName, Vector4 diffuseColour, Vector4 specularColour, float shininess, float opacity, ComPtr<ID3D11ShaderResourceView> texture );
	~Material();

	inline wstring							GetMaterialName() { return _materialName;  }
	inline Vector4							GetDiffuseColour() { return _diffuseColour; }
	inline Vector4							GetSpecularColour() { return _specularColour; }
	inline float							GetShininess() { return _shininess; }
	inline float							GetOpacity() { return _opacity; }
	inline ComPtr<ID3D11ShaderResourceView>	GetTexture() { return _texture; }

private:
	wstring									_materialName;
	Vector4									_diffuseColour;
	Vector4									_specularColour;
	float									_shininess;
	float									_opacity;
    ComPtr<ID3D11ShaderResourceView>		_texture;
};

// Basic SubMesh class.  A Mesh consists of one or more sub-meshes.  The submesh provides everything that is needed to
// draw the sub-mesh.

class SubMesh
{
public:
	SubMesh(ComPtr<ID3D11Buffer> vertexBuffer,
		ComPtr<ID3D11Buffer> indexBuffer,
		size_t vertexCount,
		size_t indexCount,
		shared_ptr<Material> material,
		bool hasNormals,
		bool hasTexCoords);
		
	~SubMesh();

	inline ComPtr<ID3D11Buffer>			GetVertexBuffer() { return _vertexBuffer; }
	inline ComPtr<ID3D11Buffer>			GetIndexBuffer() { return _indexBuffer; }
	inline shared_ptr<Material>			GetMaterial() { return _material; }
	inline size_t						GetVertexCount() { return _vertexCount; }
	inline size_t						GetIndexCount() { return _indexCount; }
	inline bool							HasNormals() { return _hasNormals; }
	inline bool							HasTexCoords() { return _hasTexCoords; }

private:
   	ComPtr<ID3D11Buffer>				_vertexBuffer;
	ComPtr<ID3D11Buffer>				_indexBuffer;
	shared_ptr<Material>				_material;
	size_t								_vertexCount;
	size_t								_indexCount;
	bool								_hasNormals;
	bool								_hasTexCoords;
};

// Core mesh class

class Mesh
{
public:
	size_t								GetSubMeshCount();
	shared_ptr<SubMesh>					GetSubMesh(unsigned int i);
	void								AddSubMesh(shared_ptr<SubMesh> subMesh);

private:
	vector<shared_ptr<SubMesh>> 		_subMeshList;
};


