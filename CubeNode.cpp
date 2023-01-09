#include "CubeNode.h"

// Implementing Initialise()
bool CubeNode::Initialise() {
	//if (DirectXFramework::GetDXFramework()->GetCubeRenderer()->Initialise()) {
	//	return true;
	//}
	return true;
	//return false;
}

// Implementing Render()
void CubeNode::Render() {

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

	DirectXFramework::GetDXFramework()->GetCubeRenderer()->Render(constantBuffer);

}

// Implementing Shutdown()
void CubeNode::Shutdown() {
	// delete this instance from memory
	delete[] this;
}

