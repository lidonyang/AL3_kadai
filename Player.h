#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include<list>


class Player
{
public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle,Vector3 position);

	///< summary>
	/// 更新
	///  </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	void Draw(ViewProjection& viewProjection);

    /// <summary>
    /// 攻撃
    /// </summary>
	void Attack();
	
	~Player();

	Vector3 GetWorldPosition();

	// 衝突判定関数
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	// 半径設定
	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	//親となるワールドトランスフォームー
	void SetParent(const WorldTransform* parent);

private:
	//ワ－ルド変換デ－タ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キー入力
	Input* input_ = nullptr;
	//弾
	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

};