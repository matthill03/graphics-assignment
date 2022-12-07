#include "TexturedCubeNode.h"


bool TexturedCubeNode::Initialise() {
	_device = DirectXFramework::GetDXFramework()->GetDevice();
	_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	_viewTransformation = DirectXFramework::GetDXFramework()->GetViewTransformation();
	_projectionTransformation = DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	CalculateVertexNormals();
	BuildGeometryBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildRasteriserState();
	BuildTexture();

	return true;
}

void TexturedCubeNode::Render() {

	DirectXFramework* dxFramework = DirectXFramework::GetDXFramework();
	ComPtr<ID3D11DeviceContext> deviceContext = dxFramework->GetDeviceContext();
	Matrix viewTransformation = dxFramework->GetViewTransformation();
	Matrix projectionTransformation = dxFramework->GetProjectionTransformation();

	// Calculate the world x view x projection transformation
	Matrix completeTransformation = _cumulativeWorldTransformation * viewTransformation * projectionTransformation;

	CBuffer constantBuffer;
	constantBuffer.WorldViewProjection = completeTransformation;
	constantBuffer.World = _cumulativeWorldTransformation;
	constantBuffer.DirectionalLightVector = Vector4(-1.0f, -1.0f, 1.0f, 0.0f);
	constantBuffer.DirectionalLightColour = Vector4(Colors::Beige);
	constantBuffer.EyePosition = _eyePosition;
	constantBuffer.SpecularPower = 0.5f;
	constantBuffer.SpecularColour = Vector4(1.0f, 1.0f, 1.0f, 0.0f);

	// Update the constant buffer. Note the layout of the constant buffer must match that in the shader
	deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &constantBuffer, 0, 0);

	// Set Texture to be used in render
	_deviceContext->PSSetShaderResources(0, 1, _texture.GetAddressOf());

	// Now render the cube
	// Specify the distance between vertices and the starting point in the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Set the vertex buffer and index buffer we are going to use
	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Specify the layout of the polygons (it will rarely be different to this)
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Specify the layout of the input vertices.  This must match the layout of the input vertices in the shader
	deviceContext->IASetInputLayout(_layout.Get());

	// Specify the vertex and pixel shaders we are going to use
	deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
	//deviceContext->PSSetShaderResources();

	// Specify details about how the object is to be drawn
	deviceContext->RSSetState(_rasteriserState.Get());

	// Now draw the first cube
	deviceContext->DrawIndexed(ARRAYSIZE(indices), 0, 0);
}

void TexturedCubeNode::Shutdown() {

}

Vector3 TexturedCubeNode::CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3) {
	Vector3 v12 = p1 - p2;
	Vector3 v13 = p1 - p3;

	Vector3 normalVector = v12.Cross(v13);

	return normalVector;
}

void TexturedCubeNode::CalculateVertexNormals() {
	for (int i = 0; i < ARRAYSIZE(vertices); i++) {
		polygonCount[i] = 0;
	}

	for (int i = 0; i < ARRAYSIZE(indices); i += 3) {
		Vector3 polygonNormal = CalculatePolygonNormal(vertices[indices[i]].Position, vertices[indices[i + 1]].Position, vertices[indices[i + 2]].Position);

		vertices[indices[i]].Normal = vertices[indices[i]].Normal + polygonNormal;
		polygonCount[indices[i]]++;

		vertices[indices[i + 1]].Normal = vertices[indices[i + 1]].Normal + polygonNormal;
		polygonCount[indices[i + 1]]++;

		vertices[indices[i + 2]].Normal = vertices[indices[i + 2]].Normal + polygonNormal;
		polygonCount[indices[i + 2]]++;
	}

	for (int i = 0; i < ARRAYSIZE(vertices); i++) {
		vertices[i].Normal /= polygonCount[i];
		vertices[i].Normal.Normalize();
	}
}

void TexturedCubeNode::BuildGeometryBuffers() {
	DirectXFramework* dxFramework = DirectXFramework::GetDXFramework();
	ComPtr<ID3D11Device> device = dxFramework->GetDevice();


	// This method uses the arrays defined in Geometry.h
	// 
	// Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor = { 0 };
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the vertices from
	D3D11_SUBRESOURCE_DATA vertexInitialisationData = { 0 };
	vertexInitialisationData.pSysMem = &vertices;

	// and create the vertex buffer
	ThrowIfFailed(device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	// Setup the structure that specifies how big the index 
	// buffer should be
	D3D11_BUFFER_DESC indexBufferDescriptor = { 0 };
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the indices from
	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = &indices;

	// and create the index buffer
	ThrowIfFailed(device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void TexturedCubeNode::BuildShaders() {
	DirectXFramework* dxFramework = DirectXFramework::GetDXFramework();
	ComPtr<ID3D11Device> device = dxFramework->GetDevice();

	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(TextureShaderFileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		VertexShaderName, "vs_5_0",
		shaderCompileFlags, 0,
		_vertexShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));

	// Compile pixel shader
	hr = D3DCompileFromFile(TextureShaderFileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		PixelShaderName, "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
}

void TexturedCubeNode::BuildVertexLayout()
{
	DirectXFramework* dxFramework = DirectXFramework::GetDXFramework();
	ComPtr<ID3D11Device> device = dxFramework->GetDevice();
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it. The vertexDesc array is
	// defined in Geometry.h

	ThrowIfFailed(device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void TexturedCubeNode::BuildConstantBuffer() {
	DirectXFramework* dxFramework = DirectXFramework::GetDXFramework();
	ComPtr<ID3D11Device> device = dxFramework->GetDevice();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void TexturedCubeNode::BuildRasteriserState() {
	DirectXFramework* dxFramework = DirectXFramework::GetDXFramework();
	ComPtr<ID3D11Device> device = dxFramework->GetDevice();

	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	ThrowIfFailed(device->CreateRasterizerState(&rasteriserDesc, _rasteriserState.GetAddressOf()));
}

void TexturedCubeNode::BuildTexture() {
	ThrowIfFailed(CreateWICTextureFromFile(_device.Get(),
		_deviceContext.Get(), 
		_textureName.c_str(), 
		nullptr, 
		_texture.GetAddressOf()
	));
}

