#include "ModelNode.h"

bool ModelNode::Initialise()
{
    _resourceManager = DirectXFramework::GetDXFramework()->GetResourceManger();
	_device = DirectXFramework::GetDXFramework()->GetDevice();
	_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

    _mesh = _resourceManager->GetMesh(_modelName);

	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildRasteriserState();

	return true;
}

void ModelNode::Shutdown()
{
    _resourceManager->ReleaseMesh(_modelName);
}

void ModelNode::Render()
{
	// Calculate the world x view x projection transformation
	Matrix completeTransformation = _cumulativeWorldTransformation * DirectXFramework::GetDXFramework()->GetViewTransformation() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	CBuffer constantBuffer;
	constantBuffer.WorldViewProjection = completeTransformation;
	constantBuffer.World = _cumulativeWorldTransformation;
	constantBuffer.DirectionalLightVector = DirectXFramework::GetDXFramework()->GetDirectionalLightVector();
	constantBuffer.SpecularDirection = DirectXFramework::GetDXFramework()->GetSpecularDirection();
	constantBuffer.EyePosition = DirectXFramework::GetDXFramework()->GetEyePosition();

	_meshCount = _mesh->GetSubMeshCount();

	for (UINT i = 0; i < _meshCount; i++) {
		_currentMesh = _mesh->GetSubMesh(UINT(i));
		_currentMaterial = _currentMesh->GetMaterial();

		constantBuffer.SpecularPower = _currentMaterial->GetShininess();
		constantBuffer.SpecularColour = _currentMaterial->GetSpecularColour();
		constantBuffer.DirectionalLightColour = _currentMaterial->GetDiffuseColour();

		if (_currentMesh->HasTexCoords()) {
			_texture = _currentMaterial->GetTexture();
			_deviceContext->PSSetShaderResources(0, 1, _texture.GetAddressOf());
			_deviceContext->PSSetShader(_texturePixelShader.Get(), 0, 0);
		}
		else {
			_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
		}

		// Update the constant buffer. Note the layout of the constant buffer must match that in the shader
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &constantBuffer, 0, 0);

		// Set Texture to be used in render

		// Now render the cube
		// Specify the distance between vertices and the starting point in the vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		// Set the vertex buffer and index buffer we are going to use
		_deviceContext->IASetVertexBuffers(0, 1, _currentMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_currentMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		// Specify the layout of the polygons (it will rarely be different to this)
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Specify the layout of the input vertices.  This must match the layout of the input vertices in the shader
		_deviceContext->IASetInputLayout(_layout.Get());

		// Specify the vertex and pixel shaders we are going to use
		_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
		//deviceContext->PSSetShaderResources();

		// Specify details about how the object is to be drawn
		_deviceContext->RSSetState(_rasteriserState.Get());

		// Now draw the first cube
		_deviceContext->DrawIndexed(UINT(_currentMesh->GetIndexCount()), 0, 0);
	}
}

void ModelNode::BuildShaders()
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

	//Compile vertex shader
	hr = D3DCompileFromFile(TextureShaderFileName,
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
	ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _textureVertexShader.GetAddressOf()));

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
	ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _texturePixelShader.GetAddressOf()));
}

void ModelNode::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it. The vertexDesc array is
	// defined in Geometry.h

	ThrowIfFailed(_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void ModelNode::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void ModelNode::BuildRasteriserState()
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
