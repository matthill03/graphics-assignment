#include "DirectXApp.h"

DirectXApp app;

void DirectXApp::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	/* Now add nodes to the scene graph*/

	SceneNodePointer head = SceneNodePointer(new TexturedCubeNode(L"Head", Vector4(1.0f, 1.0f, 1.0f, 1.0f), L"woodbox.bmp"));

	//// Scale * Rotaion * Translation
	head->SetWorldTransform(Matrix::CreateScale(Vector3(3, 3, 3)) * Matrix::CreateTranslation(Vector3(0, 34, 0)));
	//// thisWorldTransform[0][0] = vector.x; // thisWorldTransform[0][1] = vector.y; // thisWorldTransform[0][2] = vector.z;

	SceneNodePointer nose = SceneNodePointer(new CubeNode(L"Nose", Vector4(0.4f, 0.1f, 0.1f, 1.0f)));
	nose->SetWorldTransform(Matrix::CreateTranslation(Vector3(0, 33, -4)));

	SceneNodePointer body = SceneNodePointer(new TexturedCubeNode(L"Body", Vector4(0.2f, 0.0f, 0.4f, 1.0f), L"woodbox.bmp"));
	body->SetWorldTransform(Matrix::CreateScale(Vector3(5, 8, 2.5)) * Matrix::CreateTranslation(Vector3(0, 23, 0)));

	SceneNodePointer leftArm = SceneNodePointer(new CubeNode(L"Left Arm", Vector4(0.0f, 0.4f, 0.0f, 1.0f)));
	leftArm->SetWorldTransform(Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateTranslation(Vector3(-6, 22, 0)));

	SceneNodePointer rightArm = SceneNodePointer(new CubeNode(L"Right Arm", Vector4(0.0f, 0.4f, 0.0f, 1.0f)));
	rightArm->SetWorldTransform(Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateRotationX(1.6) * Matrix::CreateTranslation(Vector3(6, 22, 0)));

	SceneNodePointer leftLeg = SceneNodePointer(new CubeNode(L"Left Leg", Vector4(0.1f, 0.2f, 0.3f, 1.0f)));
	leftLeg->SetWorldTransform(Matrix::CreateScale(Vector3(1, 7.5, 1)) * Matrix::CreateTranslation(Vector3(-4, 7.5, 0)));

	SceneNodePointer rightLeg = make_shared<CubeNode>(L"Right Leg", Vector4(0.1f, 0.2f, 0.3f, 1.0f));
	rightLeg->SetWorldTransform(Matrix::CreateScale(Vector3(1, 7.5, 1)) * Matrix::CreateTranslation(Vector3(4, 7.5, 0)));

	SceneNodePointer randomTeapot = make_shared<TeapotNode>(L"Teapot", Vector4(0.1f, 0.2f, 0.2f, 1.0f));
	randomTeapot->SetWorldTransform(Matrix::CreateScale(Vector3(10, 10, 10)));

	SceneNodePointer randomModel = make_shared<ModelNode>(L"Model", Vector4(0.1f, 0.2f, 0.2f, 1.0f), L"airplane.x");
	randomModel->SetWorldTransform(Matrix::CreateScale(Vector3(10, 10, 10)));



	sceneGraph->Add(head);
	sceneGraph->Add(nose);

	sceneGraph->Add(body);

	sceneGraph->Add(leftArm);
	sceneGraph->Add(rightArm);

	sceneGraph->Add(leftLeg);
	sceneGraph->Add(rightLeg);

	/*sceneGraph->Add(randomTeapot);*/
	sceneGraph->Add(randomModel);

}

void DirectXApp::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();
	
	
	SceneNodePointer leftArm = sceneGraph->Find(L"Left Arm");
	SceneNodePointer rightArm = sceneGraph->Find(L"Right Arm");
	SceneNodePointer leftLeg = sceneGraph->Find(L"Left Leg");
	SceneNodePointer rightLeg = sceneGraph->Find(L"Right Leg");
	SceneNodePointer model = sceneGraph->Find(L"Model");

	_yPosition += 0.1;
	DirectXFramework::GetDXFramework()->SetSpecularDirection(Vector4(0.0f, _yPosition, 0.0f, 0.0f));

	if (_leftArmRotation > 0.6f) {
		_swingDirection = false;
	}
	else if (_leftArmRotation < -1.0f) {
		_swingDirection = true;
	}
	else {
		leftArm->SetWorldTransform((Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateTranslation(Vector3(0, -8.5, 0)) * Matrix::CreateRotationX(_leftArmRotation) * Matrix::CreateTranslation(Vector3(-6, 30.5, 0))));
		rightArm->SetWorldTransform((Matrix::CreateScale(Vector3(1, 8.5, 1)) * Matrix::CreateTranslation(Vector3(0, -8.5, 0)) * Matrix::CreateRotationX(_rightArmRotation) * Matrix::CreateTranslation(Vector3(6, 30.5, 0))));

		leftLeg->SetWorldTransform((Matrix::CreateScale(Vector3(1, 7.5, 1)) * Matrix::CreateTranslation(Vector3(0, -7.5, 0)) * Matrix::CreateRotationX(_rightArmRotation) * Matrix::CreateTranslation(Vector3(-4, 15, 0))));
		rightLeg->SetWorldTransform((Matrix::CreateScale(Vector3(1, 7.5, 1)) * Matrix::CreateTranslation(Vector3(0, -7.5, 0)) * Matrix::CreateRotationX(_leftArmRotation) * Matrix::CreateTranslation(Vector3(4, 15, 0))));

	}

	if (_swingDirection) {
		_leftArmRotation += 0.01f;
		_rightArmRotation -= 0.01f;
	}
	else {
		_leftArmRotation -= 0.01f;
		_rightArmRotation += 0.01f;
	}

	model->SetWorldTransform(Matrix::CreateScale(Vector3(5, 5, 5)) * Matrix::CreateTranslation(Vector3(30, 0, 0)) * Matrix::CreateRotationY(_rotationAngle * XM_PI / 360));

	_rotationAngle += 1;
	sceneGraph->SetWorldTransform(Matrix::CreateRotationY(_rotationAngle * XM_PI / 720));
	
}
