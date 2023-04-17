#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include"DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private:
	float inputFloat3[3] = {0, 0, 0};
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
	//モデル生成
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
