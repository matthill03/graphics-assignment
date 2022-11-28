#include "DirectXApp.h"
#include "CubeNode.h"

DirectXApp app;

void DirectXApp::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	// Now add nodes to the scene graph

	SceneNodePointer testCube = std::shared_ptr<SceneNode>(new CubeNode(L"Test Node", Color(Colors::AliceBlue)));

	sceneGraph->Add(testCube);
}

void DirectXApp::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	sceneGraph->SetWorldTransform(Matrix::CreateRotationX(_rotationAngle * XM_PI / 180.0f));
	_rotationAngle = (_rotationAngle + 1) % 360;
}
