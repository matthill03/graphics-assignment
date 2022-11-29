#include "DirectXApp.h"
#include "CubeNode.h"

DirectXApp app;

void DirectXApp::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	// Now add nodes to the scene graph

	SceneNodePointer head = SceneNodePointer(new CubeNode(L"Head", Vector4(0.4f, 0.1f, 0.1f, 1.0f)));

	// Scale * Rotaion * Translation
	head->SetWorldTransform(Matrix::CreateScale(Vector3(3, 3, 3)) * Matrix::CreateTranslation(Vector3(0, 34, 0)));
	// thisWorldTransform[0][0] = vector.x; // thisWorldTransform[0][1] = vector.y; // thisWorldTransform[0][2] = vector.z;

	SceneNodePointer nose = SceneNodePointer(new CubeNode(L"Nose", Vector4(0.4f, 0.1f, 0.1f, 1.0f)));
	nose->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, 33, 4)));

	SceneNodePointer body = SceneNodePointer(new CubeNode(L"Body", Vector4(0.2f, 0.0f, 0.4f, 1.0f)));
	body->SetWorldTransform(Matrix::CreateScale(Vector3(5, 8, 2.5)) * Matrix::CreateTranslation(Vector3(0, 23, 0)));

	SceneNodePointer leftArm = SceneNodePointer(new CubeNode(L"Left Arm", Vector4(0.0f, 0.4f, 0.0f, 1.0f)));
	leftArm->SetWorldTransform(Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateTranslation(Vector3(-6, 22, 0)));

	SceneNodePointer rightArm = SceneNodePointer(new CubeNode(L"Right Arm", Vector4(0.0f, 0.4f, 0.0f, 1.0f)));
	rightArm->SetWorldTransform(Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateTranslation(Vector3(6, 22, 0)));

	SceneNodePointer leftLeg = SceneNodePointer(new CubeNode(L"Left Leg", Vector4(0.1f, 0.2f, 0.3f, 1.0f)));
	leftLeg->SetWorldTransform(Matrix::CreateScale(Vector3(1, 7.5, 1)) * Matrix::CreateTranslation(Vector3(-4, 7.5, 0)));

	SceneNodePointer rightLeg = SceneNodePointer(new CubeNode(L"Right Leg", Vector4(0.1f, 0.2f, 0.3f, 1.0f)));
	rightLeg->SetWorldTransform(Matrix::CreateScale(Vector3(1, 7.5, 1)) * Matrix::CreateTranslation(Vector3(4, 7.5, 0)));


	sceneGraph->Add(head);
	sceneGraph->Add(nose);

	sceneGraph->Add(body);

	sceneGraph->Add(leftArm);
	sceneGraph->Add(rightArm);

	sceneGraph->Add(leftLeg);
	sceneGraph->Add(rightLeg);

}

void DirectXApp::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");
	leftArm->SetWorldTransform((Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateRotationX(1) * Matrix::CreateTranslation(Vector3(-6, 22, 0))));

	sceneGraph->SetWorldTransform(Matrix::CreateRotationY(_rotationAngle * XM_PI / 180.0f));
	_rotationAngle = (_rotationAngle + 1) % 360;
}
