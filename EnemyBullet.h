#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class EnemyBullet 
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// 衝突判定関数
	void OnCollision();

	// 半径設定
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;
	// 半径
	static const int kBadius = 1;


	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	// 速度
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
