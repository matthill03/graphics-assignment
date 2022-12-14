#include "TeapotRenderer.h"

bool TeapotRenderer::Initialise()
{
	// Calling all initialisation methods
	PopulateTeapotVertices();
	CalculateVertexNormals();
	BuildGeometryBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildRasteriserState();

	// When finished return true
	return true;
}

void TeapotRenderer::Render(CBuffer constantBuffer)
{
	// Update the constant buffer. Note the layout of the constant buffer must match that in the shader
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &constantBuffer, 0, 0);

	// Now render the cube
	// Specify the distance between vertices and the starting point in the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Set the vertex buffer and index buffer we are going to use
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Specify the layout of the polygons (it will rarely be different to this)
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Specify the layout of the input vertices.  This must match the layout of the input vertices in the shader
	_deviceContext->IASetInputLayout(_layout.Get());

	// Specify the vertex and pixel shaders we are going to use
	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);

	// Specify details about how the object is to be drawn
	_deviceContext->RSSetState(_rasteriserState.Get());

	// Now draw the first cube
	_deviceContext->DrawIndexed(ARRAYSIZE(indices), 0, 0);
}

void TeapotRenderer::BuildGeometryBuffers()
{
	// This method uses the arrays defined in Geometry.h
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
	ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

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
	ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void TeapotRenderer::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(ShaderFileName,
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
	ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));

	// Compile pixel shader
	hr = D3DCompileFromFile(ShaderFileName,
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
	ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
}

void TeapotRenderer::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it. The vertexDesc array is
	// defined in Geometry.h

	ThrowIfFailed(_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void TeapotRenderer::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void TeapotRenderer::BuildRasteriserState()
{
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
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _rasteriserState.GetAddressOf()));
}

void TeapotRenderer::CalculateVertexNormals()
{
	// For all verticies, set corresponding polygon count to 0
	for (int i = 0; i < ARRAYSIZE(vertices); i++) {
		polygonCount[i] = 0;
	}

	// For all polygons calculate normal, and add to each vertex in polygon
	for (int i = 0; i < ARRAYSIZE(indices); i += 3) {

		// Calculate polygonNormal for current polygon
		Vector3 polygonNormal = CalculatePolygonNormal(vertices[indices[i]].Position, vertices[indices[i + 1]].Position, vertices[indices[i + 2]].Position);

		// Add polygonNormal to each vertex normal in the polygon
		vertices[indices[i]].Normal = vertices[indices[i]].Normal + polygonNormal;
		// increment coresponding polygon count
		polygonCount[indices[i]]++;

		vertices[indices[i + 1]].Normal = vertices[indices[i + 1]].Normal + polygonNormal;
		polygonCount[indices[i + 1]]++;

		vertices[indices[i + 2]].Normal = vertices[indices[i + 2]].Normal + polygonNormal;
		polygonCount[indices[i + 2]]++;
	}

	// Work out normal at each vertex 
	for (int i = 0; i < ARRAYSIZE(vertices); i++) {
		// Take vertex normal and divide by coresponding polygonCount
		// this takes the average normal
		vertices[i].Normal /= polygonCount[i];

		// Then normalise normal, this will give each x, y and z a value of
		// -1, 0 or 1 for direction of the normal
		vertices[i].Normal.Normalize();
	}
}

Vector3 TeapotRenderer::CalculatePolygonNormal(Vector3 p1, Vector3 p2, Vector3 p3)
{
	// Calculate vectors from point 1 to point 2, and point 1 to point 3
	Vector3 v12 = p1 - p2;
	Vector3 v13 = p1 - p3;

	// Calculate the cross product to give perpendicular vector (normalVector)
	Vector3 normalVector = v12.Cross(v13);

	// Return result
	return normalVector;
}

void TeapotRenderer::PopulateTeapotVertices()
{
	for (int i = 0; i < ARRAYSIZE(vertexFloats); i += 3) {
		vertices[i / 3] = { Vector3(vertexFloats[i], vertexFloats[i + 1], vertexFloats[i + 2]), Vector3(0, 0, 0) };
	}
}
