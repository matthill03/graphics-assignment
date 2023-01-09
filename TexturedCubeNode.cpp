#include "TexturedCubeNode.h"


bool TexturedCubeNode::Initialise() {
	// Build the texture used on cube.
	BuildTexture();

	return true;
}

void TexturedCubeNode::Render() {
	// Calculate the world x view x projection transformation
	Matrix completeTransformation = _cumulativeWorldTransformation * DirectXFramework::GetDXFramework()->GetViewTransformation() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	// Populate CBuffer with correct values
	CBuffer constantBuffer;
	constantBuffer.WorldViewProjection = completeTransformation;
	constantBuffer.World = _cumulativeWorldTransformation;
	constantBuffer.DirectionalLightVector = DirectXFramework::GetDXFramework()->GetDirectionalLightVector();
	constantBuffer.DirectionalLightColour = DirectXFramework::GetDXFramework()->GetDirectionalLightColour();
	constantBuffer.EyePosition = DirectXFramework::GetDXFramework()->GetEyePosition();
	constantBuffer.SpecularPower = DirectXFramework::GetDXFramework()->GetSpecularPower();
	constantBuffer.SpecularColour = DirectXFramework::GetDXFramework()->GetSpecularColour();
	constantBuffer.SpecularDirection = DirectXFramework::GetDXFramework()->GetSpecularDirection();

	// Call TexturedCubeRender using the texture that has been built
	DirectXFramework::GetDXFramework()->GetTexturedCubeRenderer()->Render(constantBuffer, _texture);
}

void TexturedCubeNode::Shutdown()
{
	// Delete this object from memory
	delete[] this;
}

void TexturedCubeNode::BuildTexture()
{
	// Build texture using the WICTextureLoader file provided.
	ThrowIfFailed(CreateWICTextureFromFile(DirectXFramework::GetDXFramework()->GetDevice().Get(),
		DirectXFramework::GetDXFramework()->GetDeviceContext().Get(),
		_textureName.c_str(),
		nullptr,
		_texture.GetAddressOf()
	));
}













