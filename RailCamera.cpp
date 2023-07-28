#include "RailCamera.h"
#include "ImGuiManager.h"


void RailCamera::Initialize(Vector3 trans, Vector3 rot) 
{ 
	worldTransform_.Initialize();
	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rot;
	
	viewProjection_.Initialize(); 
}

void RailCamera::Update() 
{
	Vector3 move = {0.0f,0.0f,0.0f,};
	Vector3 rot = {0.0f, 0.001f, 0.0f};

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation_", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation_", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();

}
