#include "Enemy.h"
#include<assert.h>
#include"Player.h"
#include<cmath>
#include"GameScene.h"

Enemy::~Enemy() {
	// buller_の解放
	/*for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}*/
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
	//// デスフラグの立った弾の削除
	//bullets_.remove_if([](EnemyBullet* bullet) {
	//	if (bullet->IsDead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	//});

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
	
	/*if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}*/

	//弾の更新
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}*/

}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	

	////弾の描画
	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}
}

void Enemy::Fire() 
{
	assert(player_);

	//弾の速度
	const float kBullerSpeed = 1.0f;
	// 自キャラの座標を取得
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵の座標を取得
	Vector3 enemyPos = GetWorldPosition();
	// 差分ベクトルを計算
	Vector3 velocity; // = playerPos - enemyPos;
	/*velocity.x = enemyPos.x - playerPos.x;
	velocity.y = enemyPos.y - playerPos.y;
	velocity.z = enemyPos.z - playerPos.z;*/
	velocity.x = playerPos.x - enemyPos.x;
	velocity.y = playerPos.y - enemyPos.y;
	velocity.z = playerPos.z - enemyPos.z;
	// ベクトルの正規化
	float length = sqrtf((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z));
		
	Vector3 Dir;
	Dir.x = velocity.x / length;
	Dir.y = velocity.y / length;
	Dir.z = velocity.z / length;

	// ベクトルの長さを、速さに合わせる
	velocity.x = Dir.x * kBullerSpeed;
	velocity.y = Dir.y * kBullerSpeed;
	velocity.z = Dir.z * kBullerSpeed;

		//Vector3 velocity(0, 0, kBullerSpeed);

		////速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
	    EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		//bullet_ = newBullet;
		//bullets_.push_back(newBullet);
	    gameScene_->AddEnemyBullet(newBullet);
		
}
Vector3 Enemy::GetWorldPosition() {
	    // ワールド座標を入れる変数
	    Vector3 worldPos;
	    // ワールド行列の平行移動成分を取得
	    worldPos.x = worldTransform_.translation_.x;
	    worldPos.y = worldTransform_.translation_.y;
	    worldPos.z = worldTransform_.translation_.z;

	    return worldPos;
}

void Enemy::OnCollision() { isDead_ = true; }
