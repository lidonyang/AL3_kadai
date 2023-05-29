﻿#include "PlayerBullet.h"
#include<assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model); 
	model_ = model;

	textureHandle_ = TextureManager::Load("cube.jpg");

	worldTransform_.Initialize();
	worldTransform_.translation_= position;

	velocity_ = velocity;
}

void PlayerBullet::Update() 
{
	worldTransform_.UpdateMatrix();
	
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() 
{ 
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}