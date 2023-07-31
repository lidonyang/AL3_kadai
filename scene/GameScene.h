#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include"Player.h"
#include "DebugCamera.h"
#include"Enemy.h"
#include"Skydome.h"
#include"RailCamera.h"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

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

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void AddEnemy(Vector3 pos);

	void LoadEnemyPopDate();

	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	// WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラカメラ
	DebugCamera* debugCamera_ = nullptr;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemy_;

	//衝突判定と応答
	void CheckAllCollisions();

	//3Dモデルの生成
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//弾リスト
	std::list<EnemyBullet*> bullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//待機フラグ
	bool isflag = false;
	//待機タイマー
	uint32_t istime = 60;

	//プレーヤの命
	uint32_t wintextureHandle = 0;
	Sprite* winsprite_ = nullptr;
	Sprite* win1sprite_ = nullptr;
	Sprite* win2sprite_ = nullptr;
	float playerflag_ = 3.0f;
	float enemyflag = 5.0f;

	//シーン
	float scene = 0.0f;
	uint32_t starttextureHandle = 0;
	Sprite* startsprite_ = nullptr;
	uint32_t gamewintextureHandle = 0;
	Sprite* gamewinsprite_ = nullptr;
	uint32_t gameovertextureHandle = 0;
	Sprite* gameoversprite_ = nullptr;

};
