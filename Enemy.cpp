#include "Enemy.h"
#include<assert.h>


Enemy::~Enemy() {
	// buller_の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("guagua.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// 接近フェーズ初期化
	InitializeApproach();
}

// 接近フェーズ初期化
void Enemy::InitializeApproach() 
{
	// 発射初期化初期化
	fireTimer_ = kFireInterval;
}

void Enemy::UpdateApproach()
{
	worldTransform_.translation_.x += 0.0f;
	worldTransform_.translation_.y += 0.0f;
	worldTransform_.translation_.z -= velocity_.z;
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	fireTimer_--;
	if (fireTimer_ <= 0) {
		Fire();
		fireTimer_ = kFireInterval;
	}
}

void Enemy::UpdateLeave()
{
	worldTransform_.translation_.x -= velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	if (worldTransform_.translation_.z > 50.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Update()
{
	// デスフラグの立った弾の削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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


	//弾の更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() 
{

		//弾の速度
		const float kBullerSpeed = 1.0f;
		Vector3 velocity(0, 0, kBullerSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
	    EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
		
}