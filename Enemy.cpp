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

void Enemy::UpdateApproach()
{
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z -= velocity_.z;
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave()
{
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y -= velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	if (worldTransform_.translation_.z > 50.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Update()
{
	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		UpdateApproach();
		
		break;
	case Enemy::Phase::Leave:
		UpdateLeave();
		
		break;
	
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

