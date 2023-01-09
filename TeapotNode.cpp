#include "TeapotNode.h"

bool TeapotNode::Initialise()
{
	// When successfull return true
	return true;
}

void TeapotNode::Render()
{
	// Calculate the world x view x projection transformation
	Matrix completeTransformation = _cumulativeWorldTransformation * DirectXFramework::GetDXFramework()->GetViewTransformation() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();

	// Create constant buffer
	CBuffer constantBuffer;
	constantBuffer.WorldViewProjection = completeTransformation;
	constantBuffer.World = _cumulativeWorldTransformation;
	constantBuffer.AmbientLightColour = _ambientLightColour;
	constantBuffer.DirectionalLightVector = DirectXFramework::GetDXFramework()->GetDirectionalLightVector();
	constantBuffer.DirectionalLightColour = DirectXFramework::GetDXFramework()->GetDirectionalLightColour();
	constantBuffer.EyePosition = DirectXFramework::GetDXFramework()->GetEyePosition();
	constantBuffer.SpecularPower = DirectXFramework::GetDXFramework()->GetSpecularPower();
	constantBuffer.SpecularColour = DirectXFramework::GetDXFramework()->GetSpecularColour();
	constantBuffer.SpecularDirection = DirectXFramework::GetDXFramework()->GetSpecularDirection();

	// Call render with the TeapotRenderer passing in this constant buffer.
	DirectXFramework::GetDXFramework()->GetTeapotRenderer()->Render(constantBuffer);
}

void TeapotNode::Shutdown()
{
	free(this);
}




