#include "Enemy.h"
#include<assert.h>

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("guagua.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void Enemy::Update()
{
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
