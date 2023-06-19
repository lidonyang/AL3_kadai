#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include<list>


class Player;
class GameScene;

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void UpdateApproach();// 接近する関数

	void UpdateLeave();// 離脱する関数
	//移動フェ－ズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	 /// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	~Enemy();

	// 接近フェーズ初期化関数
	void InitializeApproach();


	//発射間隔
	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetWorldPosition();

	//衝突判定関数
	void OnCollision();

	// 弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	//半径設定
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	bool GetIsDead() { return isDead_; }
	

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	// 速度
	Vector3 velocity_;

	//フェ－ズ
	Phase phase_ = Phase::Approach;

	// 弾
	EnemyBullet* bullet_ = nullptr;

	//std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t fireTimer_ = 0;

	//自キャラ
	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	bool isDead_ = false;
	uint32_t deathTimer_ = 300;
};
