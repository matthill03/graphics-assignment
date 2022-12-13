#include "TexturedCubeNode.h"


bool TexturedCubeNode::Initialise() {
	BuildTexture();

	if (DirectXFramework::GetDXFramework()->GetTexturedCubeRenderer()->Initialise()) {
		return true;
	}
	return false;
}

void TexturedCubeNode::Render() {
	// Calculate the world x view x projection transformation
	Matrix completeTransformation = _cumulativeWorldTransformation * DirectXFramework::GetDXFramework()->GetViewTransformation() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	CBuffer constantBuffer;
	constantBuffer.WorldViewProjection = completeTransformation;
	constantBuffer.World = _cumulativeWorldTransformation;
	constantBuffer.DirectionalLightVector = DirectXFramework::GetDXFramework()->GetDirectionalLightVector();
	constantBuffer.DirectionalLightColour = DirectXFramework::GetDXFramework()->GetDirectionalLightColour();
	constantBuffer.EyePosition = _eyePosition;
	constantBuffer.SpecularPower = DirectXFramework::GetDXFramework()->GetSpecularPower();
	constantBuffer.SpecularColour = DirectXFramework::GetDXFramework()->GetSpecularColour();

	DirectXFramework::GetDXFramework()->GetTexturedCubeRenderer()->Render(constantBuffer, _texture);
}

void TexturedCubeNode::Shutdown()
{
}

void TexturedCubeNode::BuildTexture()
{
	ThrowIfFailed(CreateWICTextureFromFile(DirectXFramework::GetDXFramework()->GetDevice().Get(),
		DirectXFramework::GetDXFramework()->GetDeviceContext().Get(),
		_textureName.c_str(),
		nullptr,
		_texture.GetAddressOf()
	));
}













