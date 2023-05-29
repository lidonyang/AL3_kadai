#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "ImGuiManager.h"
#include"AxisIndicator.h"


GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete player_;
	delete model_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	//worldTransform_.Initialize();
	viewProjection_.Initialize();

	//playerの生成
	player_ = new Player();
	player_->Initialize(model_,textureHandle_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵の速度
	const float kenemySpeed = 0.0f;
	Vector3 velocity(kenemySpeed, kenemySpeed, kenemySpeed);
	//敵の生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, {15, 2, 50}, velocity);

	//敵キャラに自キャラのアドレス渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update() 
{
	//プレーヤーの更新
	player_->Update(); 

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACKSPACE)) {
		// デバッグカメラ有効フラグをトグル
		isDebugCameraActive_ = true;
	}
#endif
	// カメラの処理
	if (isDebugCameraActive_) 
	{
		// デバッグカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列転送
		viewProjection_.TransferMatrix();
	} 
	else 
	{
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
	//敵の更新
	enemy_->Update();

	//当たり判定読み込む
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	 
	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 当たり判定
		float dx = posB.x - posA.x;
		float dy = posB.y - posA.y;
		float dz = posB.z - posA.z;
		float d = dx * dx + dy * dy + dz * dz;
		
		float r = (player_->GetRadius() + bullet->GetRadius()) *  (player_->GetRadius() + bullet->GetRadius());
		// 球と球の交差判定
		if (d <= r) {
			player_->OnCollision();

			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 当たり判定
		float dx = posB.x - posA.x;
		float dy = posB.y - posA.y;
		float dz = posB.z - posA.z;
		float d = dx * dx + dy * dy + dz * dz;

		float r = (enemy_->GetRadius() + bullet->GetRadius()) *
		          (enemy_->GetRadius() + bullet->GetRadius());
		// 球と球の交差判定
		if (d <= r) {
			enemy_->OnCollision();

			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) 
	{
		//自弾の座標
		posA = bullet->GetWorldPosition();
		for (EnemyBullet* bullets : enemyBullets)
		{
			// 敵弾の座標
			posB = bullets->GetWorldPosition();
			// 当たり判定
			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float dz = posB.z - posA.z;
			float d = dx * dx + dy * dy + dz * dz;

			float r = (bullets->GetRadius() + bullet->GetRadius()) *
			          (bullets->GetRadius() + bullet->GetRadius());
			// 球と球の交差判定
			if (d <= r) {
				bullets->OnCollision();

				bullet->OnCollision();
			}
		}

	}

#pragma endregion
}
